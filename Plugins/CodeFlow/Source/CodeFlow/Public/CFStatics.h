// Copyright (c) 2020 Damian Nowakowski. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CFStatics.generated.h"

UCLASS()
class CODEFLOW_API UCFStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static uint64 AddRawTicker(UObject* InOwner, TUniqueFunction<void(float)>&& InFunc);
	static uint64 Delay(UObject* InOwner, float InDelayTime, TUniqueFunction<void()>&& InFunc);
};

using UFlow = UCFStatics;