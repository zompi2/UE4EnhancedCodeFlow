// Copyright (c) 2022 Damian Nowakowski. All rights reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "ECFActionSettings.h"
#include "BP/ECFHandleBP.h"
#include "ECFActionBP.generated.h"

UCLASS()
class ENHANCEDCODEFLOW_API UECFActionBP : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	/**
	 * Initializes the Async Action with project WorldContext Object and Settings.
	 * Set the action to start paused, as unreal will run it in next tick.
	 */
	virtual void Init(UObject* WorldContextObject, FECFActionSettings& Settings);

	/**
	 * Getting world from World Context Object.
	 */
	class UWorld* GetWorld() const override;

	/**
	 * Activate the action by resuming it (we always start it as paused).
	 */
	void Activate() override;

protected:

	// The World Context Object that started this action.
	UPROPERTY(Transient)
	class UObject* Proxy_WorldContextObject;

	// Handle of the Action to control.
	FECFHandle Proxy_Handle;

	// If the action was supposed to be paused - to not resume it on Activate!
	bool Proxy_IsPausedAtStart;
};
