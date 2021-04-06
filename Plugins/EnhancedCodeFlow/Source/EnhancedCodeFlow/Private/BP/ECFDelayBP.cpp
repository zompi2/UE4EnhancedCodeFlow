// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#include "ECFDelayBP.h"
#include "EnhancedCodeFlow.h"

UECFDelayBP* UECFDelayBP::ECFDelay(UObject* WorldContextObject, float DelayTime, FECFActionSettings Settings)
{
	UECFDelayBP* Proxy = NewObject<UECFDelayBP>();

	Proxy->Proxy_WorldContextObject = WorldContextObject;
	Proxy->Proxy_DelayTime = DelayTime;
	Proxy->Proxy_Settings = Settings;

	return Proxy;
}

void UECFDelayBP::Activate()
{
	FFlow::Delay(Proxy_WorldContextObject, Proxy_DelayTime, [this]()
	{
		OnComplete.Broadcast();
	}, Proxy_Settings);
}
