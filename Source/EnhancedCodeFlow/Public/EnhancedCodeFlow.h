// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ECFNodeHandle.h"
#include "EnhancedCodeFlow.generated.h"

UCLASS()
class ENHANCEDCODEFLOW_API UEnhancedCodeFlow : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static void StopTask(FECFNodeHandle Handle);

	static FECFNodeHandle AddRawTicker(UObject* InOwner, TUniqueFunction<void(float)>&& InFunc);
	static FECFNodeHandle Delay(UObject* InOwner, float InDelayTime, TUniqueFunction<void()>&& InFunc);
};

using UFlow = UEnhancedCodeFlow;