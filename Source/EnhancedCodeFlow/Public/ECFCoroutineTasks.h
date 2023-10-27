// Copyright (c) 2023 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFCoroutine.h"
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

	const UObject* Owner;
	FECFActionSettings Settings;
};

class ENHANCEDCODEFLOW_API FECFCoroutineTask_WaitSeconds : public FECFCoroutineTask
{
public:

	FECFCoroutineTask_WaitSeconds(const UObject* InOwner, const FECFActionSettings& InSettings, float InTime);
	void await_suspend(FECFCoroutineHandle CoroHandle);
	
private:

	float Time = 0.f;
};

class ENHANCEDCODEFLOW_API FECFCoroutineTask_WaitTicks : public FECFCoroutineTask
{
public:

	FECFCoroutineTask_WaitTicks(const UObject* InOwner, const FECFActionSettings& InSettings, int32 InTicks);
	void await_suspend(FECFCoroutineHandle CoroHandle);

private:

	int32 Ticks = 0;
};

class ENHANCEDCODEFLOW_API FECFCoroutineTask_WaitUntil : public FECFCoroutineTask
{
public:

	FECFCoroutineTask_WaitUntil(const UObject* InOwner, const FECFActionSettings& InSettings, TUniqueFunction<bool(float)>&& InPredicate, float InTimeOut);
	void await_suspend(FECFCoroutineHandle CoroHandle);

private:

	TUniqueFunction<bool(float)> Predicate;
	float TimeOut = 0.f;
};