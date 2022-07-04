// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#include "ECFDelayBP.h"
#include "EnhancedCodeFlow.h"

UECFDelayBP* UECFDelayBP::ECFDelay(UObject* WorldContextObject, float DelayTime, FECFActionSettings Settings, FECFHandleBP& Handle)
{
	UECFDelayBP* Proxy = NewObject<UECFDelayBP>();

	Proxy->Proxy_WorldContextObject = WorldContextObject;
	Proxy->Proxy_IsPausedAtStart = Settings.bStartPaused;

	Settings.bStartPaused = true;
	Proxy->Proxy_Handle = FFlow::Delay(WorldContextObject, DelayTime, [Proxy]()
	{
		Proxy->OnComplete.Broadcast();
	}, Settings);
	Handle = FECFHandleBP(Proxy->Proxy_Handle);

	return Proxy;
}

void UECFDelayBP::Activate()
{
	if (Proxy_IsPausedAtStart == false)
	{
		FFlow::ResumeAction(Proxy_WorldContextObject, Proxy_Handle);
	}
}
