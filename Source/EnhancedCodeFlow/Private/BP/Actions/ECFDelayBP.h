// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "../ECFActionBP.h"
#include "ECFDelayBP.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnECFDelayBPEvent);

UCLASS()
class ENHANCEDCODEFLOW_API UECFDelayBP : public UECFActionBP
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnECFDelayBPEvent OnComplete;

	/**
	 * Execute specified action after some time.
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = "Settings"), Category = "ECF")
	static UECFDelayBP* ECFDelay(UObject* WorldContextObject, float DelayTime, FECFActionSettings Settings, FECFHandleBP& Handle);
};
