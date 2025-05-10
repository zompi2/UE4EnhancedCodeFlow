// Copyright (c) 2025 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFActionBase.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveVector.h"
#include "ECFCustomTimelineVector.generated.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UCLASS()
class ENHANCEDCODEFLOW_API UECFCustomTimelineVector : public UECFActionBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

protected:

	TUniqueFunction<void(FVector, float)> TickFunc;
	TUniqueFunction<void(FVector, float, bool)> CallbackFunc;
	TUniqueFunction<void(FVector, float)> CallbackFunc_NoStopped;
	FTimeline MyTimeline;

	FVector CurrentValue = FVector::ZeroVector;
	float CurrentTime = 0.f;

	UPROPERTY(Transient)
	UCurveVector* CurveVector = nullptr;

	bool Setup(UCurveVector* InCurveVector, TUniqueFunction<void(FVector, float)>&& InTickFunc, TUniqueFunction<void(FVector, float, bool)>&& InCallbackFunc = nullptr)
	{
		TickFunc = MoveTemp(InTickFunc);
		CallbackFunc = MoveTemp(InCallbackFunc);
		CurveVector = InCurveVector;

		if (TickFunc && CurveVector)
		{
			MyTimeline.AddInterpVector(CurveVector, FOnTimelineVectorStatic::CreateUObject(this, &UECFCustomTimelineVector::HandleProgress));
			MyTimeline.SetTimelineFinishedFunc(FOnTimelineEventStatic::CreateUObject(this, &UECFCustomTimelineVector::HandleFinish));
			MyTimeline.PlayFromStart();

			return true;
		}
		else
		{
#if ECF_LOGS
			UE_LOG(LogECF, Error, TEXT("ECF - custom timeline vector failed to start. Are you sure Tick Function and Curve are set properly?"));
#endif
			return false;
		}
	}

	bool Setup(UCurveVector* InCurveVector, TUniqueFunction<void(FVector, float)>&& InTickFunc, TUniqueFunction<void(FVector, float)>&& InCallbackFunc = nullptr)
	{
		CallbackFunc_NoStopped = MoveTemp(InCallbackFunc);
		return Setup(InCurveVector, MoveTemp(InTickFunc), [this](FVector Value, float Time, bool bStopped)
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
		CurrentValue = CurveVector->GetVectorValue(CurrentTime);
	}

	void Reset(bool bCallUpdate) override
	{
		MyTimeline.SetPlaybackPosition(0.f, false, false);
		CurrentTime = 0.f;
		CurrentValue = CurveVector->GetVectorValue(CurrentTime);

		if (bCallUpdate)
		{
			TickFunc(CurrentValue, CurrentTime);
		}
	}

	void Tick(float DeltaTime) override
	{
#if STATS
		DECLARE_SCOPE_CYCLE_COUNTER(TEXT("CustomTimelineVector - Tick"), STAT_ECFDETAILS_CUSTOMTIMELINEVECTOR, STATGROUP_ECFDETAILS);
#endif

#if ECF_INSIGHT_PROFILING
		TRACE_CPUPROFILER_EVENT_SCOPE("ECF - CustomTimelineVector Tick");
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

	void HandleProgress(FVector Value)
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