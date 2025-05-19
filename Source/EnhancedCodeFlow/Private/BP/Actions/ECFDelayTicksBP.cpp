// Copyright (c) 2025 Damian Nowakowski. All rights reserved.

#include "ECFDelayTicksBP.h"
#include "EnhancedCodeFlow.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UECFDelayTicksBP* UECFDelayTicksBP::ECFDelayTicks(const UObject* WorldContextObject, int32 DelayTicks, FECFActionSettings Settings, FECFHandleBP& Handle)
{
	UECFDelayTicksBP* Proxy = NewObject<UECFDelayTicksBP>();
	if (Proxy)
	{
		Proxy->Init(WorldContextObject, Settings);
		Proxy->Proxy_Handle = FFlow::DelayTicks(WorldContextObject, DelayTicks, [Proxy](bool bStopped)
		{
			if (IsProxyValid(Proxy))
			{
				Proxy->OnComplete.Broadcast(bStopped);
				Proxy->ClearAsyncBPAction();
			}
		}, Settings);
		Handle = FECFHandleBP(Proxy->Proxy_Handle);
	}

	return Proxy;
}

ECF_PRAGMA_ENABLE_OPTIMIZATION