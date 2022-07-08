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

		if (ExecFunc && LockTime >= 0)
		{
			return true;
		}
		else
		{
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
