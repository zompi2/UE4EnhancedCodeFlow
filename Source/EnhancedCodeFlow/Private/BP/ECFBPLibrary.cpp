// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "BP/ECFBPLibrary.h"
#include "EnhancedCodeFlow.h"

/*^^^ Global ECF Functions ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

void UECFBPLibrary::ECFStopAction(const UObject* WorldContextObject, FECFHandleBP& Handle, bool bComplete/* = false*/)
{
	FFlow::StopAction(WorldContextObject, Handle.Handle, bComplete);
}

void UECFBPLibrary::ECFStopInstancedActions(const UObject* WorldContextObject, FECFInstanceIdBP InstanceId, bool bComplete /*= false*/, UObject* InOwner/* = nullptr*/)
{
	FFlow::StopInstancedAction(WorldContextObject, InstanceId.InstanceId, bComplete, InOwner);
}

void UECFBPLibrary::ECFIsActionRunning(bool& bIsRunning, const UObject* WorldContextObject, const FECFHandleBP& Handle)
{
	bIsRunning = FFlow::IsActionRunning(WorldContextObject, Handle.Handle);
}

void UECFBPLibrary::ECFStopAllActions(const UObject* WorldContextObject, bool bComplete/* = false*/, UObject* InOwner /*= nullptr*/)
{
	FFlow::StopAllActions(WorldContextObject, bComplete, InOwner);
}

/*^^^ Handle and Instance Id ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

FECFInstanceIdBP UECFBPLibrary::ECFGetNewInstanceId(EECFInstanceIdScope Scope)
{
	return FECFInstanceIdBP(FECFInstanceId::NewId(Scope));
}

bool UECFBPLibrary::IsECFHandleValid(const FECFHandleBP& Handle)
{
	return Handle.Handle.IsValid();
}

bool UECFBPLibrary::IsECFInstanceIdValid(const FECFInstanceIdBP& InstanceId)
{
	return InstanceId.InstanceId.IsValid();
}

/*^^^ Ticker ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

void UECFBPLibrary::ECFTicker(FECFHandleBP& OutHandle, UObject* Owner, const FOnECFTick& OnTickEvent, const FOnECFFinished& OnFinishedEvent, FECFActionSettings Settings, float TickingTime /*= -1.f*/)
{
	FECFHandle Handle = FFlow::AddTicker(Owner, TickingTime,
	[OnTickEvent](float DeltaTime)
	{
		if (OnTickEvent.IsBound())
		{
			OnTickEvent.Execute(DeltaTime);
		}
	},
	[OnFinishedEvent]()
	{
		if (OnFinishedEvent.IsBound())
		{
			OnFinishedEvent.Execute();
		}
	}, Settings);
	OutHandle = FECFHandleBP(MoveTemp(Handle));
}

void UECFBPLibrary::ECFRemoveAllTickers(const UObject* WorldContextObject, bool bComplete/* = false*/, UObject* InOwner /*= nullptr*/)
{
	FFlow::RemoveAllTickers(WorldContextObject, bComplete, InOwner);
}

/*^^^ Timeline ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

void UECFBPLibrary::ECFTimeline(FECFHandleBP& OutHandle, UObject* Owner, float StartValue, float StopValue, float Time, const FOnECFTimelineTick& OnTickEvent, const FOnECFTimelineTick& OnFinishedEvent, FECFActionSettings Settings, EECFBlendFunc BlendFunc /*= EECFBlendFunc::ECFBlend_Linear*/, float BlendExp /*= 1.f*/)
{
	FECFHandle Handle = FFlow::AddTimeline(Owner, StartValue, StopValue, Time, 
	[OnTickEvent](float Value, float Time)
	{
		if (OnTickEvent.IsBound())
		{
			OnTickEvent.Execute(Value, Time);
		}
	},
	[OnFinishedEvent](float Value, float Time)
	{
		if (OnFinishedEvent.IsBound())
		{
			OnFinishedEvent.Execute(Value, Time);
		}
	}, BlendFunc, BlendExp, Settings);
	OutHandle = FECFHandleBP(MoveTemp(Handle));
}

void UECFBPLibrary::ECFRemoveAllTimelines(const UObject* WorldContextObject, bool bComplete/* = false*/, UObject* InOwner /*= nullptr*/)
{
	FFlow::RemoveAllTimelines(WorldContextObject, bComplete, InOwner);
}

/*^^^ Custom Timeline ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

void UECFBPLibrary::ECFCustomTimeline(FECFHandleBP& OutHandle, UObject* Owner, UCurveFloat* CurveFloat, const FOnECFTimelineTick& OnTickEvent, const FOnECFTimelineTick& OnFinishedEvent, FECFActionSettings Settings)
{
	FECFHandle Handle = FFlow::AddCustomTimeline(Owner, CurveFloat,
		[OnTickEvent](float Value, float Time)
	{
		if (OnTickEvent.IsBound())
		{
			OnTickEvent.Execute(Value, Time);
		}
	},
		[OnFinishedEvent](float Value, float Time)
	{
		if (OnFinishedEvent.IsBound())
		{
			OnFinishedEvent.Execute(Value, Time);
		}
	}, Settings);
	OutHandle = FECFHandleBP(MoveTemp(Handle));
}

void UECFBPLibrary::ECFRemoveAllCustomTimelines(const UObject* WorldContextObject, bool bComplete/* = false*/, UObject* InOwner /*= nullptr*/)
{
	FFlow::RemoveAllCustomTimelines(WorldContextObject, bComplete, InOwner);
}

/*^^^ Time Lock ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

void UECFBPLibrary::ECFTimeLock(ETimeLockOutputType& OutExecs, FECFHandleBP& OutHandle, UObject* Owner, float LockTime, FECFInstanceIdBP InstanceId, FECFActionSettings Settings)
{
	OutExecs = ETimeLockOutputType::Locked;
	FECFHandle Handle = FFlow::TimeLock(Owner, LockTime, [&OutExecs]()
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

/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
