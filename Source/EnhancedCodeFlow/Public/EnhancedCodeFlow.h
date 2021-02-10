// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ECFHandle.h"
#include "EnhancedCodeFlow.generated.h"

UCLASS()
class ENHANCEDCODEFLOW_API UEnhancedCodeFlow : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static void StopAction(FECFHandle& Handle);
	static bool IsActionRunning(const FECFHandle& Handle);

	static FECFHandle AddTicker(UObject* InOwner, TUniqueFunction<void(float)>&& InFunc);
	static FECFHandle AddTicker(UObject* InOwner, TUniqueFunction<void(float, FECFHandle)>&& InFunc);
	static FECFHandle Delay(UObject* InOwner, float InDelayTime, TUniqueFunction<void()>&& InFunc);
	static FECFHandle WaitAndExecute(UObject* InOwner, TUniqueFunction<bool()>&& InPredicate, TUniqueFunction<void()>&& InFunc);
	static FECFHandle AddTimeline(UObject* InOwner, TUniqueFunction<void(float)>&& InTickFunc, TUniqueFunction<void(float)>&& InFunc, float InStartTime, float InStopTime, bool bInLoop);
};

using UFlow = UEnhancedCodeFlow;