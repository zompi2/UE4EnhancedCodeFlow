// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "BP/ECFBPLibrary.h"
#include "EnhancedCodeFlow.h"

/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

void UECFBPLibrary::ECFStopAction(const UObject* WorldContextObject, FECFHandleBP& Handle)
{
	FFlow::StopAction(WorldContextObject, Handle.Handle);
}

bool UECFBPLibrary::ECFIsActionRunning(const UObject* WorldContextObject, const FECFHandleBP& Handle)
{
	return FFlow::IsActionRunning(WorldContextObject, Handle.Handle);	
}

void UECFBPLibrary::ECFStopAllActions(const UObject* WorldContextObject, UObject* InOwner /*= nullptr*/)
{
	FFlow::StopAllActions(WorldContextObject, InOwner);
}

/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

FECFHandleBP UECFBPLibrary::ECFTicker(UObject* Owner, FOnECFTick OnTickEvent, FECFActionSettings Settings, float TickingTime/* = -1.f*/)
{
	FECFHandle Handle = FFlow::AddTicker(Owner, TickingTime,
	[OnTickEvent](float DeltaTime)
	{
		if (OnTickEvent.IsBound())
		{
			OnTickEvent.Execute(DeltaTime);
		}
	},
	nullptr, Settings);
	return FECFHandleBP(Handle);
}

FECFHandleBP UECFBPLibrary::ECFTickerWithFinishCallback(UObject* Owner, FOnECFTick OnTickEvent, FOnECFTickerFinished OnTickerFinishedEvent, FECFActionSettings Settings, float TickingTime /*= -1.f*/)
{
	FECFHandle Handle = FFlow::AddTicker(Owner, TickingTime,
		[OnTickEvent](float DeltaTime)
	{
		if (OnTickEvent.IsBound())
		{
			OnTickEvent.Execute(DeltaTime);
		}
	},
		[OnTickerFinishedEvent]()
	{
		if (OnTickerFinishedEvent.IsBound())
		{
			OnTickerFinishedEvent.Execute();
		}
	}, Settings);
	return FECFHandleBP(Handle);
}

/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/