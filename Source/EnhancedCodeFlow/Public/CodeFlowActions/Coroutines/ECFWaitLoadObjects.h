// Copyright (c) 2026 Damian Nowakowski. All rights reserved.

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
	TArray<FPrimaryAssetId> PrimaryAssetsToLoad;
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

	bool Setup(const TArray<FPrimaryAssetId>& InObjectsToLoad)
	{
		if (InObjectsToLoad.Num() == 0)
		{
#if ECF_LOGS
			UE_LOG(LogECF, Error, TEXT("ECF Coroutine [%s] - wait load objects (Primary Assets) failed to start. Objects array is empty."), *Settings.Label);
#endif
			return false;
		}

		PrimaryAssetsToLoad = InObjectsToLoad;

#if ECF_LOGS
		UE_LOG(LogECF, Log, TEXT("ECF Coroutine [%s] - Loading %d objects (Primary Assets) asynchronously."), *Settings.Label, PrimaryAssetsToLoad.Num());
#endif

		return true;
	}

	void Init() override
	{
		TWeakObjectPtr<ThisClass> WeakThis(this);

		if (ObjectsToLoad.Num() > 0)
		{
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
		else if (PrimaryAssetsToLoad.Num() > 0)
		{
			StreamableHandle = UAssetManager::Get().LoadPrimaryAssets(
				PrimaryAssetsToLoad,
				{},
				FStreamableDelegate::CreateLambda([WeakThis]()
				{
					if (ThisClass* StrongThis = WeakThis.Get())
					{
						if (StrongThis->IsValid())
						{
#if ECF_LOGS
							UE_LOG(LogECF, Log, TEXT("ECF Coroutine [%s] - Finished loading %d objects (Primary Assets)."), *StrongThis->Settings.Label, StrongThis->PrimaryAssetsToLoad.Num());
#endif
							StrongThis->MarkAsFinished();
							StrongThis->Complete(false);
						}
					}
				}
			));
		}
		else
		{
#if ECF_LOGS
			UE_LOG(LogECF, Error, TEXT("ECF Coroutine [%s] - wait load objects failed to initialize. Objects arrays are empty."), *Settings.Label);
#endif
		}
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
