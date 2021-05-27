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

	FECFHandleBP(const FECFHandle& InHandle) :
		Handle(InHandle)
	{}

	FECFHandleBP(FECFHandle&& InHandle) :
		Handle(MoveTemp(InHandle))
	{}
};
