// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFActionBase.h"
#include "ECFTimeLock.generated.h"

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
		CurrentTime += DeltaTime;
		if (CurrentTime >= LockTime)
		{
			MarkAsFinished();
		}
	}
};
