// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "BP/ECFHandleBP.h"
#include "ECFActionSettings.h"
#include "ECFBPLibrary.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnECFTick, float, DeltaTime);
DECLARE_DYNAMIC_DELEGATE(FOnECFTickerFinished);

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

	UFUNCTION(BlueprintCallable, meta = (HidePin = "Owner", DefaultToSelf = "Owner", AdvancedDisplay = "Settings, TickingTime"))
	static FECFHandleBP ECFTicker(UObject* Owner, FOnECFTick OnTickEvent, FECFActionSettings Settings, float TickingTime = -1.f);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "Owner", DefaultToSelf = "Owner", AdvancedDisplay = "Settings, TickingTime"))
	static FECFHandleBP ECFTickerWithFinishCallback(UObject* Owner, FOnECFTick OnTickEvent, FOnECFTickerFinished OnTickerFinishedEvent, FECFActionSettings Settings, float TickingTime = -1.f);

	/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
};
