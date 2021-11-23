// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFActionBase.h"
#include "ECFDoNTimes.generated.h"

UCLASS()
class ENHANCEDCODEFLOW_API UECFDoNTimes : public UECFActionBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

protected:

	TUniqueFunction<void(int32)> ExecFunc;
	uint32 Times;
	uint32 Counter;

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
