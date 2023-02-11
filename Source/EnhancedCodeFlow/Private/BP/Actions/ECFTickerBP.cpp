// Copyright (c) 2023 Damian Nowakowski. All rights reserved.

#include "ECFTickerBP.h"
#include "EnhancedCodeFlow.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UECFTickerBP* UECFTickerBP::ECFTicker(const UObject* WorldContextObject, float TickingTime, FECFActionSettings Settings, FECFHandleBP& Handle)
{
	UECFTickerBP* Proxy = NewObject<UECFTickerBP>();
	Proxy->Init(WorldContextObject, Settings);

	Proxy->Proxy_Handle = FFlow::AddTicker(WorldContextObject, TickingTime,
		[Proxy](float DeltaTime)
		{
			Proxy->OnTick.Broadcast(DeltaTime, false);
		}, 
		[Proxy](bool bStopped)
		{
			Proxy->OnComplete.Broadcast(0.f, bStopped);
		}, 
	Settings);
	Handle = FECFHandleBP(Proxy->Proxy_Handle);

	return Proxy;
}

ECF_PRAGMA_ENABLE_OPTIMIZATION