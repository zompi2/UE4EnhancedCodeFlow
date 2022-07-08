// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFActionBase.h"
#include "ECFDoOnce.generated.h"

UCLASS()
class ENHANCEDCODEFLOW_API UECFDoOnce : public UECFActionBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

protected:

	TUniqueFunction<void()> ExecFunc;

	bool Setup(TUniqueFunction<void()>&& InExecFunc)
	{
		ExecFunc = MoveTemp(InExecFunc);

		if (ExecFunc)
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
		ExecFunc();
	}
};
