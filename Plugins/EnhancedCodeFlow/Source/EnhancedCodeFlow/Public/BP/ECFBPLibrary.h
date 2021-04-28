// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "BP/ECFHandleBP.h"
#include "ECFActionSettings.h"
#include "ECFTypes.h"
#include "ECFBPLibrary.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnECFTick, float, DeltaTime);
DECLARE_DYNAMIC_DELEGATE(FOnECFFinished);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnECFTimelineTick, float, Value, float, Time);

UCLASS()
class ENHANCEDCODEFLOW_API UECFBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static void ECFStopAction(const UObject* WorldContextObject, UPARAM(ref) FECFHandleBP& Handle);

	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static bool ECFIsActionRunning(const UObject* WorldContextObject, const FECFHandleBP& Handle);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static void ECFStopAllActions(const UObject* WorldContextObject, UObject* InOwner = nullptr);

	/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

	UFUNCTION(BlueprintCallable, meta = (HidePin = "Owner", DefaultToSelf = "Owner", AutoCreateRefTerm = "OnFinishedEvent", AdvancedDisplay = "OnFinishedEvent, Settings, TickingTime"))
	static FECFHandleBP ECFTicker(UObject* Owner, const FOnECFTick& OnTickEvent, const FOnECFFinished& OnFinishedEvent, FECFActionSettings Settings, float TickingTime = -1.f);

	/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

	UFUNCTION(BlueprintCallable, meta = (HidePin = "Owner", DefaultToSelf = "Owner", AutoCreateRefTerm = "OnFinishedEvent", AdvancedDisplay = "OnFinishedEvent, Settings, BlendFunc, BlendExp"))
	static FECFHandleBP ECFTimeline(UObject* Owner, float StartValue, float StopValue, float Time, const FOnECFTimelineTick& OnTickEvent, const FOnECFTimelineTick& OnFinishedEvent, FECFActionSettings Settings, EECFBlendFunc BlendFunc = EECFBlendFunc::ECFBlend_Linear, float BlendExp = 1.f);

	/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

	UFUNCTION(BlueprintCallable, meta = (HidePin = "Owner", DefaultToSelf = "Owner", AutoCreateRefTerm = "OnFinishedEvent", AdvancedDisplay = "OnFinishedEvent, Settings"))
	static FECFHandleBP ECFCustomTimeline(UObject* Owner, UCurveFloat* CurveFloat, const FOnECFTimelineTick& OnTickEvent, const FOnECFTimelineTick& OnFinishedEvent, FECFActionSettings Settings);

	/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
};
