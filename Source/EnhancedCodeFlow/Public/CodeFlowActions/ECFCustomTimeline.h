// Copyright (c) 2023 Damian Nowakowski. All rights reserved.

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
	TUniqueFunction<void(float, float)> CallbackFunc;
	FTimeline MyTimeline;

	float CurrentValue;
	float CurrentTime;

	UPROPERTY(Transient)
	UCurveFloat* CurveFloat;

	bool Setup(UCurveFloat* InCurveFloat, TUniqueFunction<void(float, float)>&& InTickFunc, TUniqueFunction<void(float, float)>&& InCallbackFunc = nullptr)
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

	void Tick(float DeltaTime) override
	{
		MyTimeline.TickTimeline(DeltaTime);
	}

	void Complete() override
	{
		if (CallbackFunc)
		{
			CallbackFunc(CurrentValue, CurrentTime);
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
			Complete();
		}
		MarkAsFinished();
	}
};

ECF_PRAGMA_ENABLE_OPTIMIZATION