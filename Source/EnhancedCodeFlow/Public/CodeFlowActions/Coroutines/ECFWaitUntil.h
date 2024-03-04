// Copyright (c) 2024 Damian Nowakowski. All rights reserved.

#pragma once

#include "Coroutines/ECFCoroutineActionBase.h"
#include "ECFWaitUntil.generated.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UCLASS()
class ENHANCEDCODEFLOW_API UECFWaitUntil : public UECFCoroutineActionBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

protected:

	TUniqueFunction<bool(float)> Predicate;
	float TimeOut = 0.f;
	bool bWithTimeOut = false;

	bool Setup(TUniqueFunction<bool(float)>&& InPredicate, float InTimeOut)
	{
		Predicate = MoveTemp(InPredicate);

		if (Predicate)
		{
			if (Predicate(0.f))
			{
				CoroutineHandle.resume();
				return false;
			}
			if (InTimeOut > 0.f)
			{
				bWithTimeOut = true;
				TimeOut = InTimeOut;
				SetMaxActionTime(TimeOut);
			}
			else
			{
				bWithTimeOut = false;
			}
			return true;
		}
		else
		{
			ensureMsgf(false, TEXT("ECF Coroutine - Wait Until failed to start. Are you sure the Predicate is set properly?"));
			return false;
		}
	}

	void Tick(float DeltaTime) override
	{
#if STATS
		DECLARE_SCOPE_CYCLE_COUNTER(TEXT("WaitUntil - Tick"), STAT_ECFDETAILS_WAITUNTIL, STATGROUP_ECFDETAILS);
#endif
		if (bWithTimeOut)
		{
			TimeOut -= DeltaTime;
			if (TimeOut <= 0.f)
			{
				Complete(false);
				MarkAsFinished();
				return;
			}
		}

		if (Predicate(DeltaTime))
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