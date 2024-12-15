// Copyright (c) 2024 Damian Nowakowski. All rights reserved.

#pragma once

#include "../ECFActionBP.h"
#include "ECFCustomTimelineLinearColorBP.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnECFCustomTimelineLinearColorBPEvent, FLinearColor, Value, float, Time, bool, bStopped);

UCLASS()
class ENHANCEDCODEFLOW_API UECFCustomTimelineLinearColorBP : public UECFActionBP
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnECFCustomTimelineLinearColorBPEvent OnTick;

	UPROPERTY(BlueprintAssignable)
	FOnECFCustomTimelineLinearColorBPEvent OnFinished;
	
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = "Settings", ToolTip = "Adds a custom timeline defined by a float curve.", DisplayName = "ECF - Custom Timeline LinearColor"), Category = "ECF")
	static UECFCustomTimelineLinearColorBP* ECFCustomTimelineLinearColor(const UObject* WorldContextObject, class UCurveLinearColor* CurveLinearColor, FECFActionSettings Settings, FECFHandleBP& Handle);
};
