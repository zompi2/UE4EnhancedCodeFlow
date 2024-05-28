// Copyright (c) 2024 Damian Nowakowski. All rights reserved.

#include "ECFTickerBP.h"
#include "EnhancedCodeFlow.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UECFTickerBP* UECFTickerBP::ECFTicker(const UObject* WorldContextObject, float TickingTime, FECFActionSettings Settings, FECFHandleBP& Handle)
{
	UECFTickerBP* Proxy = NewObject<UECFTickerBP>();
	if (Proxy)
	{
		Proxy->Init(WorldContextObject, Settings);
		Proxy->Proxy_Handle = FFlow::AddTicker(WorldContextObject, TickingTime,
			[Proxy](float DeltaTime)
			{
				if (IsProxyValid(Proxy))
				{
					Proxy->OnTick.Broadcast(DeltaTime, false);
				}
			}, 
			[Proxy](bool bStopped)
			{
				if (IsProxyValid(Proxy))
				{
					Proxy->OnComplete.Broadcast(0.f, bStopped);
					Proxy->ClearAsyncBPAction();
				}
			}, 
		Settings);
		Handle = FECFHandleBP(Proxy->Proxy_Handle);
	}

	return Proxy;
}

ECF_PRAGMA_ENABLE_OPTIMIZATION