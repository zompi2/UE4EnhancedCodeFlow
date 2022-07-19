// Copyright (c) 2022 Damian Nowakowski. All rights reserved.

#pragma once

#include "../ECFActionBP.h"
#include "ECFTickerBP.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnECFTickerBPEvent, float, DeltaTime);

UCLASS()
class ENHANCEDCODEFLOW_API UECFTickerBP : public UECFActionBP
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnECFTickerBPEvent OnTick;

	UPROPERTY(BlueprintAssignable)
	FOnECFTickerBPEvent OnComplete;

	/**
	 * Creates a ticker. It can tick specified amount of time or until it won't be stopped or when owning object won't be destroyed.
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = "Settings", DisplayName = "ECF - Ticker"), Category = "ECF")
	static UECFTickerBP* ECFTicker(UObject* WorldContextObject, float TickingTime, FECFActionSettings Settings, FECFHandleBP& Handle);
};
