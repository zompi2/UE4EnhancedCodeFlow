// Copyright (c) 2024 Damian Nowakowski. All rights reserved.

#include "ECFRunAsyncThenBP.h"
#include "EnhancedCodeFlow.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UECFRunAsyncThenBP* UECFRunAsyncThenBP::ECFRunAsyncThen(const UObject* WorldContextObject, float InTimeOut, EECFAsyncPrio Priority, FECFActionSettings Settings, FECFHandleBP& Handle)
{
	UECFRunAsyncThenBP* Proxy = NewObject<UECFRunAsyncThenBP>();
	if (Proxy)
	{
		Proxy->Init(WorldContextObject, Settings);

		Proxy->Proxy_Handle = FFlow::RunAsyncThen(WorldContextObject,
			[Proxy]()
			{
				if (IsProxyValid(Proxy))
				{
					Proxy->AsyncTask.Broadcast(false, false);
				}
			},
			[Proxy](bool bTimedOut, bool bStopped)
			{
				if (IsProxyValid(Proxy))
				{
					Proxy->OnExecute.Broadcast(bTimedOut, bStopped);
					Proxy->ClearAsyncBPAction();
				}
			},
		InTimeOut, Priority, Settings);
		Handle = FECFHandleBP(Proxy->Proxy_Handle);
	}

	return Proxy;
}

ECF_PRAGMA_ENABLE_OPTIMIZATION