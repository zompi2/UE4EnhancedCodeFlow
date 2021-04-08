#pragma once

#include "CoreMinimal.h"
#include "ECFHandle.h"
#include "ECFHandleBP.generated.h"

USTRUCT(BlueprintType)
struct FECFHandleBP
{
	GENERATED_BODY()

	FECFHandle Handle;
};
