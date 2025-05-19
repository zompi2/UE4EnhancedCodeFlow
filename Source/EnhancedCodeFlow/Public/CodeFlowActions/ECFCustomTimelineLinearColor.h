// Copyright (c) 2025 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFActionBase.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveLinearColor.h"
#include "ECFCustomTimelineLinearColor.generated.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UCLASS()
class ENHANCEDCODEFLOW_API UECFCustomTimelineLinearColor : public UECFActionBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

protected:

	TUniqueFunction<void(FLinearColor, float)> TickFunc;
	TUniqueFunction<void(FLinearColor, float, bool)> CallbackFunc;
	TUniqueFunction<void(FLinearColor, float)> CallbackFunc_NoStopped;
	FTimeline MyTimeline;

	FLinearColor CurrentValue = FLinearColor::Black;
	float CurrentTime = 0.f;

	UPROPERTY(Transient)
	UCurveLinearColor* CurveLinearColor = nullptr;

	bool Setup(UCurveLinearColor* InCurveLinearColor, TUniqueFunction<void(FLinearColor, float)>&& InTickFunc, TUniqueFunction<void(FLinearColor, float, bool)>&& InCallbackFunc = nullptr)
	{
		TickFunc = MoveTemp(InTickFunc);
		CallbackFunc = MoveTemp(InCallbackFunc);
		CurveLinearColor = InCurveLinearColor;

		if (TickFunc && CurveLinearColor)
		{
			MyTimeline.AddInterpLinearColor(CurveLinearColor, FOnTimelineLinearColorStatic::CreateUObject(this, &UECFCustomTimelineLinearColor::HandleProgress));
			MyTimeline.SetTimelineFinishedFunc(FOnTimelineEventStatic::CreateUObject(this, &UECFCustomTimelineLinearColor::HandleFinish));
			MyTimeline.PlayFromStart();

			return true;
		}
		else
		{
#if ECF_LOGS
			UE_LOG(LogECF, Error, TEXT("ECF - custom timeline LinearColor failed to start. Are you sure Tick Function and Curve are set properly?"));
#endif
			return false;
		}
	}

	bool Setup(UCurveLinearColor* InCurveLinearColor, TUniqueFunction<void(FLinearColor, float)>&& InTickFunc, TUniqueFunction<void(FLinearColor, float)>&& InCallbackFunc = nullptr)
	{
		CallbackFunc_NoStopped = MoveTemp(InCallbackFunc);
		return Setup(InCurveLinearColor, MoveTemp(InTickFunc), [this](FLinearColor Value, float Time, bool bStopped)
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
		CurrentValue = CurveLinearColor->GetLinearColorValue(CurrentTime);
	}

	void Reset(bool bCallUpdate) override
	{
		MyTimeline.SetPlaybackPosition(0.f, false, false);
		CurrentTime = 0.f;
		CurrentValue = CurveLinearColor->GetLinearColorValue(CurrentTime);

		if (bCallUpdate)
		{
			TickFunc(CurrentValue, CurrentTime);
		}
	}

	void Tick(float DeltaTime) override
	{
#if STATS
		DECLARE_SCOPE_CYCLE_COUNTER(TEXT("CustomTimelineLinearColor - Tick"), STAT_ECFDETAILS_CUSTOMTIMELINELINEARCOLOR, STATGROUP_ECFDETAILS);
#endif

#if ECF_INSIGHT_PROFILING
		TRACE_CPUPROFILER_EVENT_SCOPE("ECF - CustomTimelineLinearColor Tick");
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

	void HandleProgress(FLinearColor Value)
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