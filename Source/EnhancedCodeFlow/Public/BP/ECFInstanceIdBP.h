// Copyright (c) 2023 Damian Nowakowski. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ECFInstanceId.h"
#include "ECFInstanceIdBP.generated.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

USTRUCT(BlueprintType)
struct ENHANCEDCODEFLOW_API FECFInstanceIdBP
{
	GENERATED_BODY()

	FECFInstanceId InstanceId;

	FECFInstanceIdBP()
	{}


	FECFInstanceIdBP(const FECFInstanceId& InInstanceId) :
		InstanceId(InInstanceId)
	{}

	FECFInstanceIdBP(FECFInstanceId&& InInstanceId) :
		InstanceId(MoveTemp(InInstanceId))
	{}


	FECFInstanceIdBP(const FECFInstanceIdBP& Other) :
		InstanceId(Other.InstanceId)
	{}

	FECFInstanceIdBP(FECFInstanceIdBP&& Other) :
		InstanceId(MoveTemp(Other.InstanceId))
	{}

	FECFInstanceIdBP& operator=(const FECFInstanceIdBP& Other)
	{
		InstanceId = Other.InstanceId;
		return *this;
	}

	FECFInstanceIdBP& operator=(FECFInstanceIdBP&& Other)
	{
		InstanceId = Forward<FECFInstanceId>(Other.InstanceId);
		return *this;
	}
};

ECF_PRAGMA_ENABLE_OPTIMIZATION