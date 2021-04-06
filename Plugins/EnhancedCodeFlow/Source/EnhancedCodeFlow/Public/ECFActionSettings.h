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
		bIgnorePause(false),
		bIgnoreGlobalTimeDilation(false)
	{

	}

	FECFActionSettings(float InTickInterval, bool InIgnorePause, bool InIgnoreTimeDilation) :
		TickInterval(InTickInterval),
		bIgnorePause(InIgnorePause),
		bIgnoreGlobalTimeDilation(InIgnoreTimeDilation)
	{

	}

	UPROPERTY(BlueprintReadWrite)
	float TickInterval = 0.f;

	UPROPERTY(BlueprintReadWrite)
	bool bIgnorePause = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIgnoreGlobalTimeDilation = false;
};