// Copyright (c) 2025 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFCoroutine.h"
#include "ECFSubsystem.h"
#include "ECFTypes.h"
#include "Async/StreamableManager.h"

class ENHANCEDCODEFLOW_API FECFCoroutineAwaiter
{
public:

	// Returns the state of the corotuine after it's resumed.
	bool await_resume()
	{
		return CoroHandle.promise().bStopped;
	}

	// Required by the co-routine machinery, but we always want to suspend when co-routine is awaiting, so it just returns false.
	bool await_ready() { return false; }

protected:

	// Helper function for adding coroutine actions to the ECF subsystem.
	template<typename T, typename ... Ts>
	void AddCoroutineAction(const UObject* InOwner, FECFCoroutineHandle InCoroutineHandle, const FECFActionSettings& InSettings, Ts&& ... Args)
	{
		CoroHandle = InCoroutineHandle;
		if (UECFSubsystem* ECF = UECFSubsystem::Get(InOwner))
		{
			ECF->AddCoroutineAction<T>(InOwner, InCoroutineHandle, InSettings, Forward<Ts>(Args)...);
		}
	}
	
	// Storing the actual coroutine handle.
	FECFCoroutineHandle CoroHandle;

	// Storing owner to pass it to the ECF subsystem later.
	const UObject* Owner;

	// Storing settings to pass them to the ECF subsystem later.
	FECFActionSettings Settings;
};

struct FECFCoroutineAwaiter_ResultWithTimeout
{
	bool bStopped = false;
	bool bTimedOut = false;
	FECFCoroutineAwaiter_ResultWithTimeout(bool InStopped, bool InTimedOut) :
		bStopped(InStopped),
		bTimedOut(InTimedOut)
	{
	}
};

/*^^^ Wait Seconds Coroutine Awaiter ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

class ENHANCEDCODEFLOW_API FECFCoroutineAwaiter_WaitSeconds : public FECFCoroutineAwaiter
{
public:

	// C-tor
	FECFCoroutineAwaiter_WaitSeconds(const UObject* InOwner, const FECFActionSettings& InSettings, float InTime);
	
	// Called when the suspension begins
	void await_suspend(FECFCoroutineHandle InCoroHandle);
	
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
	void await_suspend(FECFCoroutineHandle InCoroHandle);

private:

	// Storing values in order to use them when await_suspend is called
	int32 Ticks = 0;
};

/*^^^ Wait Until Coroutine Awaiter ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

enum class EECFWaitUntilPredicateType : uint8
{
	HasFinished,
	HasFinished_Deltatime
};

class ENHANCEDCODEFLOW_API FECFCoroutineAwaiter_WaitUntil : public FECFCoroutineAwaiter
{
public:

	// C-tor
	FECFCoroutineAwaiter_WaitUntil(const UObject* InOwner, const FECFActionSettings& InSettings, TUniqueFunction<bool()>&& InPredicate, float InTimeOut);
	FECFCoroutineAwaiter_WaitUntil(const UObject* InOwner, const FECFActionSettings& InSettings, TUniqueFunction<bool(float)>&& InPredicate, float InTimeOut);
	
	// Called when the suspension begins
	void await_suspend(FECFCoroutineHandle InCoroHandle);

	// Returns the state of the corotuine after it's resumed.
	FECFCoroutineAwaiter_ResultWithTimeout await_resume()
	{
		return FECFCoroutineAwaiter_ResultWithTimeout(
			CoroHandle.promise().bStopped,
			CoroHandle.promise().bTimedOut);
	}

private:

	// Storing values in order to use them when await_suspend is called
	EECFWaitUntilPredicateType PredicateType = EECFWaitUntilPredicateType::HasFinished;
	TUniqueFunction<bool()> PredicateHasFinished;
	TUniqueFunction<bool(float)> PredicateHasFinishedDeltaTime;
	float TimeOut = 0.f;
};

/*^^^ Run Async And Wait Coroutine Awaiter ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

class ENHANCEDCODEFLOW_API FECFCoroutineAwaiter_RunAsyncAndWait : public FECFCoroutineAwaiter
{
public:

	// C-tor
	FECFCoroutineAwaiter_RunAsyncAndWait(const UObject* InOwner, const FECFActionSettings& InSettings, TUniqueFunction<void()>&& InAsyncTaskFunc, float InTimeOut, EECFAsyncPrio InThreadPriority);

	// Called when the suspension begins
	void await_suspend(FECFCoroutineHandle InCoroHandle);

	// Returns the state of the corotuine after it's resumed.
	FECFCoroutineAwaiter_ResultWithTimeout await_resume()
	{
		return FECFCoroutineAwaiter_ResultWithTimeout(
			CoroHandle.promise().bStopped,
			CoroHandle.promise().bTimedOut);
	}

private:

	// Storing values in order to use them when await_suspend is called
	TUniqueFunction<void()> AsyncTaskFunction;
	float TimeOut = 0.f;
	EECFAsyncPrio ThreadPriority = EECFAsyncPrio::Normal;
};

/*^^^ Wait Load Objects Coroutine Awaiter ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

class ENHANCEDCODEFLOW_API FECFCoroutineAwaiter_WaitLoadObjects : public FECFCoroutineAwaiter
{
public:

	// C-tor
	FECFCoroutineAwaiter_WaitLoadObjects(const UObject* InOwner, const FECFActionSettings& InSettings, const TArray<FSoftObjectPath>& InObjectsToLoad);

	// Called when the suspension begins
	void await_suspend(FECFCoroutineHandle InCoroHandle);

private:

	// Storing values in order to use them when await_suspend is called
	TArray<FSoftObjectPath> ObjectsToLoad;
};
