// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "ECFActionSettings.h"
#include "ECFDelayBP.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnECFDelayBPComplete);

UCLASS()
class ENHANCEDCODEFLOW_API UECFDelayBP : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnECFDelayBPComplete OnComplete;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay="Settings"))
	static UECFDelayBP* ECFDelay(UObject* WorldContextObject, float DelayTime, FECFActionSettings Settings);

	void Activate() override;

protected:

	UPROPERTY(Transient)
	class UObject* Proxy_WorldContextObject;

	float Proxy_DelayTime;
	FECFActionSettings Proxy_Settings;

};
