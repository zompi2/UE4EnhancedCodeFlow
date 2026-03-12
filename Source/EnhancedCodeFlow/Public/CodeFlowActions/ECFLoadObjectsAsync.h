// Copyright (c) 2025 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFActionBase.h"
#include "Async/StreamableManager.h"
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
	bool bLoadingFinished = false;

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
		bLoadingFinished = false;

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

	// Template version supporting TSoftObjectPtr and TSoftClassPtr
	template<typename T>
	bool Setup(const TArray<TSoftObjectPtr<T>>& InObjectsToLoad, TUniqueFunction<void(bool)>&& InCallbackFunc)
	{
		TArray<FSoftObjectPath> Paths;
		for (const auto& Object : InObjectsToLoad)
		{
			Paths.Add(Object.ToSoftObjectPath());
		}
		return Setup(Paths, MoveTemp(InCallbackFunc));
	}

	template<typename T>
	bool Setup(const TArray<TSoftObjectPtr<T>>& InObjectsToLoad, TUniqueFunction<void()>&& InCallbackFunc)
	{
		TArray<FSoftObjectPath> Paths;
		for (const auto& Object : InObjectsToLoad)
		{
			Paths.Add(Object.ToSoftObjectPath());
		}
		return Setup(Paths, MoveTemp(InCallbackFunc));
	}

	virtual void Init() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Complete(bool bStopped) override;

public:

	virtual bool HasValidOwner() const override;
};

ECF_PRAGMA_ENABLE_OPTIMIZATION
