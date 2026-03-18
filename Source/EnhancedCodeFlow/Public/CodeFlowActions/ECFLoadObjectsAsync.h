// Copyright (c) 2025 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFActionBase.h"
#include "Engine/AssetManager.h"
#include "ECFLoadObjectsAsync.generated.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UCLASS()
class ENHANCEDCODEFLOW_API UECFLoadObjectsAsync : public UECFActionBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

protected:

	TArray<FSoftObjectPath> ObjectsToLoad;
	TSharedPtr<FStreamableHandle> StreamableHandle;

	TUniqueFunction<void(bool)> CallbackFunc;
	TUniqueFunction<void()> CallbackFunc_NoStopped;

	bool Setup(const TArray<FSoftObjectPath>& InObjectsToLoad, TUniqueFunction<void(bool)>&& InCallbackFunc)
	{
		if (InObjectsToLoad.Num() == 0)
		{
#if ECF_LOGS
			UE_LOG(LogECF, Error, TEXT("ECF - [%s] Load Objects Async failed to start. Objects array is empty."), *Settings.Label);
#endif
			return false;
		}

		if (!InCallbackFunc)
		{
#if ECF_LOGS
			UE_LOG(LogECF, Error, TEXT("ECF - [%s] Load Objects Async failed to start. Callback function is not set."), *Settings.Label);
#endif
			return false;
		}

		ObjectsToLoad = InObjectsToLoad;
		CallbackFunc = MoveTemp(InCallbackFunc);



#if ECF_LOGS
		UE_LOG(LogECF, Log, TEXT("ECF - [%s] Loading %d objects asynchronously."), *Settings.Label, ObjectsToLoad.Num());
#endif

		return true;
	}

	bool Setup(const TArray<FSoftObjectPath>& InObjectsToLoad, TUniqueFunction<void()>&& InCallbackFunc)
	{
		CallbackFunc_NoStopped = MoveTemp(InCallbackFunc);
		if (CallbackFunc_NoStopped)
		{
			return Setup(InObjectsToLoad, [this](bool bStopped)
			{
				CallbackFunc_NoStopped();
			});
		}
		else
		{
#if ECF_LOGS
			UE_LOG(LogECF, Error, TEXT("ECF - [%s] Load Objects Async failed to start. Callback function is not set."), *Settings.Label);
#endif
			return false;
		}
	}

	void Init() override
	{
		TWeakObjectPtr<ThisClass> WeakThis(this);
		FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
		StreamableHandle = StreamableManager.RequestAsyncLoad(ObjectsToLoad,
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

		if (CallbackFunc)
		{
			CallbackFunc(bStopped);
		}
	}
};

ECF_PRAGMA_ENABLE_OPTIMIZATION
