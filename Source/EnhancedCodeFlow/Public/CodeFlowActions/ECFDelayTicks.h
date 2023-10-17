// Copyright (c) 2023 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFActionBase.h"
#include "ECFDelayTicks.generated.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UCLASS()
class ENHANCEDCODEFLOW_API UECFDelayTicks : public UECFActionBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

protected:

	TUniqueFunction<void(bool)> CallbackFunc;
	int32 DelayTicks = 0;
	int32 CurrentTicks = 0;

	bool Setup(int32 InDelayTicks, TUniqueFunction<void(bool)>&& InCallbackFunc)
	{
		DelayTicks = InDelayTicks;
		CallbackFunc = MoveTemp(InCallbackFunc);

		if (CallbackFunc && DelayTicks >= 0)
		{
			return true;
		}
		else
		{
			ensureMsgf(false, TEXT("ECF - delay ticks failed to start. Are you sure the DelayTicks is not negative and Callback Function is set properly?"));
			return false;
		}
	}

	void Init() override
	{
		CurrentTicks = 0;
	}

	void Tick(float DeltaTime) override
	{
#if STATS
		DECLARE_SCOPE_CYCLE_COUNTER(TEXT("DelayTicks - Tick"), STAT_ECFDETAILS_DELAYTICKS, STATGROUP_ECFDETAILS);
#endif
		CurrentTicks++;
		if (CurrentTicks > DelayTicks)
		{
			Complete(false);
			MarkAsFinished();
		}
	}

	void Complete(bool bStopped) override
	{
		CallbackFunc(bStopped);
	}
};

ECF_PRAGMA_ENABLE_OPTIMIZATION