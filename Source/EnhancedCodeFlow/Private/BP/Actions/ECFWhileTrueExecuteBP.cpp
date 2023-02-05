// Copyright (c) 2023 Damian Nowakowski. All rights reserved.

#include "ECFWhileTrueExecuteBP.h"
#include "EnhancedCodeFlow.h"

UECFWhileTrueExecuteBP* UECFWhileTrueExecuteBP::ECFWhileTrueExecute(const UObject* WorldContextObject, float TimeOut, FECFActionSettings Settings, FECFHandleBP& Handle)
{
	UECFWhileTrueExecuteBP* Proxy = NewObject<UECFWhileTrueExecuteBP>();
	Proxy->Init(WorldContextObject, Settings);

	Proxy->Proxy_IsTrue = true;

	Proxy->Proxy_Handle = FFlow::WhileTrueExecute(WorldContextObject,
		[Proxy]()
		{
			Proxy->OnWhile.Broadcast(Proxy, 0.f, false);
			return Proxy->Proxy_IsTrue;
		},
		[Proxy](float DeltaTime)
		{
			Proxy->OnExecute.Broadcast(Proxy, DeltaTime, false);
		},
		[Proxy](bool bTimedOut)
		{
			Proxy->OnComplete.Broadcast(Proxy, 0.f, bTimedOut);
		},
	TimeOut, Settings);
	Handle = FECFHandleBP(Proxy->Proxy_Handle);

	return Proxy;
}

void UECFWhileTrueExecuteBP::Predicate(bool bIsTrue)
{
	Proxy_IsTrue = bIsTrue;
}
