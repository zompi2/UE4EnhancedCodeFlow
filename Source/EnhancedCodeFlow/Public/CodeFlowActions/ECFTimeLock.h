// Copyright (c) 2025 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFActionBase.h"
#include "ECFTimeLock.generated.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UCLASS()
class ENHANCEDCODEFLOW_API UECFTimeLock : public UECFActionBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

protected:

	TUniqueFunction<void()> ExecFunc;
	float LockTime = 0.f;
	float CurrentTime = 0.f;

	bool Setup(float InLockTime, TUniqueFunction<void()>&& InExecFunc)
	{
		LockTime = InLockTime;
		ExecFunc = MoveTemp(InExecFunc);

		if (ExecFunc && LockTime > 0)
		{
			SetMaxActionTime(LockTime);
			return true;
		}
		else
		{
#if ECF_LOGS
			UE_LOG(LogECF, Error, TEXT("ECF - [%s] Timelock failed to start. Are you sure the Lock time is greater than 0 and the Exec Function is set properly?"), *Settings.Label);
#endif
			return false;
		}
	}

	void Init() override
	{
		CurrentTime = 0;
		ExecFunc();
	}

	void Reset(bool bCallUpdate) override
	{
		CurrentTime = 0.f;
	}

	void Tick(float DeltaTime) override
	{
#if STATS
		DECLARE_SCOPE_CYCLE_COUNTER(TEXT("TimeLock - Tick"), STAT_ECFDETAILS_TIMELOCK, STATGROUP_ECFDETAILS);
#endif

#if ECF_INSIGHT_PROFILING
		TRACE_CPUPROFILER_EVENT_SCOPE("ECF - TimeLock Tick");
#endif

		CurrentTime += DeltaTime;
		if (CurrentTime >= LockTime)
		{
			MarkAsFinished();
		}
	}
};

ECF_PRAGMA_ENABLE_OPTIMIZATION