// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFActionBase.h"
#include "ECFTimeline.generated.h"

UCLASS()
class ENHANCEDCODEFLOW_API UECFTimeline : public UECFActionBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

protected:

	TUniqueFunction<void(float)> TickFunc;
	TUniqueFunction<void(float)> Func;
	float StartTime;
	float StopTime;
	bool bLoop;

	float CurrentTime;
	float TimeFlowDir = 1.f;

	bool Setup(TUniqueFunction<void(float)>&& InTickFunc, TUniqueFunction<void(float)>&& InFunc, float InStartTime, float InStopTime, bool bInLoop)
	{
		TickFunc = MoveTemp(InTickFunc);
		Func = MoveTemp(InFunc);
		StartTime = InStartTime;
		StopTime = InStopTime;
		bLoop = bInLoop;
		if (TickFunc && Func && FMath::IsNearlyEqual(StartTime, StopTime) == false)
		{
			CurrentTime = StartTime;
			TimeFlowDir = (StopTime > StartTime) ? 1.f : -1.f;
			return true;
		}
		else
		{
			return false;
		}
	}

	void Tick(float DeltaTime) override
	{
		CurrentTime += DeltaTime * TimeFlowDir;
		if (TimeFlowDir == 1.f)
		{
			if (CurrentTime >= StopTime)
			{
				if (bLoop)
				{
					while (CurrentTime >= StopTime)
					{
						CurrentTime = StartTime + (CurrentTime - StopTime);
					}
				}
				else
				{
					Func(StopTime);
					MarkAsFinished();
					return;
				}
			}
		}
		else
		{
			if (CurrentTime <= StopTime)
			{
				if (bLoop)
				{
					while (CurrentTime <= StopTime)
					{
						CurrentTime = StartTime - (StopTime - CurrentTime);
					}
				}
				else
				{
					Func(StopTime);
					MarkAsFinished();
					return;
				}
			}
		}
		Func(CurrentTime);
	}
};
