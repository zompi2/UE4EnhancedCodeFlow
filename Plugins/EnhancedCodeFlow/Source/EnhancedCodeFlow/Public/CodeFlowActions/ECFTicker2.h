// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFActionBase.h"
#include "ECFTicker2.generated.h"

UCLASS()
class ENHANCEDCODEFLOW_API UECFTicker2 : public UECFActionBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

protected:

	TUniqueFunction<void(float, FECFHandle)> TickFunc;
	float TickingTime;

	float CurrentTime;

	bool Setup(TUniqueFunction<void(float, FECFHandle)>&& InTickFunc, float InTickingTime)
	{
		TickFunc = MoveTemp(InTickFunc);
		TickingTime = InTickingTime;
		if (TickFunc && (TickingTime > 0.f || TickingTime == -1.f))
		{
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
		TickFunc(DeltaTime, HandleId);
		CurrentTime += DeltaTime;
		if (TickingTime > 0.f && CurrentTime >= TickingTime)
		{
			MarkAsFinished();
		}
	}

};
