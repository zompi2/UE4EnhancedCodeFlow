// Copyright (c) 2022 Damian Nowakowski. All rights reserved.

#include "ECFTickerBP.h"
#include "EnhancedCodeFlow.h"

UECFTickerBP* UECFTickerBP::ECFTicker(UObject* WorldContextObject, float TickingTime, FECFActionSettings Settings, FECFHandleBP& Handle)
{
	UECFTickerBP* Proxy = NewObject<UECFTickerBP>();
	Proxy->Init(WorldContextObject, Settings);

	Proxy->Proxy_Handle = FFlow::AddTicker(WorldContextObject, TickingTime, 
		[Proxy](float DeltaTime)
		{
			Proxy->OnTick.Broadcast(DeltaTime);
		}, 
		[Proxy]()
		{
			Proxy->OnComplete.Broadcast(0.f);
		}, 
	Settings);
	Handle = FECFHandleBP(Proxy->Proxy_Handle);

	return Proxy;
}
