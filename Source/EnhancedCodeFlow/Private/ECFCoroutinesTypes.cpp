// Copyright (c) 2023 Damian Nowakowski. All rights reserved.

#include "ECFCoroutinesTypes.h"

#include "CodeFlowActions/Coroutines/ECFWaitSeconds.h"
#include "CodeFlowActions/Coroutines/ECFWaitTicks.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

FECFCoroutineTask_WaitSeconds::FECFCoroutineTask_WaitSeconds(const UObject* InOwner, const FECFActionSettings& InSettings, float InTime)
{
	Owner = InOwner;
	Settings = InSettings;
	Time = InTime;
}

void FECFCoroutineTask_WaitSeconds::await_suspend(FECFCoroutineHandle CoroHandle)
{
	AddCoroutineAction<UECFWaitSeconds>(Owner, CoroHandle, Settings, Time);
}

FECFCoroutineTask_WaitTicks::FECFCoroutineTask_WaitTicks(const UObject* InOwner, const FECFActionSettings& InSettings, int32 InTicks)
{
	Owner = InOwner;
	Settings = InSettings;
	Ticks = InTicks;
}

void FECFCoroutineTask_WaitTicks::await_suspend(FECFCoroutineHandle CoroHandle)
{
	AddCoroutineAction<UECFWaitTicks>(Owner, CoroHandle, Settings, Ticks);
}

ECF_PRAGMA_ENABLE_OPTIMIZATION