// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "ECFActionSettings.h"
#include "ECFTickerBP.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnECFTickerBPTick, float, DeltaTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnECFTickerBPComplete);

UCLASS()
class ENHANCEDCODEFLOW_API UECFTickerBP : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnECFTickerBPTick OnTick;
	
	UPROPERTY(BlueprintAssignable)
	FOnECFTickerBPComplete OnComplete;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay="Settings"))
    static UECFTickerBP* ECFTicker(UObject* WorldContextObject, float TickTime, FECFActionSettings Settings);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay="Settings"))
	static UECFTickerBP* ECFInfiniteTicker(UObject* WorldContextObject, FECFActionSettings Settings);

	void Activate() override;

protected:

	UPROPERTY(Transient)
	class UObject* Proxy_WorldContextObject;

	float Proxy_TickTime;

	FECFActionSettings Proxy_Settings;
};
