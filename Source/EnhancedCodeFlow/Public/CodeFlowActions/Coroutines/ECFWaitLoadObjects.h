// Copyright (c) 2025 Damian Nowakowski. All rights reserved.

#pragma once

#include "Coroutines/ECFCoroutineActionBase.h"
#include "Engine/AssetManager.h"
#include "ECFWaitLoadObjects.generated.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UCLASS()
class ENHANCEDCODEFLOW_API UECFWaitLoadObjects : public UECFCoroutineActionBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

protected:

	TArray<FSoftObjectPath> ObjectsToLoad;
	TSharedPtr<FStreamableHandle> StreamableHandle;

	bool Setup(const TArray<FSoftObjectPath>& InObjectsToLoad)
	{
		if (InObjectsToLoad.Num() == 0)
		{
#if ECF_LOGS
			UE_LOG(LogECF, Error, TEXT("ECF Coroutine [%s] - wait load objects failed to start. Objects array is empty."), *Settings.Label);
#endif
			return false;
		}

		ObjectsToLoad = InObjectsToLoad;

#if ECF_LOGS
		UE_LOG(LogECF, Log, TEXT("ECF Coroutine [%s] - Loading %d objects asynchronously."), *Settings.Label, ObjectsToLoad.Num());
#endif

		return true;
	}

	void Init() override
	{
		TWeakObjectPtr<ThisClass> WeakThis(this);
		FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
		StreamableHandle = StreamableManager.RequestAsyncLoad(
			ObjectsToLoad,
			[WeakThis]()
			{
				if (ThisClass* StrongThis = WeakThis.Get())
				{
					if (StrongThis->IsValid())
					{
#if ECF_LOGS
						UE_LOG(LogECF, Log, TEXT("ECF Coroutine [%s] - Finished loading %d objects."), *StrongThis->Settings.Label, StrongThis->ObjectsToLoad.Num());
#endif
						StrongThis->MarkAsFinished();
						StrongThis->Complete(false);
					}
				}
			}
		);
	}

	void Complete(bool bStopped) override
	{
		if (StreamableHandle.IsValid())
		{
			if (StreamableHandle->IsActive())
			{
				StreamableHandle->CancelHandle();
			}
			StreamableHandle.Reset();
		}

		CoroutineHandle.promise().bStopped = bStopped;
		CoroutineHandle.resume();
	}
};

ECF_PRAGMA_ENABLE_OPTIMIZATION
