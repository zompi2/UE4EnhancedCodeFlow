// Copyright (c) 2023 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFCoroutineActionBase.h"
#include "ECFWaitSeconds.generated.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UCLASS()
class ENHANCEDCODEFLOW_API UECFWaitSeconds : public UECFCoroutineActionBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

protected:

	float DelayTime;
	float CurrentTime;

	bool Setup(float InDelayTime)
	{
		DelayTime = InDelayTime;

		if (DelayTime >= 0)
		{
			if (DelayTime > 0)
			{
				SetMaxActionTime(DelayTime);
			}
			return true;
		}
		else
		{
			ensureMsgf(false, TEXT("ECF Coroutine - wait seconds failed to start. Are you sure the DelayTime is not negative?"));
			return false;
		}
	}

	void Init() override
	{
		CurrentTime = 0;
	}

	void Tick(float DeltaTime) override
	{
#if STATS
		DECLARE_SCOPE_CYCLE_COUNTER(TEXT("WaitSeconds - Tick"), STAT_ECFDETAILS_WAITSECONDS, STATGROUP_ECFDETAILS);
#endif
		CurrentTime += DeltaTime;
		if (CurrentTime > DelayTime)
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