// Copyright (c) 2025 Damian Nowakowski. All rights reserved.

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
	TUniqueFunction<void()> CallbackFunc_NoStopped;
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
#if ECF_LOGS
			UE_LOG(LogECF, Error, TEXT("ECF - [%s] delay ticks failed to start. Are you sure the DelayTicks is not negative and Callback Function is set properly?"), *Settings.Label);
#endif
			return false;
		}
	}

	bool Setup(int32 InDelayTicks, TUniqueFunction<void()>&& InCallbackFunc)
	{
		CallbackFunc_NoStopped = MoveTemp(InCallbackFunc);
		if (CallbackFunc_NoStopped)
		{
			return Setup(InDelayTicks, [this](bool bStopped)
			{
				CallbackFunc_NoStopped();
			});
		}
		else
		{
#if ECF_LOGS
			UE_LOG(LogECF, Error, TEXT("ECF - delay ticks failed to start. Are you sure the Callback Function is set properly?"));
#endif
			return false;
		}
	}

	void Init() override
	{
		CurrentTicks = 0;
	}

	void Reset(bool bCallUpdate) override
	{
		CurrentTicks = 0;
	}

	void Tick(float DeltaTime) override
	{
#if STATS
		DECLARE_SCOPE_CYCLE_COUNTER(TEXT("DelayTicks - Tick"), STAT_ECFDETAILS_DELAYTICKS, STATGROUP_ECFDETAILS);
#endif

#if ECF_INSIGHT_PROFILING
		TRACE_CPUPROFILER_EVENT_SCOPE("ECF - DelayTicks Tick");
#endif

		CurrentTicks++;
		if (CurrentTicks > DelayTicks)
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