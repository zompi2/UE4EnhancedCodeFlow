// Copyright (c) 2024 Damian Nowakowski. All rights reserved.

#pragma once

#include "../ECFActionBP.h"
#include "ECFTypes.h"
#include "ECFTimelineBP.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnECFTimelineBPEvent, float, Value, float, Time, bool, bStopped);

UCLASS()
class ENHANCEDCODEFLOW_API UECFTimelineBP : public UECFActionBP
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnECFTimelineBPEvent OnTick;

	UPROPERTY(BlueprintAssignable)
	FOnECFTimelineBPEvent OnFinished;
	
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = "Settings, BlendFunc, BlendExp", ToolTip = "Adds a simple timeline that runs in a given range during a given time.", DisplayName = "ECF - Timeline"), Category = "ECF")
	static UECFTimelineBP* ECFTimeline(const UObject* WorldContextObject, float StartValue, float StopValue, float Time, FECFActionSettings Settings, FECFHandleBP& Handle, EECFBlendFunc BlendFunc = EECFBlendFunc::ECFBlend_Linear, float BlendExp = 1.f);
};
