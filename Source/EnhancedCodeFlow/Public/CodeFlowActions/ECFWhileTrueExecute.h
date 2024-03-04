// Copyright (c) 2024 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFActionBase.h"
#include "ECFWhileTrueExecute.generated.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UCLASS()
class ENHANCEDCODEFLOW_API UECFWhileTrueExecute : public UECFActionBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

protected:

	TUniqueFunction<bool()> Predicate;
	TUniqueFunction<void(float)> TickFunc;
	TUniqueFunction<void(bool, bool)> CompleteFunc;

	float TimeOut = 0.f;
	bool bWithTimeOut = false;
	bool bTimedOut = false;

	bool Setup(TUniqueFunction<bool()>&& InPredicate, TUniqueFunction<void(float)>&& InTickFunc, TUniqueFunction<void(bool, bool)>&& InCompleteFunc, float InTimeOut)
	{
		Predicate = MoveTemp(InPredicate);
		TickFunc = MoveTemp(InTickFunc);
		CompleteFunc = MoveTemp(InCompleteFunc);

		if (Predicate && TickFunc)
		{
			if (Predicate() == false)
			{
				if (CompleteFunc)
				{
					CompleteFunc(false, false);
				}
				return false;
			}
			if (InTimeOut > 0.f)
			{
				bWithTimeOut = true;
				TimeOut = InTimeOut;
				bTimedOut = false;
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
			ensureMsgf(false, TEXT("ECF - While True Execute failed to start. Are you sure the Predicate and Function are set properly?"));
			return false;
		}
	}

	void Tick(float DeltaTime) override 
	{
#if STATS
		DECLARE_SCOPE_CYCLE_COUNTER(TEXT("WhileTrueExecute - Tick"), STAT_ECFDETAILS_WHILETRUEEXECUTE, STATGROUP_ECFDETAILS);
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
			TickFunc(DeltaTime);
		}
		else
		{
			Complete(false);
			MarkAsFinished();
		}
	}

	void Complete(bool bStopped) override
	{
		if (CompleteFunc)
		{
			CompleteFunc(bTimedOut, bStopped);
		}
	}
};

ECF_PRAGMA_ENABLE_OPTIMIZATION