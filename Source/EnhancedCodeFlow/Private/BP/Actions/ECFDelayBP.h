// Copyright (c) 2022 Damian Nowakowski. All rights reserved.

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

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = "Settings", ToolTip = "Execute specified action after some time.", DisplayName = "ECF - Delay"), Category = "ECF")
	static UECFDelayBP* ECFDelay(const UObject* WorldContextObject, float DelayTime, FECFActionSettings Settings, FECFHandleBP& Handle);
};
