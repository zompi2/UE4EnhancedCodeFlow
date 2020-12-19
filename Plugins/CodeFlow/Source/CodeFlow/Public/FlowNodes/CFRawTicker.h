// Copyright (c) 2020 Damian Nowakowski. All rights reserved.

#pragma once

#include "CFNodeBase.h"
#include "CFRawTicker.generated.h"

UCLASS()
class CODEFLOW_API UCFRawTicker : public UCFNodeBase
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
};
