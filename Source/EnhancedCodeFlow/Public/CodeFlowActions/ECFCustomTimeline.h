// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFActionBase.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"
#include "ECFCustomTimeline.generated.h"

UCLASS()
class ENHANCEDCODEFLOW_API UECFCustomTimeline : public UECFActionBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

protected:

	TUniqueFunction<void(float)> TickFunc;
	TUniqueFunction<void(float)> CallbackFunc;
	FTimeline MyTimeline;
	float CurrentValue;

	UPROPERTY(Transient)
	UCurveFloat* CurveFloat;

	bool Setup(UCurveFloat* InCurveFloat, TUniqueFunction<void(float)>&& InTickFunc, TUniqueFunction<void(float)>&& InCallbackFunc = nullptr)
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
			return false;
		}
	}

	void Tick(float DeltaTime) override
	{
		MyTimeline.TickTimeline(DeltaTime);
	}

private:

	UFUNCTION()
	void HandleProgress(float Value)
	{
		CurrentValue = Value;
		TickFunc(CurrentValue);
	}

	UFUNCTION()
	void HandleFinish()
	{
		if (CallbackFunc)
		{
			CallbackFunc(CurrentValue);
		}
		MarkAsFinished();
	}
};
