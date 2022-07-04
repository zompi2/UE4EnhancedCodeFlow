// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

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

	bool Setup(TUniqueFunction<bool()>&& InPredicate, TUniqueFunction<void(float)>&& InTickFunc)
	{
		Predicate = MoveTemp(InPredicate);
		TickFunc = MoveTemp(InTickFunc);

		if (Predicate && TickFunc)
		{
			if (Predicate() == false)
			{
				return false;
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
		if (Predicate())
		{
			TickFunc(DeltaTime);
		}
		else
		{
			MarkAsFinished();
		}
	}
};
