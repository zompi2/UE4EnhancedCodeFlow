#pragma once

#include "CoreMinimal.h"
#include "ECFHandle.h"
#include "ECFHandleBP.generated.h"

USTRUCT(BlueprintType)
struct ENHANCEDCODEFLOW_API FECFHandleBP
{
	GENERATED_BODY()

	FECFHandle Handle;

	FECFHandleBP()
	{}

	FECFHandleBP(FECFHandle InHandle) :
		Handle(InHandle)
	{}
};
