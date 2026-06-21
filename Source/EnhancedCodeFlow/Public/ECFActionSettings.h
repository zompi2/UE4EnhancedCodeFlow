// Copyright (c) 2026 Damian Nowakowski. All rights reserved.

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

	FECFActionSettings& WithTickInterval(float InTickInterval)
	{
		TickInterval = InTickInterval;
		return *this;
	}

	UPROPERTY(BlueprintReadWrite, Category = "ECF")
	float FirstDelay = 0.f;

	FECFActionSettings& WithFirstDelay(float InFirstDelay)
	{
		FirstDelay = InFirstDelay;
		return *this;
	}

	UPROPERTY(BlueprintReadWrite, Category = "ECF")
	bool bIgnorePause = false;

	FECFActionSettings& WithIgnorePause()
	{
		bIgnorePause = true;
		return *this;
	}

	UPROPERTY(BlueprintReadWrite, Category = "ECF")
	bool bIgnoreGlobalTimeDilation = false;

	FECFActionSettings& WithIgnoreGlobalTimeDilation()
	{
		bIgnoreGlobalTimeDilation = true;
		return *this;
	}

	UPROPERTY(BlueprintReadWrite, Category = "ECF")
	bool bStartPaused = false;

	FECFActionSettings& WithStartPaused()
	{
		bStartPaused = true;
		return *this;
	}

	UPROPERTY(BlueprintReadWrite, Category = "ECF")
	FString Label;

	FECFActionSettings& WithLabel(const FString& InLabel)
	{
		Label = InLabel;
		return *this;
	}
};

#define ECF_TICKINTERVAL(_Interval) FECFActionSettings().WithTickInterval(_Interval)
#define ECF_DELAYFIRST(_Delay) FECFActionSettings().WithFirstDelay(_Delay)
#define ECF_IGNOREPAUSE FECFActionSettings().WithIgnorePause()
#define ECF_IGNORETIMEDILATION FECFActionSettings().WithIgnoreGlobalTimeDilation()
#define ECF_IGNOREPAUSEDILATION ECFActionSettings().WithIgnorePause().WithIgnoreGlobalTimeDilation()
#define ECF_STARTPAUSED ECFActionSettings().WithStartPaused()
#define ECF_LABEL(_Label) ECFActionSettings().WithLabel(_Label)
