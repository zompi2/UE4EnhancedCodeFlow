// Copyright (c) 2025 Damian Nowakowski. All rights reserved.

#pragma once

#include "BP/ECFBPLibrary.h"
#include "EnhancedCodeFlow.h"
#include "ECFActionsHeader.h"

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

TArray<FECFHandleBP> UECFBPLibrary::GetActionsHandlesByClass(const UObject* WorldContextObject, TSubclassOf<UECFActionBase> Class)
{
	TArray<FECFHandle> Result = FFlow::GetActionsHandlesByClass(WorldContextObject, Class);
	TArray<FECFHandleBP> ResultBP;
	ResultBP.Reserve(Result.Num());
	for (const FECFHandle& Handle : Result)
	{
		ResultBP.Add(FECFHandleBP(Handle));
	}
	return ResultBP;
}

TArray<FECFHandleBP> UECFBPLibrary::GetActionsHandlesByLabel(const UObject* WorldContextObject, const FString& Label)
{
	TArray<FECFHandle> Result = FFlow::GetActionsHandlesByLabel(WorldContextObject, Label);
	TArray<FECFHandleBP> ResultBP;
	ResultBP.Reserve(Result.Num());
	for (const FECFHandle& Handle : Result)
	{
		ResultBP.Add(FECFHandleBP(Handle));
	}
	return ResultBP;
}

void UECFBPLibrary::ECFPauseAction(const UObject* WorldContextObject, const FECFHandleBP& Handle)
{
	FFlow::PauseAction(WorldContextObject, Handle.Handle);
}

void UECFBPLibrary::ECFResumeAction(const UObject* WorldContextObject, const FECFHandleBP& Handle)
{
	FFlow::ResumeAction(WorldContextObject, Handle.Handle);
}

void UECFBPLibrary::ECFIsActionPaused(bool& bIsRunning, bool& bIsPaused, const UObject* WorldContextObject, const FECFHandleBP& Handle)
{
	bIsRunning = FFlow::IsActionPaused(WorldContextObject, Handle.Handle, bIsPaused);
}

bool UECFBPLibrary::ECFResetAction(const UObject* WorldContextObject, const FECFHandleBP& Handle, bool bCallUpdate)
{
	return FFlow::ResetAction(WorldContextObject, Handle.Handle, bCallUpdate);
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

void UECFBPLibrary::ECFStopAllActionsOfClass(const UObject* WorldContextObject, TSubclassOf<UECFActionBase> Class, bool bComplete/* = false*/, UObject* InOwner /*= nullptr*/)
{
	FFlow::StopAllActionsOfClass(WorldContextObject, Class, bComplete, InOwner);
}

void UECFBPLibrary::ECFStopAllActionsWithLabel(const UObject* WorldContextObject, FString Label, bool bComplete/* = false*/, UObject* InOwner /*= nullptr*/)
{
	FFlow::StopAllActionsWithLabel(WorldContextObject, Label, bComplete, InOwner);
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
	FFlow::StopAllActionsOfClass<UECFTimeLock>(WorldContextObject, false, InOwner);
}

/*^^^ Removing Async Actions ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

void UECFBPLibrary::ECFRemoveAllDelays(const UObject* WorldContextObject, bool bComplete/* = false*/, UObject* InOwner /*= nullptr*/)
{
	FFlow::StopAllActionsOfClass<UECFDelay>(WorldContextObject, bComplete, InOwner);
}

void UECFBPLibrary::ECFRemoveAllWaitAndExecutes(const UObject* WorldContextObject, bool bComplete/* = false*/, UObject* InOwner /*= nullptr*/)
{
	FFlow::StopAllActionsOfClass<UECFWaitAndExecute>(WorldContextObject, bComplete, InOwner);
}

void UECFBPLibrary::RemoveAllWhileTrueExecutes(const UObject* WorldContextObject, bool bComplete/* = false*/, UObject* InOwner /*= nullptr*/)
{
	FFlow::StopAllActionsOfClass<UECFWhileTrueExecute>(WorldContextObject, bComplete, InOwner);
}

void UECFBPLibrary::RemoveAllRunAsyncThen(const UObject* WorldContextObject, UObject* InOwner/* = nullptr*/)
{
	FFlow::StopAllActionsOfClass<UECFRunAsyncThen>(WorldContextObject, false, InOwner);
}

void UECFBPLibrary::ECFRemoveAllTickers(const UObject* WorldContextObject, bool bComplete/* = false*/, UObject* InOwner /*= nullptr*/)
{
	FFlow::StopAllActionsOfClass<UECFTicker>(WorldContextObject, bComplete, InOwner);
}

void UECFBPLibrary::ECFRemoveAllTimelines(const UObject* WorldContextObject, bool bComplete/* = false*/, UObject* InOwner /*= nullptr*/)
{
	FFlow::StopAllActionsOfClass<UECFTimeline>(WorldContextObject, bComplete, InOwner);
}

void UECFBPLibrary::ECFRemoveAllTimelinesVector(const UObject* WorldContextObject, bool bComplete/* = false*/, UObject* InOwner /*= nullptr*/)
{
	FFlow::StopAllActionsOfClass<UECFTimelineVector>(WorldContextObject, bComplete, InOwner);
}

void UECFBPLibrary::ECFRemoveAllTimelinesLinearColor(const UObject* WorldContextObject, bool bComplete/* = false*/, UObject* InOwner /*= nullptr*/)
{
	FFlow::StopAllActionsOfClass<UECFTimelineLinearColor>(WorldContextObject, bComplete, InOwner);
}

void UECFBPLibrary::ECFRemoveAllCustomTimelines(const UObject* WorldContextObject, bool bComplete/* = false*/, UObject* InOwner /*= nullptr*/)
{
	FFlow::StopAllActionsOfClass<UECFCustomTimeline>(WorldContextObject, bComplete, InOwner);
}

void UECFBPLibrary::ECFRemoveAllCustomTimelinesVector(const UObject* WorldContextObject, bool bComplete/* = false*/, UObject* InOwner /*= nullptr*/)
{
	FFlow::StopAllActionsOfClass<UECFCustomTimelineVector>(WorldContextObject, bComplete, InOwner);
}

void UECFBPLibrary::ECFRemoveAllCustomTimelinesLinearColor(const UObject* WorldContextObject, bool bComplete/* = false*/, UObject* InOwner /*= nullptr*/)
{
	FFlow::StopAllActionsOfClass<UECFCustomTimelineLinearColor>(WorldContextObject, bComplete, InOwner);
}

void UECFBPLibrary::ECFRemoveAllDoNoMoreThanXTimes(const UObject* WorldContextObject, UObject* InOwner /*= nullptr*/)
{
	FFlow::StopAllActionsOfClass<UECFDoNoMoreThanXTime>(WorldContextObject, false,  InOwner);
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


ECF_PRAGMA_ENABLE_OPTIMIZATION