// Copyright (c) 2023 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFCoroutineActionBase.h"
#include "ECFWaitTicks.generated.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UCLASS()
class ENHANCEDCODEFLOW_API UECFWaitTicks : public UECFCoroutineActionBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

protected:

	int32 WaitTicks = 0;
	int32 CurrentTicks = 0;

	bool Setup(int32 InWaitTicks)
	{
		WaitTicks = InWaitTicks;

		if (WaitTicks >= 0)
		{
			return true;
		}
		else
		{
			ensureMsgf(false, TEXT("ECF Coroutine - wait ticks failed to start. Are you sure the WaitTicks is not negative?"));
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
		DECLARE_SCOPE_CYCLE_COUNTER(TEXT("WaitTicks - Tick"), STAT_ECFDETAILS_WAITTICKS, STATGROUP_ECFDETAILS);
#endif
		CurrentTicks++;
		if (CurrentTicks > WaitTicks)
		{
			Complete(false);
			MarkAsFinished();
		}
	}

	void Complete(bool bStopped) override
	{
		CoroutineHandle.resume();
	}
};

ECF_PRAGMA_ENABLE_OPTIMIZATION