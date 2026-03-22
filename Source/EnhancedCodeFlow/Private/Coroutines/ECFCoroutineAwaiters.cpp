// Copyright (c) 2026 Damian Nowakowski. All rights reserved.

#include "Coroutines/ECFCoroutineAwaiters.h"

#include "CodeFlowActions/Coroutines/ECFWaitSeconds.h"
#include "CodeFlowActions/Coroutines/ECFWaitTicks.h"
#include "CodeFlowActions/Coroutines/ECFWaitUntil.h"
#include "CodeFlowActions/Coroutines/ECFRunAsyncAndWait.h"
#include "CodeFlowActions/Coroutines/ECFWaitLoadObjects.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

/*^^^ Wait Seconds Coroutine Awaiter ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

FECFCoroutineAwaiter_WaitSeconds::FECFCoroutineAwaiter_WaitSeconds(const UObject* InOwner, const FECFActionSettings& InSettings, float InTime)
{
	Owner = InOwner;
	Settings = InSettings;
	Time = InTime;
}

void FECFCoroutineAwaiter_WaitSeconds::await_suspend(FECFCoroutineHandle InCoroHandle)
{
	AddCoroutineAction<UECFWaitSeconds>(Owner, InCoroHandle, Settings, Time);
}

/*^^^ Wait Ticks Coroutine Awaiter ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

FECFCoroutineAwaiter_WaitTicks::FECFCoroutineAwaiter_WaitTicks(const UObject* InOwner, const FECFActionSettings& InSettings, int32 InTicks)
{
	Owner = InOwner;
	Settings = InSettings;
	Ticks = InTicks;
}

void FECFCoroutineAwaiter_WaitTicks::await_suspend(FECFCoroutineHandle InCoroHandle)
{
	AddCoroutineAction<UECFWaitTicks>(Owner, InCoroHandle, Settings, Ticks);
}

/*^^^ Wait Until Coroutine Awaiter ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

FECFCoroutineAwaiter_WaitUntil::FECFCoroutineAwaiter_WaitUntil(const UObject* InOwner, const FECFActionSettings& InSettings, TUniqueFunction<bool()>&& InPredicate, float InTimeOut)
{
	Owner = InOwner;
	Settings = InSettings;

	PredicateType = EECFWaitUntilPredicateType::HasFinished;
	PredicateHasFinished = MoveTemp(InPredicate);
	TimeOut = InTimeOut;
}

FECFCoroutineAwaiter_WaitUntil::FECFCoroutineAwaiter_WaitUntil(const UObject* InOwner, const FECFActionSettings& InSettings, TUniqueFunction<bool(float)>&& InPredicate, float InTimeOut)
{
	Owner = InOwner;
	Settings = InSettings;

	PredicateType = EECFWaitUntilPredicateType::HasFinished_Deltatime;
	PredicateHasFinishedDeltaTime = MoveTemp(InPredicate);
	TimeOut = InTimeOut;
}

void FECFCoroutineAwaiter_WaitUntil::await_suspend(FECFCoroutineHandle InCoroHandle)
{
	if (PredicateType == EECFWaitUntilPredicateType::HasFinished)
	{
		 AddCoroutineAction<UECFWaitUntil>(Owner, InCoroHandle, Settings, MoveTemp(PredicateHasFinished), TimeOut);
	}
	else if (PredicateType == EECFWaitUntilPredicateType::HasFinished_Deltatime)
	{
		AddCoroutineAction<UECFWaitUntil>(Owner, InCoroHandle, Settings, MoveTemp(PredicateHasFinishedDeltaTime), TimeOut);
	}
}

/*^^^ Run Async And Wait Coroutine Awaiter ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

FECFCoroutineAwaiter_RunAsyncAndWait::FECFCoroutineAwaiter_RunAsyncAndWait(const UObject* InOwner, const FECFActionSettings& InSettings, TUniqueFunction<void()>&& InAsyncTaskFunc, float InTimeOut, EECFAsyncPrio InThreadPriority)
{
	Owner = InOwner;
	Settings = InSettings;

	AsyncTaskFunction = MoveTemp(InAsyncTaskFunc);
	TimeOut = InTimeOut;
	ThreadPriority = InThreadPriority;
}

void FECFCoroutineAwaiter_RunAsyncAndWait::await_suspend(FECFCoroutineHandle InCoroHandle)
{
	AddCoroutineAction<UECFRunAsyncAndWait>(Owner, InCoroHandle, Settings, MoveTemp(AsyncTaskFunction), TimeOut, ThreadPriority);
}

/*^^^ Wait Load Objects Coroutine Awaiter ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

FECFCoroutineAwaiter_WaitLoadObjects::FECFCoroutineAwaiter_WaitLoadObjects(const UObject* InOwner, const FECFActionSettings& InSettings, const TArray<FSoftObjectPath>& InObjectsToLoad)
{
	Owner = InOwner;
	Settings = InSettings;
	ObjectsToLoad = InObjectsToLoad;
}

void FECFCoroutineAwaiter_WaitLoadObjects::await_suspend(FECFCoroutineHandle InCoroHandle)
{
	AddCoroutineAction<UECFWaitLoadObjects>(Owner, InCoroHandle, Settings, ObjectsToLoad);
}

ECF_PRAGMA_ENABLE_OPTIMIZATION
