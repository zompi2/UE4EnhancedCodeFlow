// Copyright (c) 2025 Damian Nowakowski. All rights reserved.

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

private:

	FVector GetValue()
	{
		switch (BlendFunc)
		{
		case EECFBlendFunc::ECFBlend_Linear:
			return FMath::Lerp(StartValue, StopValue, CurrentTime / Time);
		case EECFBlendFunc::ECFBlend_Cubic:
			return FMath::CubicInterp(StartValue, FVector::ZeroVector, StopValue, FVector::ZeroVector, CurrentTime / Time);
		case EECFBlendFunc::ECFBlend_EaseIn:
			return FMath::Lerp(StartValue, StopValue, FMath::Pow(CurrentTime / Time, BlendExp));
		case EECFBlendFunc::ECFBlend_EaseOut:
			return FMath::Lerp(StartValue, StopValue, FMath::Pow(CurrentTime / Time, 1.f / BlendExp));
		case EECFBlendFunc::ECFBlend_EaseInOut:
			return FMath::InterpEaseInOut(StartValue, StopValue, CurrentTime / Time, BlendExp);
		}
		return FVector::ZeroVector;
	}

protected:

	TUniqueFunction<void(FVector, float)> TickFunc;
	TUniqueFunction<void(FVector, float, bool)> CallbackFunc;
	TUniqueFunction<void(FVector, float)> CallbackFunc_NoStopped;
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
			ensureMsgf(false, TEXT("ECF - Timeline Vector failed to start. Are you sure the Ticking time is greater than 0 and Ticking Function are set properly? /n Remember, that BlendExp must be different than zero and StartValue and StopValue must not be the same!"));
			return false;
		}
	}

	bool Setup(FVector InStartValue, FVector InStopValue, float InTime, TUniqueFunction<void(FVector, float)>&& InTickFunc, TUniqueFunction<void(FVector, float)>&& InCallbackFunc, EECFBlendFunc InBlendFunc, float InBlendExp)
	{
		CallbackFunc_NoStopped = MoveTemp(InCallbackFunc);
		return Setup(InStartValue, InStopValue, InTime, MoveTemp(InTickFunc), [this](FVector FwdValue, float FwdTime, bool bStopped)
		{
			if (CallbackFunc_NoStopped)
			{
				CallbackFunc_NoStopped(FwdValue, FwdTime);
			}
		}, InBlendFunc, InBlendExp);
	}

	void Reset(bool bCallUpdate) override
	{
		CurrentTime = 0.f;
		CurrentValue = GetValue();

		if (bCallUpdate)
		{
			TickFunc(CurrentValue, CurrentTime);
		}
	}

	void Tick(float DeltaTime) override
	{
#if STATS
		DECLARE_SCOPE_CYCLE_COUNTER(TEXT("Timeline Vector - Tick"), STAT_ECFDETAILS_TIMELINEVECTOR, STATGROUP_ECFDETAILS);
#endif
		CurrentTime = FMath::Clamp(CurrentTime + DeltaTime, 0.f, Time);
		CurrentValue = GetValue();
		
		TickFunc(CurrentValue, CurrentTime);

		if (CurrentTime >= Time)
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