// Copyright (c) 2023 Damian Nowakowski. All rights reserved.

#include "ECFDelayBP.h"
#include "EnhancedCodeFlow.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UECFDelayBP* UECFDelayBP::ECFDelay(const UObject* WorldContextObject, float DelayTime, FECFActionSettings Settings, FECFHandleBP& Handle)
{
	UECFDelayBP* Proxy = NewObject<UECFDelayBP>();
	Proxy->Init(WorldContextObject, Settings);

	Proxy->Proxy_Handle = FFlow::Delay(WorldContextObject, DelayTime, [Proxy](bool bStopped)
	{
		Proxy->OnComplete.Broadcast(bStopped);
	}, Settings);
	Handle = FECFHandleBP(Proxy->Proxy_Handle);

	return Proxy;
}

ECF_PRAGMA_ENABLE_OPTIMIZATION