// Copyright (c) 2024 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFActionBase.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"
#include "ECFCustomTimeline.generated.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UCLASS()
class ENHANCEDCODEFLOW_API UECFCustomTimeline : public UECFActionBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

protected:

	TUniqueFunction<void(float, float)> TickFunc;
	TUniqueFunction<void(float, float, bool)> CallbackFunc;
	TUniqueFunction<void(float, float)> CallbackFunc_NoStopped;
	FTimeline MyTimeline;

	float CurrentValue = 0.f;
	float CurrentTime = 0.f;

	UPROPERTY(Transient)
	UCurveFloat* CurveFloat = nullptr;

	bool Setup(UCurveFloat* InCurveFloat, TUniqueFunction<void(float, float)>&& InTickFunc, TUniqueFunction<void(float, float, bool)>&& InCallbackFunc = nullptr)
	{
		TickFunc = MoveTemp(InTickFunc);
		CallbackFunc = MoveTemp(InCallbackFunc);
		CurveFloat = InCurveFloat;

		if (TickFunc && CurveFloat)
		{
			FOnTimelineFloat ProgressFunction;
			ProgressFunction.BindUFunction(this, FName("HandleProgress"));
			MyTimeline.AddInterpFloat(CurveFloat, ProgressFunction);

			FOnTimelineEvent FinishFunction;
			FinishFunction.BindUFunction(this, FName("HandleFinish"));
			MyTimeline.SetTimelineFinishedFunc(FinishFunction);

			MyTimeline.PlayFromStart();

			return true;
		}
		else
		{
			ensureMsgf(false, TEXT("ECF - custom timeline failed to start. Are you sure Tick Function and Curve are set properly?"));
			return false;
		}
	}

	bool Setup(UCurveFloat* InCurveFloat, TUniqueFunction<void(float, float)>&& InTickFunc, TUniqueFunction<void(float, float)>&& InCallbackFunc = nullptr)
	{
		CallbackFunc_NoStopped = MoveTemp(InCallbackFunc);
		return Setup(InCurveFloat, MoveTemp(InTickFunc), [this](float Value, float Time, bool bStopped)
		{
			if (CallbackFunc_NoStopped)
			{
				CallbackFunc_NoStopped(Value, Time);
			}
		});
	}

	void Tick(float DeltaTime) override
	{
#if STATS
		DECLARE_SCOPE_CYCLE_COUNTER(TEXT("CustomTimeline - Tick"), STAT_ECFDETAILS_CUSTOMTIMELINE, STATGROUP_ECFDETAILS);
#endif
		MyTimeline.TickTimeline(DeltaTime);
	}

	void Complete(bool bStopped) override
	{
		if (CallbackFunc)
		{
			CallbackFunc(CurrentValue, CurrentTime, bStopped);
		}
	}

private:

	UFUNCTION()
	void HandleProgress(float Value)
	{
		CurrentValue = Value;
		CurrentTime = MyTimeline.GetPlaybackPosition();
		if (HasValidOwner())
		{
			TickFunc(CurrentValue, CurrentTime);
		}
	}

	UFUNCTION()
	void HandleFinish()
	{
		if (HasValidOwner())
		{
			Complete(false);
		}
		MarkAsFinished();
	}
};

ECF_PRAGMA_ENABLE_OPTIMIZATION