// Copyright (c) 2025 Damian Nowakowski. All rights reserved.

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
			MyTimeline.AddInterpFloat(CurveFloat, FOnTimelineFloatStatic::CreateUObject(this, &UECFCustomTimeline::HandleProgress));
			MyTimeline.SetTimelineFinishedFunc(FOnTimelineEventStatic::CreateUObject(this, &UECFCustomTimeline::HandleFinish));
			MyTimeline.PlayFromStart();

			return true;
		}
		else
		{
#if ECF_LOGS
			UE_LOG(LogECF, Error, TEXT("ECF - custom timeline failed to start. Are you sure Tick Function and Curve are set properly?"));
#endif
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

	void Init() override
	{
		CurrentTime = 0.f;
		CurrentValue = CurveFloat->GetFloatValue(CurrentTime);
	}

	void Reset(bool bCallUpdate) override
	{
		MyTimeline.SetPlaybackPosition(0.f, false, false);
		CurrentTime = 0.f;
		CurrentValue = CurveFloat->GetFloatValue(CurrentTime);

		if (bCallUpdate)
		{
			TickFunc(CurrentValue, CurrentTime);
		}
	}

	void Tick(float DeltaTime) override
	{
#if STATS
		DECLARE_SCOPE_CYCLE_COUNTER(TEXT("CustomTimeline - Tick"), STAT_ECFDETAILS_CUSTOMTIMELINE, STATGROUP_ECFDETAILS);
#endif

#if ECF_INSIGHT_PROFILING
		TRACE_CPUPROFILER_EVENT_SCOPE("ECF - CustomTimeline Tick");
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

	void HandleProgress(float Value)
	{
		CurrentValue = Value;
		CurrentTime = MyTimeline.GetPlaybackPosition();
		if (HasValidOwner())
		{
			TickFunc(CurrentValue, CurrentTime);
		}
	}

	void HandleFinish()
	{
		MarkAsFinished();
		if (HasValidOwner())
		{
			Complete(false);
		}
	}
};

ECF_PRAGMA_ENABLE_OPTIMIZATION