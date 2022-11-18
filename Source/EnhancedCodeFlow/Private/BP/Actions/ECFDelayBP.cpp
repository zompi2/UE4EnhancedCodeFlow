// Copyright (c) 2022 Damian Nowakowski. All rights reserved.

#include "ECFDelayBP.h"
#include "EnhancedCodeFlow.h"

UECFDelayBP* UECFDelayBP::ECFDelay(UObject* WorldContextObject, float DelayTime, FECFActionSettings Settings, FECFHandleBP& Handle)
{
	UECFDelayBP* Proxy = NewObject<UECFDelayBP>();
	Proxy->Init(WorldContextObject, Settings);

	Proxy->Proxy_Handle = FFlow::Delay(WorldContextObject, DelayTime, [Proxy]()
	{
		Proxy->OnComplete.Broadcast();
	}, Settings);
	Handle = FECFHandleBP(Proxy->Proxy_Handle);

	return Proxy;
}
