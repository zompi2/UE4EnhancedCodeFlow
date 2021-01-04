// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFNodeBase.h"
#include "ECFRawTicker.generated.h"

UCLASS()
class ENHANCEDCODEFLOW_API UECFRawTicker : public UECFNodeBase
{
	GENERATED_BODY()

public:

	void Tick(float DeltaTime) override 
	{
		Func(DeltaTime);
	}

	TUniqueFunction<void(float)> Func;

	bool IsValid() override
	{
		return Super::IsValid() && Func;
	}

	void Setup(TUniqueFunction<void(float)>&& InFunc) 
	{
		Func = MoveTemp(InFunc);
	}
};
