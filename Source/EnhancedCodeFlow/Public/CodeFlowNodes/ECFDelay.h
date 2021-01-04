// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFNodeBase.h"
#include "ECFDelay.generated.h"

UCLASS()
class ENHANCEDCODEFLOW_API UECFDelay : public UECFNodeBase
{
	GENERATED_BODY()

protected:

	TUniqueFunction<void()> Func;
	float DelayTime;
	float CurrentTime;

public:

	bool Setup(float InDelayTime, TUniqueFunction<void()>&& InFunc)
	{
		DelayTime = InDelayTime;
		Func = MoveTemp(InFunc);

		if (Func && DelayTime >= 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

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
			MarkAsFinished();
		}
	}

	bool IsValid() override
	{
		return Super::IsValid() && Func;
	}
};
