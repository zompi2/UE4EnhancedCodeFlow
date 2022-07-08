// Copyright (c) 2022 Damian Nowakowski. All rights reserved.

#include "ECFWhileTrueExecuteBP.h"
#include "EnhancedCodeFlow.h"

UECFWhileTrueExecuteBP* UECFWhileTrueExecuteBP::ECFWhileTrueExecute(UObject* WorldContextObject, FECFActionSettings Settings, FECFHandleBP& Handle)
{
	UECFWhileTrueExecuteBP* Proxy = NewObject<UECFWhileTrueExecuteBP>();
	Proxy->Init(WorldContextObject, Settings);

	Proxy->Proxy_IsTrue = true;

	Proxy->Proxy_Handle = FFlow::WhileTrueExecute(Proxy,
		[Proxy]()
		{
			Proxy->OnWhile.Broadcast(Proxy, 0.f);
			return Proxy->Proxy_IsTrue;
		},
		[Proxy](float DeltaTime)
		{
			Proxy->OnExecute.Broadcast(Proxy, DeltaTime);
		}, 
	Settings);
	Handle = FECFHandleBP(Proxy->Proxy_Handle);

	return Proxy;
}

void UECFWhileTrueExecuteBP::Predicate(bool bIsTrue)
{
	Proxy_IsTrue = bIsTrue;
}
