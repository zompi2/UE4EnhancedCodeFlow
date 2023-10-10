// Copyright (c) 2023 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFCoro.h"
#include "ECFSubsystem.h"

class ENHANCEDCODEFLOW_API FECFCoroutineTask
{
protected:

	template<typename T, typename ... Ts>
	void AddCoroutineAction(const UObject* InOwner, FECFCoroutineHandle InCoroutineHandle, const FECFActionSettings& Settings, Ts&& ... Args)
	{
		if (UECFSubsystem* ECF = UECFSubsystem::Get(InOwner))
		{
			ECF->AddCoroutineAction<T>(InOwner, InCoroutineHandle, Settings, Forward<Ts>(Args)...);
		}
	}
};

class ENHANCEDCODEFLOW_API FECFCoroutineWaitTask : public FECFCoroutineTask
{
public:
	FECFCoroutineWaitTask(class UObject* InOwner, const FECFActionSettings& InSettings, float InTime);

	void await_suspend(FECFCoroutineHandle CoroHandle);
	bool await_ready() { return false; }
	void await_resume() {}

private:
	class UObject* Owner;
	FECFActionSettings Settings;
	float Time;
};