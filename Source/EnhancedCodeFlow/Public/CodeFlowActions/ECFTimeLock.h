// Copyright (c) 2023 Damian Nowakowski. All rights reserved.

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
	float LockTime;
	float CurrentTime;

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
			ensureMsgf(false, TEXT("ECF - Timelock failed to start. Are you sure the Lock time is greater than 0 and the Exec Function is set properly?"));
			return false;
		}
	}

	void Init() override
	{
		CurrentTime = 0;
		ExecFunc();
	}

	void Tick(float DeltaTime) override
	{
#if STATS
		DECLARE_SCOPE_CYCLE_COUNTER(TEXT("TimeLock - Tick"), STAT_ECFDETAILS_TIMELOCK, STATGROUP_ECFDETAILS);
#endif
		CurrentTime += DeltaTime;
		if (CurrentTime >= LockTime)
		{
			MarkAsFinished();
		}
	}
};

ECF_PRAGMA_ENABLE_OPTIMIZATION