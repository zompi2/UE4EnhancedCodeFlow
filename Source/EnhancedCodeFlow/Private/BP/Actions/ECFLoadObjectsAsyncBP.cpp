// Copyright (c) 2025 Damian Nowakowski. All rights reserved.

#include "ECFLoadObjectsAsyncBP.h"
#include "EnhancedCodeFlow.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UECFLoadObjectsAsyncBP* UECFLoadObjectsAsyncBP::ECFLoadObjectsAsync(const UObject* WorldContextObject, const TArray<FSoftObjectPath>& ObjectsToLoad, FECFActionSettings Settings, FECFHandleBP& Handle)
{
	UECFLoadObjectsAsyncBP* Proxy = NewObject<UECFLoadObjectsAsyncBP>();
	if (Proxy)
	{
		Proxy->Init(WorldContextObject, Settings);
		Proxy->Proxy_Handle = FFlow::LoadObjectsAsync(WorldContextObject, ObjectsToLoad, [Proxy](bool bStopped)
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
