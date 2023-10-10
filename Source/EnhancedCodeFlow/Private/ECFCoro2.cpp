// Copyright (c) 2023 Damian Nowakowski. All rights reserved.

#include "ECFCoro2.h"
#include "ECFSubsystem.h"
#include "CodeFlowActions/ECFDelayCoro.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

FECFCoroutineWaitTask::FECFCoroutineWaitTask(UObject* InOwner, const FECFActionSettings& InSettings, float InTime)
{
	Owner = InOwner;
	Settings = InSettings;
	Time = InTime;
}

void FECFCoroutineWaitTask::await_suspend(FECFCoroutineHandle CoroHandle)
{
	AddCoroutineAction<UECFDelayCoro>(Owner, CoroHandle, Settings, Time);
}

ECF_PRAGMA_ENABLE_OPTIMIZATION