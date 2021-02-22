// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

/**
 * Library of static functions used to launch Code Flow Actions.
 * You must use this library to control the plugin.
 * Most of the functions requires WorldContextObject to determine the
 * exact world this function should be launch in. This is needed in a case
 * if multiple worlds are running in the same editor instance (e.g. when testing multiplayer).
 * Every Action function require it's owner, so it can be properly cleaned up in case the owner
 * might be destroyed.
 * Callbacks should be defined using lambdas as they are unique and will be moved into plugin stack.
 * Example of the plugin usage when using Delay Action:
 * FFlow::Delay(this, 2.f, [this](float DeltaTime)
 * {
 *     // Stuff to do after 2 seconds delay.
 * });
 */

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ECFHandle.h"
#include "ECFTypes.h"

class ENHANCEDCODEFLOW_API FEnhancedCodeFlow
{

public:

	/**
	 * Stops the running action pointed by given handle. Invalidates given handle.
	 */
	static void StopAction(const UObject* WorldContextObject, FECFHandle& Handle);

	/**
	 * Checks if the action pointed by given handle is running.
	 */
	static bool IsActionRunning(const UObject* WorldContextObject, const FECFHandle& Handle);
	
	/**
	 * Stops all running actions.
	 */
	static void StopAllActions(const UObject* WorldContextObject);

	/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

	/**
	 * Adds a ticker that will tick as long as it won't be stopped or when owning object won't be destroyed.
	 * @param InTickFunc - a ticking function can be: [](float DeltaTime) -> void, or [](float DeltaTime, FECFHandle TickerHandle) -> void.
	 */
	static FECFHandle AddTicker(UObject* InOwner, TUniqueFunction<void(float)>&& InTickFunc);
	static FECFHandle AddTicker(UObject* InOwner, TUniqueFunction<void(float, FECFHandle)>&& InTickFunc);

	/**
	 * Removes all running tickers.
	 * @param InOwner [optional] -	if defined it will remove tickers only from the given owner. Otherwise
	 *								it will remove tickers from everywhere.
	 */
	static void RemoveAllTickers(const UObject* WorldContextObject, UObject* InOwner = nullptr);

	/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

	/**
	 * Execute specified action after some time.
	 * @param InDelayTiem - time in seconds to wait before executing action.
	 * @param InCallbackFunc - a callback with action to execute. Must be: []() -> void. 
	 */
	static FECFHandle Delay(UObject* InOwner, float InDelayTime, TUniqueFunction<void()>&& InCallbackFunc);

	/**
	 * Stops all delays. Callbacks will not be executed.
	 * @param InOwner [optional] -	if defined it will remove delayed actions only from the given owner. Otherwise
	 *								it will remove delayed actions from everywhere.
	 */
	static void RemoveAllDelays(const UObject* WorldContextObject, UObject* InOwner = nullptr);

	/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

	static FECFHandle WaitAndExecute(UObject* InOwner, TUniqueFunction<bool()>&& InPredicate, TUniqueFunction<void()>&& InCallbackFunc);
	static void RemoveAllWaitAndExecutes(const UObject* WorldContextObject, UObject* InOwner = nullptr);

	static FECFHandle WhileTrueExecute(UObject* InOwner, TUniqueFunction<bool()>&& InPredicate, TUniqueFunction<void(float)>&& InTickFunc);
	static void RemoveAllWhileTrueExecutes(const UObject* WorldContextObject, UObject* InOwner = nullptr);

	static FECFHandle AddTimeline(UObject* InOwner, float InStartValue, float InStopValue, float InTime, TUniqueFunction<void(float)>&& InTickFunc, TUniqueFunction<void(float)>&& InCallbackFunc = nullptr, EECFBlendFunc InBlendFunc = EECFBlendFunc::ECFBlend_Linear, float InBlendExp = 1.f);
	static void RemoveAllTimelines(const UObject* WorldContextObject, UObject* InOwner = nullptr);

	static FECFHandle AddCustomTimeline(UObject* InOwner, UCurveFloat* CurveFloat, TUniqueFunction<void(float)>&& InTickFunc, TUniqueFunction<void(float)>&& InCallbackFunc = nullptr);
	static void RemoveAllCustomTimelines(const UObject* WorldContextObject, UObject* InOwner = nullptr);
};

using FFlow = FEnhancedCodeFlow;