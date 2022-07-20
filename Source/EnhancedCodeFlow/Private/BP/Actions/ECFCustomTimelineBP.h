// Copyright (c) 2022 Damian Nowakowski. All rights reserved.

#pragma once

#include "../ECFActionBP.h"
#include "ECFCustomTimelineBP.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnECFCustomTimelineBPEvent, float, Value, float, Time);

UCLASS()
class ENHANCEDCODEFLOW_API UECFCustomTimelineBP : public UECFActionBP
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnECFCustomTimelineBPEvent OnTick;

	UPROPERTY(BlueprintAssignable)
	FOnECFCustomTimelineBPEvent OnFinished;
	
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = "Settings", ToolTip = "Adds a custom timeline defined by a float curve.", DisplayName = "ECF - Custom Timeline"), Category = "ECF")
	static UECFCustomTimelineBP* ECFCustomTimeline(UObject* WorldContextObject, UCurveFloat* CurveFloat, FECFActionSettings Settings, FECFHandleBP& Handle);
};
