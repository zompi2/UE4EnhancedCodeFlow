// Copyright (c) 2024 Damian Nowakowski. All rights reserved.

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
	 * Default constructor and destructor. Used only for stat counting.
	 */
	UECFActionBP();
	virtual ~UECFActionBP();

	/**
	 * Initializes the Async Action with project WorldContext Object and Settings.
	 * Set the action to start paused, as unreal will run it in next tick.
	 */
	virtual void Init(const UObject* WorldContextObject, FECFActionSettings& Settings);

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
	const UObject* Proxy_WorldContextObject;

	// Handle of the Action to control.
	FECFHandle Proxy_Handle;

	// If the action was supposed to be paused - to not resume it on Activate!
	bool Proxy_IsPausedAtStart;

	// Just a handy flag to check if the action has been activated already.
	bool bActivated = false;

	// Mark this async node as ready to destroy
	void ClearAsyncBPAction();
};
