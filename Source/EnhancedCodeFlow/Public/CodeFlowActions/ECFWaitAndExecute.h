// Copyright (c) 2023 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFActionBase.h"
#include "ECFWaitAndExecute.generated.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UCLASS()
class ENHANCEDCODEFLOW_API UECFWaitAndExecute : public UECFActionBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

protected:

	TUniqueFunction<bool()> Predicate;
	TUniqueFunction<void(bool, bool)> Func;

	float TimeOut = 0.f;
	bool bWithTimeOut = false;
	bool bTimedOut = false;

	bool Setup(TUniqueFunction<bool()>&& InPredicate, TUniqueFunction<void(bool, bool)>&& InFunc, float InTimeOut)
	{
		Predicate = MoveTemp(InPredicate);
		Func = MoveTemp(InFunc);

		if (Predicate && Func)
		{
			if (Predicate())
			{
				Func(false, false);
				return false;
			}
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
			return true;
		}
		else
		{
			ensureMsgf(false, TEXT("ECF - Wait and Execute failed to start. Are you sure the Predicate and Function are set properly?"));
			return false;
		}
	}

	void Tick(float DeltaTime) override 
	{
#if STATS
		DECLARE_SCOPE_CYCLE_COUNTER(TEXT("WaitAndExecute - Tick"), STAT_ECFDETAILS_WAITANDEXECUTE, STATGROUP_ECFDETAILS);
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

		if (Predicate())
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