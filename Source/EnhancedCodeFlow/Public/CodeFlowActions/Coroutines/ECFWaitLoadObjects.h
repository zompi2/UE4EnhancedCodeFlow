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

	// Template version supporting TSoftObjectPtr and TSoftClassPtr
	template<typename T>
	bool Setup(const TArray<TSoftObjectPtr<T>>& InObjectsToLoad)
	{
		TArray<FSoftObjectPath> Paths;
		for (const auto& Object : InObjectsToLoad)
		{
			Paths.Add(Object.ToSoftObjectPath());
		}
		return Setup(Paths);
	}

	void Init() override
	{
		UECFCoroutineActionBase::Init();

		TWeakObjectPtr<ThisClass> WeakThis(this);

		// Load all objects asynchronously using the global StreamableManager
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

	void Tick(float DeltaTime) override
	{
#if STATS
		DECLARE_SCOPE_CYCLE_COUNTER(TEXT("WaitLoadObjects - Tick"), STAT_ECFDETAILS_WAITLOADOBJECTS, STATGROUP_ECFDETAILS);
#endif

#if ECF_INSIGHT_PROFILING
		TRACE_CPUPROFILER_EVENT_SCOPE("ECF - WaitLoadObjects Tick");
#endif
	}

	void Complete(bool bStopped) override
	{
		CoroutineHandle.promise().bStopped = bStopped;
		CoroutineHandle.resume();
	}
};

ECF_PRAGMA_ENABLE_OPTIMIZATION
