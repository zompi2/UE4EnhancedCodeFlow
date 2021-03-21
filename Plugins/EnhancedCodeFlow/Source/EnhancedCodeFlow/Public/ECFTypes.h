// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ECFTypes.generated.h"

// Possible blend functions for ECF system.
UENUM(BlueprintType)
enum class EECFBlendFunc : uint8
{
	ECFBlend_Linear,
	ECFBlend_Cubic,
	ECFBlend_EaseIn,
	ECFBlend_EaseOut,
	ECFBlend_EaseInOut
};