// Copyright (c) 2025 Damian Nowakowski. All rights reserved.

#include "CodeFlowActions/ECFLoadObjectsAsync.h"
#include "ECFSubsystem.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

void UECFLoadObjectsAsync::Init()
{
	UECFActionBase::Init();

	if (UWorld* World = GetWorld())
	{
		if (UECFSubsystem* ECFSubsystem = World->GetSubsystem<UECFSubsystem>())
		{
			FStreamableManager& StreamableManager = ECFSubsystem->GetStreamableManager();

			TWeakObjectPtr<ThisClass> WeakThis(this);

			// Load all objects asynchronously
			StreamableHandle = StreamableManager.RequestAsyncLoad(
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
							StrongThis->MarkAsFinished();
						}
					}
				}
			);
		}
	}
}

void UECFLoadObjectsAsync::Tick(float DeltaTime)
{
	UECFActionBase::Tick(DeltaTime);

	// Check if the loading handle is still valid
	if (!StreamableHandle.IsValid() || !StreamableHandle->IsLoading())
	{
		if (IsValid())
		{
			CallbackFunc(false);
		}
		MarkAsFinished();
	}
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
