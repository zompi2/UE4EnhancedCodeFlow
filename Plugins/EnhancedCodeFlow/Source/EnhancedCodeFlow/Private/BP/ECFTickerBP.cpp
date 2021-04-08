// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#include "ECFTickerBP.h"
#include "EnhancedCodeFlow.h"

UECFTickerBP* UECFTickerBP::ECFTicker(UObject* WorldContextObject, float TickTime, FECFActionSettings Settings)
{
	UECFTickerBP* Proxy = NewObject<UECFTickerBP>();

	Proxy->Proxy_WorldContextObject = WorldContextObject;
	Proxy->Proxy_TickTime = TickTime;
	Proxy->Proxy_Settings = Settings;

	return Proxy;
}

UECFTickerBP* UECFTickerBP::ECFInfiniteTicker(UObject* WorldContextObject, FECFActionSettings Settings)
{
	return ECFTicker(WorldContextObject, -1.f, Settings);
}

void UECFTickerBP::Activate()
{
	FFlow::AddTicker(Proxy_WorldContextObject, Proxy_TickTime, [this](float DeltaTime, FECFHandle Handle)
	{
		OnTick.Broadcast(DeltaTime);
	}, [this]()
	{
		OnComplete.Broadcast();
	}, Proxy_Settings);
}
