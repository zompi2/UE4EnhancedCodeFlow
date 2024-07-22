// Copyright (c) 2024 Damian Nowakowski. All rights reserved.

#include "Coroutines/ECFCoroutineAwaiters.h"

#include "CodeFlowActions/Coroutines/ECFWaitSeconds.h"
#include "CodeFlowActions/Coroutines/ECFWaitTicks.h"
#include "CodeFlowActions/Coroutines/ECFWaitUntil.h"
#include "CodeFlowActions/Coroutines/ECFRunAsyncAndWait.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

/*^^^ Wait Seconds Coroutine Awaiter ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

FECFCoroutineAwaiter_WaitSeconds::FECFCoroutineAwaiter_WaitSeconds(const UObject* InOwner, const FECFActionSettings& InSettings, float InTime)
{
	Owner = InOwner;
	Settings = InSettings;
	Time = InTime;
}

void FECFCoroutineAwaiter_WaitSeconds::await_suspend(FECFCoroutineHandle CoroHandle)
{
	AddCoroutineAction<UECFWaitSeconds>(Owner, CoroHandle, Settings, Time);
}

/*^^^ Wait Ticks Coroutine Awaiter ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

FECFCoroutineAwaiter_WaitTicks::FECFCoroutineAwaiter_WaitTicks(const UObject* InOwner, const FECFActionSettings& InSettings, int32 InTicks)
{
	Owner = InOwner;
	Settings = InSettings;
	Ticks = InTicks;
}

void FECFCoroutineAwaiter_WaitTicks::await_suspend(FECFCoroutineHandle CoroHandle)
{
	AddCoroutineAction<UECFWaitTicks>(Owner, CoroHandle, Settings, Ticks);
}

/*^^^ Wait Until Coroutine Awaiter ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

FECFCoroutineAwaiter_WaitUntil::FECFCoroutineAwaiter_WaitUntil(const UObject* InOwner, const FECFActionSettings& InSettings, TUniqueFunction<bool(float)>&& InPredicate, float InTimeOut)
{
	Owner = InOwner;
	Settings = InSettings;
	Predicate = MoveTemp(InPredicate);
	TimeOut = InTimeOut;
}

void FECFCoroutineAwaiter_WaitUntil::await_suspend(FECFCoroutineHandle CoroHandle)
{
	AddCoroutineAction<UECFWaitUntil>(Owner, CoroHandle, Settings, MoveTemp(Predicate), TimeOut);
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

void FECFCoroutineAwaiter_RunAsyncAndWait::await_suspend(FECFCoroutineHandle CoroHandle)
{
	AddCoroutineAction<UECFRunAsyncAndWait>(Owner, CoroHandle, Settings, MoveTemp(AsyncTaskFunction), TimeOut, ThreadPriority);
}

ECF_PRAGMA_ENABLE_OPTIMIZATION
