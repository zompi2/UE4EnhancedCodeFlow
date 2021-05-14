// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "BP/ECFBPLibrary.h"
#include "EnhancedCodeFlow.h"

/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

void UECFBPLibrary::ECFStopAction(const UObject* WorldContextObject, FECFHandleBP& Handle, bool bComplete/* = false*/)
{
	FFlow::StopAction(WorldContextObject, Handle.Handle, bComplete);
}

bool UECFBPLibrary::ECFIsActionRunning(const UObject* WorldContextObject, const FECFHandleBP& Handle)
{
	return FFlow::IsActionRunning(WorldContextObject, Handle.Handle);	
}

void UECFBPLibrary::ECFStopAllActions(const UObject* WorldContextObject, bool bComplete/* = false*/, UObject* InOwner /*= nullptr*/)
{
	FFlow::StopAllActions(WorldContextObject, bComplete, InOwner);
}

/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

FECFHandleBP UECFBPLibrary::ECFTicker(UObject* Owner, const FOnECFTick& OnTickEvent, const FOnECFFinished& OnFinishedEvent, FECFActionSettings Settings, float TickingTime /*= -1.f*/)
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
	return FECFHandleBP(Handle);
}

void UECFBPLibrary::ECFRemoveAllTickers(const UObject* WorldContextObject, bool bComplete/* = false*/, UObject* InOwner /*= nullptr*/)
{
	FFlow::RemoveAllTickers(WorldContextObject, bComplete, InOwner);
}

/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

FECFHandleBP UECFBPLibrary::ECFTimeline(UObject* Owner, float StartValue, float StopValue, float Time, const FOnECFTimelineTick& OnTickEvent, const FOnECFTimelineTick& OnFinishedEvent, FECFActionSettings Settings, EECFBlendFunc BlendFunc /*= EECFBlendFunc::ECFBlend_Linear*/, float BlendExp /*= 1.f*/)
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
	return FECFHandleBP(Handle);
}

void UECFBPLibrary::ECFRemoveAllTimelines(const UObject* WorldContextObject, bool bComplete/* = false*/, UObject* InOwner /*= nullptr*/)
{
	FFlow::RemoveAllTimelines(WorldContextObject, bComplete, InOwner);
}

/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

FECFHandleBP UECFBPLibrary::ECFCustomTimeline(UObject* Owner, UCurveFloat* CurveFloat, const FOnECFTimelineTick& OnTickEvent, const FOnECFTimelineTick& OnFinishedEvent, FECFActionSettings Settings)
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
	return FECFHandleBP(Handle);
}

void UECFBPLibrary::ECFRemoveAllCustomTimelines(const UObject* WorldContextObject, bool bComplete/* = false*/, UObject* InOwner /*= nullptr*/)
{
	FFlow::RemoveAllCustomTimelines(WorldContextObject, bComplete, InOwner);
}

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
