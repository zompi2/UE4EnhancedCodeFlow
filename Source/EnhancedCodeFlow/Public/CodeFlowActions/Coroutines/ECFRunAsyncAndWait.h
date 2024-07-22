// Copyright (c) 2024 Damian Nowakowski. All rights reserved.

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
	bool bWithTimeOut = false;
	bool bTimedOut = false;

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
				bTimedOut = false;
				TimeOut = InTimeOut;
				SetMaxActionTime(TimeOut);
			}
			else
			{
				bWithTimeOut = false;
				bTimedOut = false;
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
			ensureMsgf(false, TEXT("ECF Coroutine - Run Async Task and Wait failed to start. Are you sure the AsyncTask function is set properly?"));
			return false;
		}
	}

	void Tick(float DeltaTime) override
	{
#if STATS
		DECLARE_SCOPE_CYCLE_COUNTER(TEXT("RunAsyncAndWait - Tick"), STAT_ECFDETAILS_RUNASYNCANDWAIT, STATGROUP_ECFDETAILS);
#endif
		if (bWithTimeOut)
		{
			TimeOut -= DeltaTime;
			if (TimeOut <= 0.f)
			{
				bTimedOut = true;
				Complete(false);
				MarkAsFinished();
				return;
			}
		}

		if (bIsAsyncTaskDone)
		{
			Complete(false);
			MarkAsFinished();
		}
	}

	void Complete(bool bStopped) override
	{
		CoroutineHandle.resume();
	}
};

ECF_PRAGMA_ENABLE_OPTIMIZATION