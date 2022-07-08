// Copyright (c) 2022 Damian Nowakowski. All rights reserved.

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
			ensureMsgf(false, TEXT("ECF - Ticker failed to start. Are you sure the Ticking time and Ticking Function are set properly?"));
			return false;
		}
	}

	void Tick(float DeltaTime) override
	{
		TickFunc(DeltaTime);
		CurrentTime += DeltaTime;
		if (TickingTime > 0.f && CurrentTime >= TickingTime)
		{
			Complete();
			MarkAsFinished();
		}
	}

	void Complete() override
	{
		if (CallbackFunc)
		{
			CallbackFunc();
		}
	}
};
