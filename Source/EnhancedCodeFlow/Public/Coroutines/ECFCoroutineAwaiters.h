// Copyright (c) 2024 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFCoroutine.h"
#include "ECFSubsystem.h"
#include "ECFTypes.h"

class ENHANCEDCODEFLOW_API FECFCoroutineAwaiter
{
public:

	// Functions required by any coroutine awaiter.
	void await_resume() {}
	bool await_ready() { return false; }

protected:

	// Helper function for adding coroutine actions to the ECF subsystem.
	template<typename T, typename ... Ts>
	void AddCoroutineAction(const UObject* InOwner, FECFCoroutineHandle InCoroutineHandle, const FECFActionSettings& InSettings, Ts&& ... Args)
	{
		if (UECFSubsystem* ECF = UECFSubsystem::Get(InOwner))
		{
			ECF->AddCoroutineAction<T>(InOwner, InCoroutineHandle, InSettings, Forward<Ts>(Args)...);
		}
	}

	// Storing owner to pass it to the ECF subsystem later.
	const UObject* Owner;

	// Storing settings to pass them to the ECF subsystem later.
	FECFActionSettings Settings;
};

/*^^^ Wait Seconds Coroutine Awaiter ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

class ENHANCEDCODEFLOW_API FECFCoroutineAwaiter_WaitSeconds : public FECFCoroutineAwaiter
{
public:

	// C-tor
	FECFCoroutineAwaiter_WaitSeconds(const UObject* InOwner, const FECFActionSettings& InSettings, float InTime);
	
	// Called when the suspension begins
	void await_suspend(FECFCoroutineHandle CoroHandle);
	
private:

	// Storing values in order to use them when await_suspend is called
	float Time = 0.f;
};

/*^^^ Wait Ticks Coroutine Awaiter ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

class ENHANCEDCODEFLOW_API FECFCoroutineAwaiter_WaitTicks : public FECFCoroutineAwaiter
{
public:

	// C-tor
	FECFCoroutineAwaiter_WaitTicks(const UObject* InOwner, const FECFActionSettings& InSettings, int32 InTicks);
	
	// Called when the suspension begins
	void await_suspend(FECFCoroutineHandle CoroHandle);

private:

	// Storing values in order to use them when await_suspend is called
	int32 Ticks = 0;
};

/*^^^ Wait Until Coroutine Awaiter ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

class ENHANCEDCODEFLOW_API FECFCoroutineAwaiter_WaitUntil : public FECFCoroutineAwaiter
{
public:

	// C-tor
	FECFCoroutineAwaiter_WaitUntil(const UObject* InOwner, const FECFActionSettings& InSettings, TUniqueFunction<bool(float)>&& InPredicate, float InTimeOut);
	
	// Called when the suspension begins
	void await_suspend(FECFCoroutineHandle CoroHandle);

private:

	// Storing values in order to use them when await_suspend is called
	TUniqueFunction<bool(float)> Predicate;
	float TimeOut = 0.f;
};

/*^^^ Run Async And Wait Coroutine Awaiter ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

class ENHANCEDCODEFLOW_API FECFCoroutineAwaiter_RunAsyncAndWait : public FECFCoroutineAwaiter
{
public:

	// C-tor
	FECFCoroutineAwaiter_RunAsyncAndWait(const UObject* InOwner, const FECFActionSettings& InSettings, TUniqueFunction<void()>&& InAsyncTaskFunc, float InTimeOut, EECFAsyncPrio InThreadPriority);

	// Called when the suspension begins
	void await_suspend(FECFCoroutineHandle CoroHandle);

private:

	// Storing values in order to use them when await_suspend is called
	TUniqueFunction<void()> AsyncTaskFunction;
	float TimeOut = 0.f;
	EECFAsyncPrio ThreadPriority = EECFAsyncPrio::Normal;
};

