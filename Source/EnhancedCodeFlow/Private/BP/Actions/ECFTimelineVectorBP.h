// Copyright (c) 2025 Damian Nowakowski. All rights reserved.

#pragma once

#include "../ECFActionBP.h"
#include "ECFTypes.h"
#include "ECFTimelineVectorBP.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnECFTimelineVectorBPEvent, FVector, Value, float, Time, bool, bStopped);

UCLASS()
class ENHANCEDCODEFLOW_API UECFTimelineVectorBP : public UECFActionBP
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnECFTimelineVectorBPEvent OnTick;

	UPROPERTY(BlueprintAssignable)
	FOnECFTimelineVectorBPEvent OnFinished;
	
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = "Settings, BlendFunc, BlendExp", ToolTip = "Adds a simple vector timeline that runs in a given range during a given time.", DisplayName = "ECF - Timeline Vector"), Category = "ECF")
	static UECFTimelineVectorBP* ECFTimelineVector(const UObject* WorldContextObject, FVector StartValue, FVector StopValue, float Time, FECFActionSettings Settings, FECFHandleBP& Handle, EECFBlendFunc BlendFunc = EECFBlendFunc::ECFBlend_Linear, float BlendExp = 1.f);
};
