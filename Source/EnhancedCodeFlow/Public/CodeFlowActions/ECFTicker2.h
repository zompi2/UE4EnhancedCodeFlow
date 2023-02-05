// Copyright (c) 2023 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFActionBase.h"
#include "ECFTicker2.generated.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UCLASS()
class ENHANCEDCODEFLOW_API UECFTicker2 : public UECFActionBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

protected:

	TUniqueFunction<void(float, FECFHandle)> TickFunc;
	TUniqueFunction<void()> CallbackFunc;
	float TickingTime;

	float CurrentTime;

	bool Setup(float InTickingTime, TUniqueFunction<void(float, FECFHandle)>&& InTickFunc, TUniqueFunction<void()> InCallbackFunc = nullptr)
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
			ensureMsgf(false, TEXT("ECF - Ticker(2) failed to start. Are you sure the Ticking time and Ticking Function are set properly?"));
			return false;
		}
	}

	void Tick(float DeltaTime) override
	{
		TickFunc(DeltaTime, HandleId);
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

ECF_PRAGMA_ENABLE_OPTIMIZATION