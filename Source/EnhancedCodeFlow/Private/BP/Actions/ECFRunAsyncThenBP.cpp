// Copyright (c) 2024 Damian Nowakowski. All rights reserved.

#include "ECFRunAsyncThenBP.h"
#include "EnhancedCodeFlow.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UECFRunAsyncThenBP* UECFRunAsyncThenBP::ECFRunAsyncThen(const UObject* WorldContextObject, float InTimeOut, FECFActionSettings Settings, FECFHandleBP& Handle)
{
	UECFRunAsyncThenBP* Proxy = NewObject<UECFRunAsyncThenBP>();
	Proxy->Init(WorldContextObject, Settings);

	Proxy->Proxy_Handle = FFlow::RunAsyncThen(WorldContextObject,
		[Proxy]()
		{
			Proxy->AsyncTask.Broadcast(false, false);
		},
		[Proxy](bool bTimedOut, bool bStopped)
		{
			Proxy->OnExecute.Broadcast(bTimedOut, bStopped);
			Proxy->ClearAsyncBPAction();
		},
	InTimeOut, ENamedThreads::AnyBackgroundThreadNormalTask, Settings);
	Handle = FECFHandleBP(Proxy->Proxy_Handle);

	return Proxy;
}

ECF_PRAGMA_ENABLE_OPTIMIZATION