// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFNodeBase.h"
#include "ECFWaitAndExecute.generated.h"

UCLASS()
class ENHANCEDCODEFLOW_API UECFWaitAndExecute : public UECFNodeBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

public:

	bool IsValid() const override
	{
		return Super::IsValid() && Predicate && Func;
	}

protected:

	TUniqueFunction<bool()> Predicate;
	TUniqueFunction<void()> Func;

	bool Setup(TUniqueFunction<bool()>&& InPredicate, TUniqueFunction<void()>&& InFunc)
	{
		Predicate = MoveTemp(InPredicate);
		Func = MoveTemp(InFunc);

		if (Predicate && Func)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void Tick(float DeltaTime) override 
	{
		if (Predicate())
		{
			Func();
			MarkAsFinished();
		}
	}
};
