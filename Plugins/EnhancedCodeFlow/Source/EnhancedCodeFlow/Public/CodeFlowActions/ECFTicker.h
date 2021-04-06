// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFActionBase.h"
#include "ECFTicker.generated.h"

UCLASS()
class ENHANCEDCODEFLOW_API UECFTicker : public UECFActionBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

protected:

	TUniqueFunction<void(float)> TickFunc;
	TUniqueFunction<void()> CallbackFunc;
	float TickingTime;

	float CurrentTime;

	bool Setup(float InTickingTime, TUniqueFunction<void(float)>&& InTickFunc, TUniqueFunction<void()> InCallbackFunc = nullptr)
	{
		TickingTime = InTickingTime;
		TickFunc = MoveTemp(InTickFunc);
		CallbackFunc = MoveTemp(InCallbackFunc);

		if (TickFunc && (TickingTime > 0.f || TickingTime == -1.f))
		{
			if (TickingTime > 0.f)
			{
				SetMaxActionTime(TickingTime);
			}

			CurrentTime = 0.f;
			return true;
		}
		else
		{
			return false;
		}
	}

	void Tick(float DeltaTime) override
	{
		TickFunc(DeltaTime);
		CurrentTime += DeltaTime;
		if (TickingTime > 0.f && CurrentTime >= TickingTime)
		{
			if (CallbackFunc)
			{
				CallbackFunc();
			}
			MarkAsFinished();
		}
	}

};
