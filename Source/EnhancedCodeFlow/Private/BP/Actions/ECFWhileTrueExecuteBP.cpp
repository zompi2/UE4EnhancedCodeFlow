// Copyright (c) 2023 Damian Nowakowski. All rights reserved.

#include "ECFWhileTrueExecuteBP.h"
#include "EnhancedCodeFlow.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UECFWhileTrueExecuteBP* UECFWhileTrueExecuteBP::ECFWhileTrueExecute(const UObject* WorldContextObject, float TimeOut, FECFActionSettings Settings, FECFHandleBP& Handle)
{
	UECFWhileTrueExecuteBP* Proxy = NewObject<UECFWhileTrueExecuteBP>();
	Proxy->Init(WorldContextObject, Settings);

	Proxy->Proxy_IsTrue = true;

	Proxy->Proxy_Handle = FFlow::WhileTrueExecute(WorldContextObject,
		[Proxy]()
		{
			Proxy->OnWhile.Broadcast(Proxy, 0.f, false, false);
			return Proxy->Proxy_IsTrue;
		},
		[Proxy](float DeltaTime)
		{
			Proxy->OnExecute.Broadcast(Proxy, DeltaTime, false, false);
		},
		[Proxy](bool bTimedOut, bool bStopped)
		{
			Proxy->OnComplete.Broadcast(Proxy, 0.f, bTimedOut, bStopped);
			Proxy->ClearAsyncBPAction();
		},
	TimeOut, Settings);
	Handle = FECFHandleBP(Proxy->Proxy_Handle);

	return Proxy;
}

void UECFWhileTrueExecuteBP::Predicate(bool bIsTrue)
{
	Proxy_IsTrue = bIsTrue;
}

ECF_PRAGMA_ENABLE_OPTIMIZATION