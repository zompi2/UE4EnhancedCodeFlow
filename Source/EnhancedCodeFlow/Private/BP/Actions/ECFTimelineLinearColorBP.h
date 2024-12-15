// Copyright (c) 2024 Damian Nowakowski. All rights reserved.

#pragma once

#include "../ECFActionBP.h"
#include "ECFTypes.h"
#include "ECFTimelineLinearColorBP.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnECFTimelineLinearColorBPEvent, FLinearColor, Value, float, Time, bool, bStopped);

UCLASS()
class ENHANCEDCODEFLOW_API UECFTimelineLinearColorBP : public UECFActionBP
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnECFTimelineLinearColorBPEvent OnTick;

	UPROPERTY(BlueprintAssignable)
	FOnECFTimelineLinearColorBPEvent OnFinished;
	
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = "Settings, BlendFunc, BlendExp", ToolTip = "Adds a simple LinearColor timeline that runs in a given range during a given time.", DisplayName = "ECF - Timeline LinearColor"), Category = "ECF")
	static UECFTimelineLinearColorBP* ECFTimelineLinearColor(const UObject* WorldContextObject, FLinearColor StartValue, FLinearColor StopValue, float Time, FECFActionSettings Settings, FECFHandleBP& Handle, EECFBlendFunc BlendFunc = EECFBlendFunc::ECFBlend_Linear, float BlendExp = 1.f);
};
