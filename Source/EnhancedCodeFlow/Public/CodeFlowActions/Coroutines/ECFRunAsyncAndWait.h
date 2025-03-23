// Copyright (c) 2025 Damian Nowakowski. All rights reserved.

#pragma once

#include "Coroutines/ECFCoroutineActionBase.h"
#include "Templates/Atomic.h"
#include "Async/Async.h"
#include "ECFTypes.h"
#include "Async/TaskGraphInterfaces.h"
#include "ECFRunAsyncAndWait.generated.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UCLASS()
class ENHANCEDCODEFLOW_API UECFRunAsyncAndWait: public UECFCoroutineActionBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

protected:

	TUniqueFunction<void()> AsyncTaskFunc;
	float TimeOut = 0.f;
	float OriginTimeOut = 0.f;
	bool bWithTimeOut = false;

	ENamedThreads::Type ThreadType = ENamedThreads::AnyBackgroundThreadNormalTask;
	TAtomic<bool> bIsAsyncTaskDone = false;

	bool Setup(TUniqueFunction<void()>&& InAsyncTaskFunc, float InTimeOut, EECFAsyncPrio InThreadPriority)
	{
		AsyncTaskFunc = MoveTemp(InAsyncTaskFunc);

		switch (InThreadPriority)
		{
			case EECFAsyncPrio::Normal:
				ThreadType = ENamedThreads::AnyBackgroundThreadNormalTask;
				break;
			case EECFAsyncPrio::HiPriority:
				ThreadType = ENamedThreads::AnyBackgroundHiPriTask;
				break;
		}

		if (AsyncTaskFunc)
		{
			if (InTimeOut > 0.f)
			{
				bWithTimeOut = true;
				TimeOut = InTimeOut;
				OriginTimeOut = InTimeOut;
				SetMaxActionTime(TimeOut);
			}
			else
			{
				bWithTimeOut = false;
			}

			bIsAsyncTaskDone = false;

			TWeakObjectPtr<ThisClass> WeakThis(this);
			AsyncTask(ThreadType, [WeakThis]()
			{
				if (ThisClass* StrongThis = WeakThis.Get())
				{
					StrongThis->AsyncTaskFunc();
					StrongThis->bIsAsyncTaskDone = true;
				}
			});

			return true;
		}
		else
		{
#if ECF_LOGS
			UE_LOG(LogECF, Error, TEXT("ECF Coroutine - Run Async Task and Wait failed to start. Are you sure the AsyncTask function is set properly?"));
#endif
			return false;
		}
	}

	void Reset(bool bCallUpdate) override
	{
		if (bWithTimeOut)
		{
			TimeOut = OriginTimeOut;
		}
	}

	void Tick(float DeltaTime) override
	{
#if STATS
		DECLARE_SCOPE_CYCLE_COUNTER(TEXT("RunAsyncAndWait - Tick"), STAT_ECFDETAILS_RUNASYNCANDWAIT, STATGROUP_ECFDETAILS);
#endif

#if ECF_INSIGHT_PROFILING
		TRACE_CPUPROFILER_EVENT_SCOPE("ECF - RunAsyncAndWait Tick");
#endif

		if (bWithTimeOut)
		{
			TimeOut -= DeltaTime;
			if (TimeOut <= 0.f)
			{
				MarkAsFinished();
				Complete(false);
				return;
			}
		}

		if (bIsAsyncTaskDone)
		{
			MarkAsFinished();
			Complete(false);
		}
	}

	void Complete(bool bStopped) override
	{
		CoroutineHandle.resume();
	}
};

ECF_PRAGMA_ENABLE_OPTIMIZATION