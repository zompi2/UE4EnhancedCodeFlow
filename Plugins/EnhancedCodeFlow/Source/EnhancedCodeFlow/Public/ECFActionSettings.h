// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ECFActionSettings.generated.h"

USTRUCT(BlueprintType)
struct FECFActionSettings
{
	GENERATED_BODY()

	FECFActionSettings() :
	TickInterval(0.f),
	bIgnoreTimeDilation(false)
	{

	}

	FECFActionSettings(float InTickInterval) :
		TickInterval(InTickInterval),
		bIgnoreTimeDilation(false)
	{

	}

	FECFActionSettings(bool InIgnoreTimeDilation) :
		TickInterval(0.f),
		bIgnoreTimeDilation(InIgnoreTimeDilation)
	{

	}

	FECFActionSettings(float InTickInterval, bool InIgnoreTimeDilation) :
		TickInterval(InTickInterval),
		bIgnoreTimeDilation(InIgnoreTimeDilation)
	{

	}

	UPROPERTY(BlueprintReadWrite)
	float TickInterval = 0.f;

	UPROPERTY(BlueprintReadWrite)
	bool bIgnoreTimeDilation = false;
};