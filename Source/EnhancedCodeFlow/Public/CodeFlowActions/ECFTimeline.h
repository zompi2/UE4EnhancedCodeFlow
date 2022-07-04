// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFActionBase.h"
#include "ECFTypes.h"
#include "ECFTimeline.generated.h"

UCLASS()
class ENHANCEDCODEFLOW_API UECFTimeline : public UECFActionBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

protected:

	TUniqueFunction<void(float, float)> TickFunc;
	TUniqueFunction<void(float, float)> CallbackFunc;
	float StartValue;
	float StopValue;
	float Time;
	EECFBlendFunc BlendFunc;
	float BlendExp;

	float CurrentTime;
	float CurrentValue;

	bool Setup(float InStartValue, float InStopValue, float InTime, TUniqueFunction<void(float, float)>&& InTickFunc, TUniqueFunction<void(float, float)>&& InCallbackFunc, EECFBlendFunc InBlendFunc, float InBlendExp)
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
			return false;
		}
	}

	void Tick(float DeltaTime) override
	{
		CurrentTime = FMath::Clamp(CurrentTime + DeltaTime, 0.f, Time);

		switch (BlendFunc)
		{
		case EECFBlendFunc::ECFBlend_Linear:
			CurrentValue = FMath::Lerp(StartValue, StopValue, CurrentTime / Time);
			break;
		case EECFBlendFunc::ECFBlend_Cubic:
			CurrentValue = FMath::CubicInterp(StartValue, 0.f, StopValue, 0.f, CurrentTime / Time);
			break;
		case EECFBlendFunc::ECFBlend_EaseIn:
			CurrentValue = FMath::Lerp(StartValue, StopValue, FMath::Pow(CurrentTime / Time, BlendExp));
			break;
		case EECFBlendFunc::ECFBlend_EaseOut:
			CurrentValue = FMath::Lerp(StartValue, StopValue, FMath::Pow(CurrentTime / Time, 1.f / BlendExp));
			break;
		case EECFBlendFunc::ECFBlend_EaseInOut:
			CurrentValue = FMath::InterpEaseInOut(StartValue, StopValue, CurrentTime / Time, BlendExp);
			break;
		}

		TickFunc(CurrentValue, CurrentTime);

		if ((StopValue > StartValue && CurrentValue >= StopValue) || (StopValue < StartValue && CurrentValue <= StopValue))
		{
			Complete();
			MarkAsFinished();
		}
	}

	void Complete() override
	{
		if (CallbackFunc)
		{
			CallbackFunc(CurrentValue, CurrentTime);
		}
	}
};
