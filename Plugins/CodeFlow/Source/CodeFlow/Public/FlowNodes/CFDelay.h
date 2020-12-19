// Copyright (c) 2020 Damian Nowakowski. All rights reserved.

#pragma once

#include "CFNodeBase.h"
#include "CFDelay.generated.h"

UCLASS()
class CODEFLOW_API UCFDelay : public UCFNodeBase
{
	GENERATED_BODY()

public:

	void Init() override 
	{
		CurrentTime = 0;
	}

	void Tick(float DeltaTime) override 
	{
		CurrentTime += DeltaTime;
		if (CurrentTime > DelayTime)
		{
			Func();
			bHasFinished = true;
		}
	}

	TUniqueFunction<void()> Func;
	float DelayTime;
	float CurrentTime;

	bool IsValid() override
	{
		return Super::IsValid() && Func;
	}

	void Setup(float InDelayTime, TUniqueFunction<void()>&& InFunc)
	{
		DelayTime = InDelayTime;
		Func = MoveTemp(InFunc);
	}
};
