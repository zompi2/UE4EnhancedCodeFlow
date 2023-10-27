// Copyright (c) 2023 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFActionBase.h"
#include "ECFTicker_WithHandle.generated.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UCLASS()
class ENHANCEDCODEFLOW_API UECFTicker_WithHandle : public UECFActionBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

protected:

	TUniqueFunction<void(float, FECFHandle)> TickFunc;
	TUniqueFunction<void(bool)> CallbackFunc;
	float TickingTime = 0.f;
	float CurrentTime = 0.f;

	bool Setup(float InTickingTime, TUniqueFunction<void(float, FECFHandle)>&& InTickFunc, TUniqueFunction<void(bool)>&& InCallbackFunc = nullptr)
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
#if STATS
		DECLARE_SCOPE_CYCLE_COUNTER(TEXT("Ticker - Tick"), STAT_ECFDETAILS_TICKER, STATGROUP_ECFDETAILS);
#endif
		TickFunc(DeltaTime, HandleId);
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