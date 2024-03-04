// Copyright (c) 2024 Damian Nowakowski. All rights reserved.

#include "ECFDoNoMoreThanXTimeBP.h"
#include "EnhancedCodeFlow.h"
#include "BP/ECFBPLibrary.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UECFDoNoMoreThanXTimeBP* UECFDoNoMoreThanXTimeBP::ECFDoNoMoreThanXTime(const UObject* WorldContextObject, float Time, FECFHandleBP& Handle, FECFInstanceIdBP& InstanceId, FECFActionSettings Settings, int32 MaxExecsEnqueued /*= 1*/)
{
	UECFDoNoMoreThanXTimeBP* Proxy = NewObject<UECFDoNoMoreThanXTimeBP>();
	Proxy->Init(WorldContextObject, Settings);

	if (InstanceId.InstanceId.IsValid() == false)
	{
		UECFBPLibrary::ECFGetNewInstanceId(InstanceId);
	}

	Proxy->Proxy_Handle = FFlow::DoNoMoreThanXTime(WorldContextObject, [Proxy]()
	{
		// Because the action will be executed on first call, check if the async action has been activated.
		// Not activated actions don't have bindings to delegates! 
		// Enqueue the OnExecute broadcast for the activation.
		if (Proxy->bActivated)
		{
			Proxy->OnExecute.Broadcast();
			Proxy->ClearAsyncBPAction();
		}
		else
		{
			Proxy->bExecuteOnActivation = true;
		}
	}, Time, MaxExecsEnqueued, InstanceId.InstanceId, Settings);
	Handle = FECFHandleBP(Proxy->Proxy_Handle);

	return Proxy;
}

void UECFDoNoMoreThanXTimeBP::Activate()
{
	Super::Activate();

	// Broadcast OnExecute event if it was enqueued.
	if (bExecuteOnActivation)
	{
		bExecuteOnActivation = false;
		OnExecute.Broadcast();
		ClearAsyncBPAction();
	}
}

ECF_PRAGMA_ENABLE_OPTIMIZATION