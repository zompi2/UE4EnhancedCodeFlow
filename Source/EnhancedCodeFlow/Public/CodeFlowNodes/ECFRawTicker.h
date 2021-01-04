// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFNodeBase.h"
#include "ECFRawTicker.generated.h"

UCLASS()
class ENHANCEDCODEFLOW_API UECFRawTicker : public UECFNodeBase
{
	GENERATED_BODY()

protected:

	TUniqueFunction<void(float)> Func;

public:

	void Tick(float DeltaTime) override 
	{
		Func(DeltaTime);
	}

	bool IsValid() override
	{
		return Super::IsValid() && Func;
	}

	bool Setup(TUniqueFunction<void(float)>&& InFunc) 
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
};
