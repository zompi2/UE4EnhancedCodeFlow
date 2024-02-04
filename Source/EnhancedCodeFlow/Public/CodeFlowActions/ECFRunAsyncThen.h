// Copyright (c) 2024 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFActionBase.h"
#include "Templates/Atomic.h"
#include "Async/Async.h"
#include "ECFRunAsyncThen.generated.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UCLASS()
class ENHANCEDCODEFLOW_API UECFRunAsyncThen : public UECFActionBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

protected:

	TUniqueFunction<void()> AsyncTaskFunc;
	TUniqueFunction<void(bool, bool)> Func;

	float TimeOut = 0.f;
	bool bWithTimeOut = false;
	bool bTimedOut = false;

	TAtomic<bool> bIsAsyncTaskDone = false;

	bool Setup(TUniqueFunction<void()>&& InAsyncTaskFunc, TUniqueFunction<void(bool, bool)>&& InFunc, float InTimeOut, ENamedThreads::Type ThreadType)
	{
		if (IsInGameThread() == false)
		{
			return false;
		}

		AsyncTaskFunc = MoveTemp(InAsyncTaskFunc);
		Func = MoveTemp(InFunc);

		if (AsyncTaskFunc && Func)
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
			AsyncTask(ThreadType, [this]()
			{
				AsyncTaskFunc();
				bIsAsyncTaskDone = true;
			});

			return true;
		}
		else
		{
			ensureMsgf(false, TEXT("ECF - Run Async Task and Run failed to start. Are you sure the AsyncTask and Function are set properly? Are you sure you run this function from GameThread?"));
			return false;
		}
	}

	void Tick(float DeltaTime) override 
	{
#if STATS
		DECLARE_SCOPE_CYCLE_COUNTER(TEXT("RunAsyncThen - Tick"), STAT_ECFDETAILS_RUNASYNCTHEN, STATGROUP_ECFDETAILS);
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
		Func(bTimedOut, bStopped);
	}
};

ECF_PRAGMA_ENABLE_OPTIMIZATION