// Copyright (c) 2022 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFActionBase.h"
#include "ECFDelay.generated.h"

UCLASS()
class ENHANCEDCODEFLOW_API UECFDelay : public UECFActionBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

protected:

	TUniqueFunction<void()> CallbackFunc;
	float DelayTime;
	float CurrentTime;

	bool Setup(float InDelayTime, TUniqueFunction<void()>&& InCallbackFunc)
	{
		DelayTime = InDelayTime;
		CallbackFunc = MoveTemp(InCallbackFunc);

		if (CallbackFunc && DelayTime >= 0)
		{
			if (DelayTime > 0)
			{
				SetMaxActionTime(DelayTime);
			}
			return true;
		}
		else
		{
			ensureMsgf(false, TEXT("ECF - delay failed to start. Are you sure the DelayTime is not negative and Callback Function is set properly?"));
			return false;
		}
	}

	void Init() override
	{
		CurrentTime = 0;
	}

	void Tick(float DeltaTime) override
	{
		CurrentTime += DeltaTime;
		if (CurrentTime > DelayTime)
		{
			Complete();
			MarkAsFinished();
		}
	}

	void Complete() override
	{
		CallbackFunc();
	}
};
