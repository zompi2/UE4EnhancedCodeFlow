// Copyright (c) 2023 Damian Nowakowski. All rights reserved.

#pragma once

#include "BP/ECFBPLibrary.h"
#include "EnhancedCodeFlow.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

/*^^^ ECF Flow Functions ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

void UECFBPLibrary::ECFSetPause(const UObject* WorldContextObject, bool bPaused)
{
	FFlow::SetPause(WorldContextObject, bPaused);
}

void UECFBPLibrary::ECFGetPause(const UObject* WorldContextObject, bool& bIsPaused)
{
	bIsPaused = FFlow::GetPause(WorldContextObject);
}

void UECFBPLibrary::ECFIsActionRunning(bool& bIsRunning, const UObject* WorldContextObject, const FECFHandleBP& Handle)
{
	bIsRunning = FFlow::IsActionRunning(WorldContextObject, Handle.Handle);
}

void UECFBPLibrary::ECFIsActionPaused(bool& bIsRunning, bool& bIsPaused, const UObject* WorldContextObject, const FECFHandleBP& Handle)
{
	bIsRunning = FFlow::IsActionPaused(WorldContextObject, Handle.Handle, bIsPaused);
}

void UECFBPLibrary::ECFPauseAction(const UObject* WorldContextObject, const FECFHandleBP& Handle)
{
	FFlow::PauseAction(WorldContextObject, Handle.Handle);
}

void UECFBPLibrary::ECFResumeAction(const UObject* WorldContextObject, const FECFHandleBP& Handle)
{
	FFlow::ResumeAction(WorldContextObject, Handle.Handle);
}

/*^^^ Stopping ECF Functions ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

void UECFBPLibrary::ECFStopAction(const UObject* WorldContextObject, FECFHandleBP& Handle, bool bComplete/* = false*/)
{
	FFlow::StopAction(WorldContextObject, Handle.Handle, bComplete);
}

void UECFBPLibrary::ECFStopInstancedActions(const UObject* WorldContextObject, FECFInstanceIdBP InstanceId, bool bComplete /*= false*/)
{
	FFlow::StopInstancedAction(WorldContextObject, InstanceId.InstanceId, bComplete);
}

void UECFBPLibrary::ECFStopAllActions(const UObject* WorldContextObject, bool bComplete/* = false*/, UObject* InOwner /*= nullptr*/)
{
	FFlow::StopAllActions(WorldContextObject, bComplete, InOwner);
}

/*^^^ Handle and Instance Id ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

void UECFBPLibrary::IsECFHandleValid(bool& bOutIsValid, const FECFHandleBP& Handle)
{
	bOutIsValid = Handle.Handle.IsValid();
}

void UECFBPLibrary::ECFGetNewInstanceId(FECFInstanceIdBP& OutInstanceId)
{
	OutInstanceId = FECFInstanceIdBP(FECFInstanceId::NewId());
}

void UECFBPLibrary::ECFValidateInstanceId(FECFInstanceIdBP& InInstanceId, FECFInstanceIdBP& OutInstanceId)
{
	if (InInstanceId.InstanceId.IsValid() == false)
	{
		ECFGetNewInstanceId(InInstanceId);
	}
	OutInstanceId = InInstanceId;
}

void UECFBPLibrary::IsECFInstanceIdValid(bool& bIsValid, const FECFInstanceIdBP& InstanceId)
{
	bIsValid = InstanceId.InstanceId.IsValid();
}

/*^^^ Time Lock ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

void UECFBPLibrary::ECFTimeLock(const UObject* WorldContextObject, ETimeLockOutputType& OutExecs, FECFHandleBP& OutHandle, float LockTime, FECFInstanceIdBP& InstanceId, FECFActionSettings Settings)
{
	if (InstanceId.InstanceId.IsValid() == false)
	{
		ECFGetNewInstanceId(InstanceId);
	}
	OutExecs = ETimeLockOutputType::Locked;
	FECFHandle Handle = FFlow::TimeLock(WorldContextObject, LockTime, [&OutExecs]()
	{
		OutExecs = ETimeLockOutputType::Exec;
	}, InstanceId.InstanceId, Settings);
	OutHandle = FECFHandleBP(MoveTemp(Handle));
}

void UECFBPLibrary::ECFRemoveAllTimeLocks(const UObject* WorldContextObject, UObject* InOwner /*= nullptr*/)
{
	FFlow::RemoveAllTimeLocks(WorldContextObject, InOwner);
}

/*^^^ Removing Async Actions ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

void UECFBPLibrary::ECFRemoveAllDelays(const UObject* WorldContextObject, bool bComplete/* = false*/, UObject* InOwner /*= nullptr*/)
{
	FFlow::RemoveAllDelays(WorldContextObject, bComplete, InOwner);
}

void UECFBPLibrary::ECFRemoveAllWaitAndExecutes(const UObject* WorldContextObject, bool bComplete/* = false*/, UObject* InOwner /*= nullptr*/)
{
	FFlow::RemoveAllWaitAndExecutes(WorldContextObject, bComplete, InOwner);
}

void UECFBPLibrary::RemoveAllWhileTrueExecutes(const UObject* WorldContextObject, bool bComplete/* = false*/, UObject* InOwner /*= nullptr*/)
{
	FFlow::RemoveAllWhileTrueExecutes(WorldContextObject, bComplete, InOwner);
}

void UECFBPLibrary::RemoveAllRunAsyncThen(const UObject* WorldContextObject, UObject* InOwner/* = nullptr*/)
{
	FFlow::RemoveAllRunAsyncThen(WorldContextObject, InOwner);
}

void UECFBPLibrary::ECFRemoveAllTickers(const UObject* WorldContextObject, bool bComplete/* = false*/, UObject* InOwner /*= nullptr*/)
{
	FFlow::RemoveAllTickers(WorldContextObject, bComplete, InOwner);
}

void UECFBPLibrary::ECFRemoveAllTimelines(const UObject* WorldContextObject, bool bComplete/* = false*/, UObject* InOwner /*= nullptr*/)
{
	FFlow::RemoveAllTimelines(WorldContextObject, bComplete, InOwner);
}

void UECFBPLibrary::ECFRemoveAllCustomTimelines(const UObject* WorldContextObject, bool bComplete/* = false*/, UObject* InOwner /*= nullptr*/)
{
	FFlow::RemoveAllCustomTimelines(WorldContextObject, bComplete, InOwner);
}

void UECFBPLibrary::ECFRemoveAllDoNoMoreThanXTimes(const UObject* WorldContextObject, UObject* InOwner /*= nullptr*/)
{
	FFlow::RemoveAllDoNoMoreThanXTimes(WorldContextObject, InOwner);
}

/*^^^ Casting ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/	

FString UECFBPLibrary::Conv_ECFHandleToString(const FECFHandleBP& Handle)
{
	return Handle.Handle.ToString();
}

FString UECFBPLibrary::Conv_ECFInstanceIdToString(const FECFInstanceIdBP& InstanceId)
{
	return InstanceId.InstanceId.ToString();
}

/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

ECF_PRAGMA_ENABLE_OPTIMIZATION