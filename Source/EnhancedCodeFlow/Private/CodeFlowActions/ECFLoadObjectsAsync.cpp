// Copyright (c) 2025 Damian Nowakowski. All rights reserved.

#include "CodeFlowActions/ECFLoadObjectsAsync.h"
#include "Async/StreamableManager.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

void UECFLoadObjectsAsync::Init()
{
	UECFActionBase::Init();

	TWeakObjectPtr<ThisClass> WeakThis(this);

	// Load all objects asynchronously using the global StreamableManager
	StreamableHandle = FStreamableManager::Get().RequestAsyncLoad(
		ObjectsToLoad,
		[WeakThis]()
		{
			if (ThisClass* StrongThis = WeakThis.Get())
			{
				if (StrongThis->IsValid())
				{
#if ECF_LOGS
					UE_LOG(LogECF, Log, TEXT("ECF - [%s] Finished loading %d objects."), *StrongThis->Settings.Label, StrongThis->ObjectsToLoad.Num());
#endif
					StrongThis->bLoadingFinished = true;
					StrongThis->MarkAsFinished();
				}
			}
		}
	);
}

void UECFLoadObjectsAsync::Tick(float DeltaTime)
{
	UECFActionBase::Tick(DeltaTime);
}

void UECFLoadObjectsAsync::Complete(bool bStopped)
{
	UECFActionBase::Complete(bStopped);

	if (bStopped && StreamableHandle.IsValid())
	{
		// Release the streamable handle
		StreamableHandle.Reset();

#if ECF_LOGS
		UE_LOG(LogECF, Log, TEXT("ECF - [%s] Stopped loading objects."), *Settings.Label);
#endif
	}

	if (CallbackFunc)
	{
		CallbackFunc(bStopped);
	}
}

bool UECFLoadObjectsAsync::HasValidOwner() const
{
	return UECFActionBase::HasValidOwner();
}

ECF_PRAGMA_ENABLE_OPTIMIZATION
