// Copyright (c) 2023 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFActionBase.h"
#include "ECFDoNTimes.generated.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UCLASS()
class ENHANCEDCODEFLOW_API UECFDoNTimes : public UECFActionBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

protected:

	TUniqueFunction<void(int32)> ExecFunc;
	uint32 Times = 0;
	uint32 Counter = 0;

	bool Setup(uint32 InTimes, TUniqueFunction<void(int32)>&& InExecFunc)
	{
		Times = InTimes;
		ExecFunc = MoveTemp(InExecFunc);

		if (ExecFunc && Times > 0)
		{
			return true;
		}
		else
		{
			ensureMsgf(false, TEXT("ECF - DoNTimes failed to start. Are you sure Exec Fuinction and Times number are set properly?"));
			return false;
		}
	}

	void Init() override
	{
		Counter = 0;
		RetriggeredInstancedAction();
	}

	void RetriggeredInstancedAction() override
	{
		Counter++;
		if (Counter <= Times)
		{
			ExecFunc(Counter);
		}
	}
};

ECF_PRAGMA_ENABLE_OPTIMIZATION