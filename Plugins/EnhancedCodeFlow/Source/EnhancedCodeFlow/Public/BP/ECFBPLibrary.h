// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

/**
 * Library of static functions used to launch Code Flow Actions via Blueprints.
 */

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

	/**
	 * Stops the running action pointed by given handle. Invalidates given handle.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "ECF")
	static void ECFStopAction(const UObject* WorldContextObject, UPARAM(ref) FECFHandleBP& Handle);

	/**
	 * Checks if the action pointed by given handle is running.
	 */
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"), Category = "ECF")
	static bool ECFIsActionRunning(const UObject* WorldContextObject, const FECFHandleBP& Handle);

	/**
	 * Stops all running actions.
	 * If owner is defined it will remove all actions from the given owner.
	 * Otherwise it will stop all the actions from everywhere.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "ECF")
	static void ECFStopAllActions(const UObject* WorldContextObject, UObject* InOwner = nullptr);

	/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

	/**
	 * Creates a ticker. It can tick specified amount of time or until it won't be stopped or when owning object won't be destroyed.
	 */
	UFUNCTION(BlueprintCallable, meta = (HidePin = "Owner", DefaultToSelf = "Owner", AutoCreateRefTerm = "OnFinishedEvent", AdvancedDisplay = "OnFinishedEvent, Settings, TickingTime"), Category = "ECF")
	static FECFHandleBP ECFTicker(UObject* Owner, const FOnECFTick& OnTickEvent, const FOnECFFinished& OnFinishedEvent, FECFActionSettings Settings, float TickingTime = -1.f);

	/**
	 * Removes all running tickers.
	 * If owner is defined it will remove all tickers from the given owner.
	 * Otherwise it will stop all the tickers from everywhere.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "ECF")
	static void ECFRemoveAllTickers(const UObject* WorldContextObject, UObject* InOwner = nullptr);

	/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

	/**
	 * Adds a simple timeline that runs in a given range during a given time.
	 */
	UFUNCTION(BlueprintCallable, meta = (HidePin = "Owner", DefaultToSelf = "Owner", AutoCreateRefTerm = "OnFinishedEvent", AdvancedDisplay = "OnFinishedEvent, Settings, BlendFunc, BlendExp"), Category = "ECF")
	static FECFHandleBP ECFTimeline(UObject* Owner, float StartValue, float StopValue, float Time, const FOnECFTimelineTick& OnTickEvent, const FOnECFTimelineTick& OnFinishedEvent, FECFActionSettings Settings, EECFBlendFunc BlendFunc = EECFBlendFunc::ECFBlend_Linear, float BlendExp = 1.f);

	/**
	 * Removes all running timelines.
	 * If owner is defined it will remove all timelines from the given owner.
	 * Otherwise it will stop all the timelines from everywhere.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "ECF")
	static void ECFRemoveAllTimelines(const UObject* WorldContextObject, UObject* InOwner = nullptr);

	/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

	/**
	 * Adds a custom timeline defined by a float curve.
	 */
	UFUNCTION(BlueprintCallable, meta = (HidePin = "Owner", DefaultToSelf = "Owner", AutoCreateRefTerm = "OnFinishedEvent", AdvancedDisplay = "OnFinishedEvent, Settings"), Category = "ECF")
	static FECFHandleBP ECFCustomTimeline(UObject* Owner, UCurveFloat* CurveFloat, const FOnECFTimelineTick& OnTickEvent, const FOnECFTimelineTick& OnFinishedEvent, FECFActionSettings Settings);

	/**
	 * Removes all running custom timelines.
	 * If owner is defined it will remove all custom timelines from the given owner.
	 * Otherwise it will stop all the custom timelines from everywhere.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "ECF")
	static void ECFRemoveAllCustomTimelines(const UObject* WorldContextObject, UObject* InOwner = nullptr);

	/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

	/**
	 * Removes all running delays.
	 * If owner is defined it will remove all delays from the given owner.
	 * Otherwise it will stop all the delays from everywhere.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "ECF")
	static void ECFRemoveAllDelays(const UObject* WorldContextObject, UObject* InOwner = nullptr);

	/**
	 * Removes all running "wait and execute"s.
	 * If owner is defined it will remove all "wait and execute"s from the given owner.
	 * Otherwise it will stop all the "wait and execute"s from everywhere.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "ECF")
	static void ECFRemoveAllWaitAndExecutes(const UObject* WorldContextObject, UObject* InOwner = nullptr);

	/**
	 * Removes all running "while true execute"s.
	 * If owner is defined it will remove all "while true execute"s from the given owner.
	 * Otherwise it will stop all the "while true execute"s from everywhere.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "ECF")
	static void RemoveAllWhileTrueExecutes(const UObject* WorldContextObject, UObject* InOwner = nullptr);

	/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
};
