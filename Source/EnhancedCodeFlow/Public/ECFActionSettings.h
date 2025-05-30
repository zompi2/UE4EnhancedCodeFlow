// Copyright (c) 2025 Damian Nowakowski. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ECFActionSettings.generated.h"

USTRUCT(BlueprintType)
struct ENHANCEDCODEFLOW_API FECFActionSettings
{
	GENERATED_BODY()

	FECFActionSettings() :
		TickInterval(0.f),
		FirstDelay(0.f),
		bIgnorePause(false),
		bIgnoreGlobalTimeDilation(false),
		bStartPaused(false),
		Label(TEXT(""))
	{

	}

	FECFActionSettings(float InTickInterval, float InFirstDelay = 0.f, bool InIgnorePause = false, bool InIgnoreTimeDilation = false, bool InStartPaused = false, const FString& InLabel = TEXT("")) :
		TickInterval(InTickInterval),
		FirstDelay(InFirstDelay),
		bIgnorePause(InIgnorePause),
		bIgnoreGlobalTimeDilation(InIgnoreTimeDilation),
		bStartPaused(InStartPaused),
		Label(InLabel)
	{
	}

	UPROPERTY(BlueprintReadWrite, Category = "ECF")
	float TickInterval = 0.f;

	UPROPERTY(BlueprintReadWrite, Category = "ECF")
	float FirstDelay = 0.f;

	UPROPERTY(BlueprintReadWrite, Category = "ECF")
	bool bIgnorePause = false;

	UPROPERTY(BlueprintReadWrite, Category = "ECF")
	bool bIgnoreGlobalTimeDilation = false;

	UPROPERTY(BlueprintReadWrite, Category = "ECF")
	bool bStartPaused = false;

	UPROPERTY(BlueprintReadWrite, Category = "ECF")
	FString Label;
};

#define ECF_TICKINTERVAL(_Interval) FECFActionSettings(_Interval, 0.f, false, false, false, TEXT(""))
#define ECF_DELAYFIRST(_Delay) FECFActionSettings(0.f, _Delay, false, false, false, TEXT(""))
#define ECF_IGNOREPAUSE FECFActionSettings(0.f, 0.f, true, false, false, TEXT(""))
#define ECF_IGNORETIMEDILATION FECFActionSettings(0.f, 0.f, false, true, false, TEXT(""))
#define ECF_IGNOREPAUSEDILATION FECFActionSettings(0.f, 0.f, true, true, false, TEXT(""))
#define ECF_STARTPAUSED FECFActionSettings(0.f, 0.f, false, false, true, TEXT(""))
