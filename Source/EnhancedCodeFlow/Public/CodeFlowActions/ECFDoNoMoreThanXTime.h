// Copyright (c) 2022 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFActionBase.h"
#include "ECFDoNoMoreThanXTime.generated.h"

UCLASS()
class ENHANCEDCODEFLOW_API UECFDoNoMoreThanXTime : public UECFActionBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

protected:

	TUniqueFunction<void()> ExecFunc;
	float LockTime;
	float CurrentTime;
	int32 ExecsEnqueued;
	int32 MaxExecsEnqueued;


	bool Setup(TUniqueFunction<void()>&& InExecFunc, float InLockTime, int32 InMaxExecsEnqueued)
	{
		ExecFunc = MoveTemp(InExecFunc);
		LockTime = InLockTime;
		MaxExecsEnqueued = InMaxExecsEnqueued;

		if (ExecFunc && LockTime > 0 && InMaxExecsEnqueued > 0)
		{
			return true;
		}
		else
		{
			ensureMsgf(false, TEXT("ECF - Do No More Than Times failed to start. Are you sure the Lock time and Max Execs Eneueud are greater than 0 and the Exec Function is set properly?"));
			return false;
		}
	}

	void Init() override
	{
		CurrentTime = 0;
		ExecsEnqueued = 0;
		ExecFunc();
	}

	void RetriggeredInstancedAction() override
	{
		if (CurrentTime < LockTime)
		{
			if (ExecsEnqueued < MaxExecsEnqueued)
			{
				ExecsEnqueued++;
			}
		}
		else
		{
			CurrentTime = 0;
			ExecFunc();
		}
	}

	void Tick(float DeltaTime) override
	{
		if (CurrentTime < LockTime)
		{
			CurrentTime += DeltaTime;
		}
		
		if ((ExecsEnqueued > 0) && (CurrentTime >= LockTime))
		{
			CurrentTime = 0;
			ExecsEnqueued--;
			ExecFunc();
		}
	}
};
