// Copyright (c) 2023 Damian Nowakowski. All rights reserved.

#include "ECFCoroutinesTypes.h"

#include "CodeFlowActions/Coroutines/ECFWaitSeconds.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

FECFCoroutineTask_WaitSeconds::FECFCoroutineTask_WaitSeconds(UObject* InOwner, const FECFActionSettings& InSettings, float InTime)
{
	Owner = InOwner;
	Settings = InSettings;
	Time = InTime;
}

void FECFCoroutineTask_WaitSeconds::await_suspend(FECFCoroutineHandle CoroHandle)
{
	AddCoroutineAction<UECFWaitSeconds>(Owner, CoroHandle, Settings, Time);
}

ECF_PRAGMA_ENABLE_OPTIMIZATION