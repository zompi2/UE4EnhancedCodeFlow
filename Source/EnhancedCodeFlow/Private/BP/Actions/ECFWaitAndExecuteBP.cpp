// Copyright (c) 2022 Damian Nowakowski. All rights reserved.

#include "ECFWaitAndExecuteBP.h"
#include "EnhancedCodeFlow.h"

UECFWaitAndExecuteBP* UECFWaitAndExecuteBP::ECFWaitAndExecute(UObject* WorldContextObject, FECFActionSettings Settings, FECFHandleBP& Handle)
{
	UECFWaitAndExecuteBP* Proxy = NewObject<UECFWaitAndExecuteBP>();
	Proxy->Init(WorldContextObject, Settings);

	Proxy->Proxy_HasFinished = false;

	Proxy->Proxy_Handle = FFlow::WaitAndExecute(WorldContextObject,
		[Proxy]()
		{
			Proxy->OnWait.Broadcast(Proxy);
			return Proxy->Proxy_HasFinished;
		},
		[Proxy]()
		{
			Proxy->OnExecute.Broadcast(Proxy);
		},
	Settings);
	Handle = FECFHandleBP(Proxy->Proxy_Handle);

	return Proxy;
}

void UECFWaitAndExecuteBP::Predicate(bool bHasFinished)
{
	Proxy_HasFinished = bHasFinished;
}