// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFNodeBase.h"
#include "ECFTicker2.generated.h"

UCLASS()
class ENHANCEDCODEFLOW_API UECFTicker2 : public UECFNodeBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

public:

	bool IsValid() const override
	{
		return Super::IsValid() && Func;
	}

protected:

	TUniqueFunction<void(float, FECFHandle)> Func;

	bool Setup(TUniqueFunction<void(float, FECFHandle)>&& InFunc)
	{
		Func = MoveTemp(InFunc);
		if (Func)
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
		Func(DeltaTime, HandleId);
	}

};
