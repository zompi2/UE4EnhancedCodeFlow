// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#include "ECFWaitAndExecuteBP.h"
#include "EnhancedCodeFlow.h"

UECFWaitAndExecuteBP* UECFWaitAndExecuteBP::ECFWaitAndExecute(UObject* WorldContextObject, const FOnECFWaitAndExecuteBPFinished& OnFinished, FECFActionSettings Settings)
{
	UECFWaitAndExecuteBP* Proxy = NewObject<UECFWaitAndExecuteBP>();

	Proxy->Proxy_WorldContextObject = WorldContextObject;
	Proxy->Proxy_OnFinished = OnFinished;
	Proxy->Proxy_Settings = Settings;
	Proxy->Proxy_HasFinished = false;

	return Proxy;
}

void UECFWaitAndExecuteBP::Activate()
{
	Proxy_Handle = FFlow::WaitAndExecute(Proxy_WorldContextObject, 
	[this]()
	{
		OnCheck.Broadcast(this);
		return Proxy_HasFinished;
	},
	[this]()
	{
		Proxy_OnFinished.Execute();
	}, Proxy_Settings);
}

void UECFWaitAndExecuteBP::Predicate(bool bHasFinished)
{
	Proxy_HasFinished = bHasFinished;
}

FECFHandleBP UECFWaitAndExecuteBP::GetHandle()
{
	return FECFHandleBP(Proxy_Handle);
}