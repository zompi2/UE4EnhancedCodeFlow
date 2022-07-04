// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFActionBase.h"
#include "ECFWaitAndExecute.generated.h"

UCLASS()
class ENHANCEDCODEFLOW_API UECFWaitAndExecute : public UECFActionBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

protected:

	TUniqueFunction<bool()> Predicate;
	TUniqueFunction<void()> Func;

	bool Setup(TUniqueFunction<bool()>&& InPredicate, TUniqueFunction<void()>&& InFunc)
	{
		Predicate = MoveTemp(InPredicate);
		Func = MoveTemp(InFunc);

		if (Predicate && Func)
		{
			if (Predicate())
			{
				Func();
				return false;
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
		if (Predicate())
		{
			Complete();
			MarkAsFinished();
		}
	}

	void Complete() override
	{
		Func();
	}
};
