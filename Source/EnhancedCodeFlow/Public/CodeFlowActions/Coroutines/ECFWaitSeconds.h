// Copyright (c) 2025 Damian Nowakowski. All rights reserved.

#pragma once

#include "Coroutines/ECFCoroutineActionBase.h"
#include "ECFWaitSeconds.generated.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UCLASS()
class ENHANCEDCODEFLOW_API UECFWaitSeconds : public UECFCoroutineActionBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

protected:

	float WaitTime = 0.f;
	float CurrentTime = 0.f;

	bool Setup(float InWaitTime)
	{
		WaitTime = InWaitTime;

		if (WaitTime >= 0)
		{
			if (WaitTime > 0)
			{
				SetMaxActionTime(WaitTime);
			}
			return true;
		}
		else
		{
#if ECF_LOGS
			UE_LOG(LogECF, Error, TEXT("ECF Coroutine - wait seconds failed to start. Are you sure the WaitTime is not negative?"));
#endif
			return false;
		}
	}

	void Init() override
	{
		CurrentTime = 0;
	}

	void Reset(bool bCallUpdate) override
	{
		CurrentTime = 0;
	}

	void Tick(float DeltaTime) override
	{
#if STATS
		DECLARE_SCOPE_CYCLE_COUNTER(TEXT("WaitSeconds - Tick"), STAT_ECFDETAILS_WAITSECONDS, STATGROUP_ECFDETAILS);
#endif

#if ECF_INSIGHT_PROFILING
		TRACE_CPUPROFILER_EVENT_SCOPE("ECF - WaitSeconds Tick");
#endif

		CurrentTime += DeltaTime;
		if (CurrentTime > WaitTime)
		{
			MarkAsFinished();
			Complete(false);
		}
	}

	void Complete(bool bStopped) override
	{
		CoroutineHandle.resume();
	}
};

ECF_PRAGMA_ENABLE_OPTIMIZATION