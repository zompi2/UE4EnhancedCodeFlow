﻿// Copyright (c) 2025 Damian Nowakowski. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ECFHandle.h"
#include "ECFHandleBP.generated.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

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


	FECFHandleBP(const FECFHandleBP& Other) :
		Handle(Other.Handle)
	{}

	FECFHandleBP(FECFHandleBP&& Other) :
		Handle(MoveTemp(Other.Handle))
	{}

	FECFHandleBP& operator=(const FECFHandleBP& Other)
	{
		Handle = Other.Handle;
		return *this;
	}

	FECFHandleBP& operator=(FECFHandleBP&& Other)
	{
		Handle = Forward<FECFHandle>(Other.Handle);
		return *this;
	}
};

ECF_PRAGMA_ENABLE_OPTIMIZATION