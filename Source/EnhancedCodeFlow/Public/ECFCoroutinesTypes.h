// Copyright (c) 2023 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFCoroutines.h"
#include "ECFSubsystem.h"

class ENHANCEDCODEFLOW_API FECFCoroutineTask
{
public:

	void await_resume() {}
	bool await_ready() { return false; }

protected:

	template<typename T, typename ... Ts>
	void AddCoroutineAction(const UObject* InOwner, FECFCoroutineHandle InCoroutineHandle, const FECFActionSettings& Settings, Ts&& ... Args)
	{
		if (UECFSubsystem* ECF = UECFSubsystem::Get(InOwner))
		{
			ECF->AddCoroutineAction<T>(InOwner, InCoroutineHandle, Settings, Forward<Ts>(Args)...);
		}
	}

	class UObject* Owner;
	FECFActionSettings Settings;
};

class ENHANCEDCODEFLOW_API FECFCoroutineTask_WaitSeconds : public FECFCoroutineTask
{
public:

	FECFCoroutineTask_WaitSeconds(class UObject* InOwner, const FECFActionSettings& InSettings, float InTime);


	void await_suspend(FECFCoroutineHandle CoroHandle);
	
private:

	float Time;
};