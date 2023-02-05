// Copyright (c) 2023 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFActionBase.h"
#include "ECFWhileTrueExecute.generated.h"

UCLASS()
class ENHANCEDCODEFLOW_API UECFWhileTrueExecute : public UECFActionBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

protected:

	TUniqueFunction<bool()> Predicate;
	TUniqueFunction<void(float)> TickFunc;
	TUniqueFunction<void(bool)> CompleteFunc;

	float TimeOut = 0.f;
	bool bWithTimeOut = false;
	bool bTimedOut = false;

	bool Setup(TUniqueFunction<bool()>&& InPredicate, TUniqueFunction<void(float)>&& InTickFunc, TUniqueFunction<void(bool)>&& InCompleteFunc, float InTimeOut)
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
					CompleteFunc(false);
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
		if (bWithTimeOut)
		{
			TimeOut -= DeltaTime;
			if (TimeOut <= 0.f)
			{
				bTimedOut = true;
				Complete();
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
			Complete();
			MarkAsFinished();
		}
	}

	void Complete() override
	{
		if (CompleteFunc)
		{
			CompleteFunc(bTimedOut);
		}
	}
};
