// Copyright (c) 2023 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFActionBase.h"
#include "ECFTicker.generated.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UCLASS()
class ENHANCEDCODEFLOW_API UECFTicker : public UECFActionBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

protected:

	TUniqueFunction<void(float)> TickFunc;
	TUniqueFunction<void(bool)> CallbackFunc;
	float TickingTime;

	float CurrentTime;

	bool Setup(float InTickingTime, TUniqueFunction<void(float)>&& InTickFunc, TUniqueFunction<void(bool)>&& InCallbackFunc = nullptr)
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
			Complete(false);
			MarkAsFinished();
		}
	}

	void Complete(bool bStopped) override
	{
		if (CallbackFunc)
		{
			CallbackFunc(bStopped);
		}
	}
};

ECF_PRAGMA_ENABLE_OPTIMIZATION