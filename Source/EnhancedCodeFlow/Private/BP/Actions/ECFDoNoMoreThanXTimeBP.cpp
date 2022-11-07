// Copyright (c) 2022 Damian Nowakowski. All rights reserved.

#include "ECFDoNoMoreThanXTimeBP.h"
#include "EnhancedCodeFlow.h"
#include "BP/ECFBPLibrary.h"

UECFDoNoMoreThanXTimeBP* UECFDoNoMoreThanXTimeBP::ECFDoNoMoreThanXTime(UObject* WorldContextObject, float Time, FECFHandleBP& Handle, FECFInstanceIdBP& InstanceId, FECFActionSettings Settings, int32 MaxExecsEnqueued /*= 1*/)
{
	UECFDoNoMoreThanXTimeBP* Proxy = NewObject<UECFDoNoMoreThanXTimeBP>();
	Proxy->Init(WorldContextObject, Settings);

	if (InstanceId.InstanceId.IsValid() == false)
	{
		UECFBPLibrary::ECFGetNewInstanceId(InstanceId);
	}

	Proxy->Proxy_Handle = FFlow::DoNoMoreThanXTime(Proxy, [Proxy]()
	{
		Proxy->OnExecute.Broadcast();
	}, Time, MaxExecsEnqueued, InstanceId.InstanceId, Settings);
	Handle = FECFHandleBP(Proxy->Proxy_Handle);

	return Proxy;
}
