// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ECFHandle.h"
#include "ECFTypes.h"
#include "EnhancedCodeFlow.generated.h"

UCLASS()
class ENHANCEDCODEFLOW_API UEnhancedCodeFlow : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static void StopAction(const UObject* WorldContextObject, FECFHandle& Handle);
	static bool IsActionRunning(const UObject* WorldContextObject, const FECFHandle& Handle);
	static void StopAllActions(const UObject* WorldContextObject);

	static FECFHandle AddTicker(UObject* InOwner, TUniqueFunction<void(float)>&& InTickFunc);
	static FECFHandle AddTicker(UObject* InOwner, TUniqueFunction<void(float, FECFHandle)>&& InTickFunc);
	static void RemoveAllTickers(const UObject* WorldContextObject);

	static FECFHandle Delay(UObject* InOwner, float InDelayTime, TUniqueFunction<void()>&& InCallbackFunc);
	static void RemoveAllDelays(const UObject* WorldContextObject);

	static FECFHandle WaitAndExecute(UObject* InOwner, TUniqueFunction<bool()>&& InPredicate, TUniqueFunction<void()>&& InCallbackFunc);
	static void RemoveAllWaitAndExecutes(const UObject* WorldContextObject);

	static FECFHandle AddTimeline(UObject* InOwner, float InStartValue, float InStopValue, float InTime, TUniqueFunction<void(float)>&& InTickFunc, TUniqueFunction<void(float)>&& InCallbackFunc = nullptr, EECFBlendFunc InBlendFunc = EECFBlendFunc::ECFBlend_Linear, float InBlendExp = 1.f);
	static void RemoveAllTimelines(const UObject* WorldContextObject);

	static FECFHandle AddCustomTimeline(UObject* InOwner, UCurveFloat* CurveFloat, TUniqueFunction<void(float)>&& InTickFunc, TUniqueFunction<void(float)>&& InCallbackFunc = nullptr);
	static void RemoveAllCustomTimelines(const UObject* WorldContextObject);
};

using UFlow = UEnhancedCodeFlow;