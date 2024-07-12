// Copyright (c) 2024 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFActionBase.h"
#include "ECFTypes.h"
#include "ECFTimelineVector.generated.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UCLASS()
class ENHANCEDCODEFLOW_API UECFTimelineVector : public UECFActionBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

protected:

	TUniqueFunction<void(FVector, float)> TickFunc;
	TUniqueFunction<void(FVector, float, bool)> CallbackFunc;
	FVector StartValue;
	FVector StopValue;
	float Time;
	EECFBlendFunc BlendFunc;
	float BlendExp;

	float CurrentTime;
	FVector CurrentValue;

	bool Setup(FVector InStartValue, FVector InStopValue, float InTime, TUniqueFunction<void(FVector, float)>&& InTickFunc, TUniqueFunction<void(FVector, float, bool)>&& InCallbackFunc, EECFBlendFunc InBlendFunc, float InBlendExp)
	{
		StartValue = InStartValue;
		StopValue = InStopValue;
		Time = InTime;

		TickFunc = MoveTemp(InTickFunc);
		CallbackFunc = MoveTemp(InCallbackFunc);

		BlendFunc = InBlendFunc;
		BlendExp = InBlendExp;

		if (TickFunc && Time > 0 && BlendExp != 0 && StartValue != StopValue)
		{
			SetMaxActionTime(Time);
			CurrentTime = 0.f;
			return true;
		}
		else
		{
			ensureMsgf(false, TEXT("ECF - Timeline failed to start. Are you sure the Ticking time is greater than 0 and Ticking Function are set properly? /n Remember, that BlendExp must be different than zero and StartValue and StopValue must not be the same!"));
			return false;
		}
	}

	void Tick(float DeltaTime) override
	{
#if STATS
		DECLARE_SCOPE_CYCLE_COUNTER(TEXT("Timeline - Tick"), STAT_ECFDETAILS_TIMELINE, STATGROUP_ECFDETAILS);
#endif
		CurrentTime = FMath::Clamp(CurrentTime + DeltaTime, 0.f, Time);

		const float LerpValue = CurrentTime / Time;

		switch (BlendFunc)
		{
		case EECFBlendFunc::ECFBlend_Linear:
			CurrentValue = FMath::Lerp(StartValue, StopValue, LerpValue);
			break;
		case EECFBlendFunc::ECFBlend_Cubic:
			CurrentValue = FMath::CubicInterp(StartValue, FVector::ZeroVector, StopValue, FVector::ZeroVector, LerpValue);
			break;
		case EECFBlendFunc::ECFBlend_EaseIn:
			CurrentValue = FMath::Lerp(StartValue, StopValue, FMath::Pow(LerpValue, BlendExp));
			break;
		case EECFBlendFunc::ECFBlend_EaseOut:
			CurrentValue = FMath::Lerp(StartValue, StopValue, FMath::Pow(LerpValue, 1.f / BlendExp));
			break;
		case EECFBlendFunc::ECFBlend_EaseInOut:
			CurrentValue = FMath::InterpEaseInOut(StartValue, StopValue, LerpValue, BlendExp);
			break;
		}

		TickFunc(CurrentValue, CurrentTime);

		if (LerpValue >= 1.f)
		{
			Complete(false);
			MarkAsFinished();
		}
	}

	void Complete(bool bStopped) override
	{
		if (CallbackFunc)
		{
			CallbackFunc(CurrentValue, CurrentTime, bStopped);
		}
	}
};

ECF_PRAGMA_ENABLE_OPTIMIZATION