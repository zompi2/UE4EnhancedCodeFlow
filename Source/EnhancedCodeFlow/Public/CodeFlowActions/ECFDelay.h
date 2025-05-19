// Copyright (c) 2025 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFActionBase.h"
#include "ECFDelay.generated.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UCLASS()
class ENHANCEDCODEFLOW_API UECFDelay : public UECFActionBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

protected:

	TUniqueFunction<void(bool)> CallbackFunc;
	TUniqueFunction<void()> CallbackFunc_NoStopped;
	float DelayTime = 0.f;
	float CurrentTime = 0.f;

	bool Setup(float InDelayTime, TUniqueFunction<void(bool)>&& InCallbackFunc)
	{
		DelayTime = InDelayTime;
		CallbackFunc = MoveTemp(InCallbackFunc);

		if (CallbackFunc && DelayTime >= 0)
		{
			if (DelayTime > 0)
			{
				SetMaxActionTime(DelayTime);
			}
			return true;
		}
		else
		{
#if ECF_LOGS
			UE_LOG(LogECF, Error, TEXT("ECF - delay failed to start. Are you sure the DelayTime is not negative and Callback Function is set properly?"));
#endif
			return false;
		}
	}

	bool Setup(float InDelayTime, TUniqueFunction<void()>&& InCallbackFunc)
	{
		CallbackFunc_NoStopped = MoveTemp(InCallbackFunc);
		if (CallbackFunc_NoStopped)
		{
			return Setup(InDelayTime, [this](bool bStopped)
			{
				CallbackFunc_NoStopped();
			});			
		}
		else
		{
#if ECF_LOGS
			UE_LOG(LogECF, Error, TEXT("ECF - delay failed to start. Are you sure the Callback Function is set properly?"));
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
		DECLARE_SCOPE_CYCLE_COUNTER(TEXT("Delay - Tick"), STAT_ECFDETAILS_DELAY, STATGROUP_ECFDETAILS);
#endif

#if ECF_INSIGHT_PROFILING
		TRACE_CPUPROFILER_EVENT_SCOPE("ECF - Delay Tick");
#endif

		CurrentTime += DeltaTime;
		if (CurrentTime > DelayTime)
		{
			MarkAsFinished();
			Complete(false);
		}
	}

	void Complete(bool bStopped) override
	{
		CallbackFunc(bStopped);
	}
};

ECF_PRAGMA_ENABLE_OPTIMIZATION