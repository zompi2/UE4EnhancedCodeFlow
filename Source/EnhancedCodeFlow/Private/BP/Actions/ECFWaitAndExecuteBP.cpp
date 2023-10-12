// Copyright (c) 2023 Damian Nowakowski. All rights reserved.

#include "ECFWaitAndExecuteBP.h"
#include "EnhancedCodeFlow.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UECFWaitAndExecuteBP* UECFWaitAndExecuteBP::ECFWaitAndExecute(const UObject* WorldContextObject, float InTimeOut, FECFActionSettings Settings, FECFHandleBP& Handle)
{
	UECFWaitAndExecuteBP* Proxy = NewObject<UECFWaitAndExecuteBP>();
	Proxy->Init(WorldContextObject, Settings);

	Proxy->Proxy_HasFinished = false;

	Proxy->Proxy_Handle = FFlow::WaitAndExecute(WorldContextObject,
		[Proxy](float DeltaTime)
		{
			Proxy->OnWait.Broadcast(Proxy, DeltaTime, false, false);
			return Proxy->Proxy_HasFinished;
		},
		[Proxy](bool bTimedOut, bool bStopped)
		{
			Proxy->OnExecute.Broadcast(Proxy, 0.f, bTimedOut, bStopped);
			Proxy->ClearAsyncBPAction();
		},
	InTimeOut, Settings);
	Handle = FECFHandleBP(Proxy->Proxy_Handle);

	return Proxy;
}

void UECFWaitAndExecuteBP::Predicate(bool bHasFinished)
{
	Proxy_HasFinished = bHasFinished;
}

ECF_PRAGMA_ENABLE_OPTIMIZATION