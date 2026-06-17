// Copyright (c) 2026 Damian Nowakowski. All rights reserved.

#include "Coroutines/ECFCoroutineAwaiters.h"

#include "CodeFlowActions/Coroutines/ECFWaitSeconds.h"
#include "CodeFlowActions/Coroutines/ECFWaitTicks.h"
#include "CodeFlowActions/Coroutines/ECFWaitUntil.h"
#include "CodeFlowActions/Coroutines/ECFWaitForFlag.h"
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

/*^^^ Wait For Flag Coroutine Awaiter ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

FECFCoroutineAwaiter_WaitForFlag::FECFCoroutineAwaiter_WaitForFlag(const UObject* InOwner, const FECFActionSettings& InSettings, bool* bInFlag, float InTimeOut)
{
	Owner = InOwner;
	Settings = InSettings;

	bFlag = bInFlag;
	TimeOut = InTimeOut;
}

void FECFCoroutineAwaiter_WaitForFlag::await_suspend(FECFCoroutineHandle InCoroHandle)
{
	AddCoroutineAction<UECFWaitForFlag>(Owner, InCoroHandle, Settings, bFlag, TimeOut);
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


FECFCoroutineAwaiter_WaitLoadObjects::FECFCoroutineAwaiter_WaitLoadObjects(const UObject* InOwner, const FECFActionSettings& InSettings, const TArray<FPrimaryAssetId>& InPrimaryAssetsToLoad)
{
	Owner = InOwner;
	Settings = InSettings;
	PrimaryAssetsToLoad = InPrimaryAssetsToLoad;
}

void FECFCoroutineAwaiter_WaitLoadObjects::await_suspend(FECFCoroutineHandle InCoroHandle)
{
	if (ObjectsToLoad.Num() > 0)
	{
		AddCoroutineAction<UECFWaitLoadObjects>(Owner, InCoroHandle, Settings, ObjectsToLoad);
	}
	else if (PrimaryAssetsToLoad.Num() > 0)
	{
		AddCoroutineAction<UECFWaitLoadObjects>(Owner, InCoroHandle, Settings, PrimaryAssetsToLoad);
	}
	else
	{
#if ECF_LOGS
		UE_LOG(LogECF, Error, TEXT("ECF Coroutine [%s] - wait load objects failed to start. Objects or Assets Id array is empty."), *Settings.Label);
#endif
		// If no objects or primary assets to load, we can immediately resume the coroutine, so it won't stuck in a suspended state.
		InCoroHandle.resume();
	}
}

ECF_PRAGMA_ENABLE_OPTIMIZATION
