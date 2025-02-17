// Copyright (c) 2024 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFActionBase.h"
#include "ECFTicker.generated.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UCLASS()
class ENHANCEDCODEFLOW_API UECFTicker : public UECFActionBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

protected:

	TUniqueFunction<void(float, FECFHandle)> TickFunc;
	TUniqueFunction<void(float)> TickFunc_NoHandle;
	TUniqueFunction<void(bool)> CallbackFunc;
	TUniqueFunction<void()> CallbackFunc_NoStopped;
	float TickingTime = 0.f;
	float CurrentTime = 0.f;

	// Handle - yes, bStopped - yes
	bool Setup(float InTickingTime, TUniqueFunction<void(float, FECFHandle)>&& InTickFunc, TUniqueFunction<void(bool)>&& InCallbackFunc = nullptr)
	{
		TickingTime = InTickingTime;
		TickFunc = MoveTemp(InTickFunc);
		CallbackFunc = MoveTemp(InCallbackFunc);

		if (TickFunc && (TickingTime > 0.f || TickingTime == -1.f))
		{
			if (TickingTime > 0.f)
			{
				SetMaxActionTime(TickingTime);
			}

			CurrentTime = 0.f;
			return true;
		}
		else
		{
			ensureMsgf(false, TEXT("ECF - Ticker(2) failed to start. Are you sure the Ticking time and Ticking Function are set properly?"));
			return false;
		}
	}

	// Handle - yes, bStopped - no
	bool Setup(float InTickingTime, TUniqueFunction<void(float, FECFHandle)>&& InTickFunc, TUniqueFunction<void()>&& InCallbackFunc = nullptr)
	{
		CallbackFunc_NoStopped = MoveTemp(InCallbackFunc);
		return Setup(InTickingTime, MoveTemp(InTickFunc), [this](bool bStopped)
		{
			if (CallbackFunc_NoStopped)
			{
				CallbackFunc_NoStopped();
			}
		});
	}

	// Handle - no, bStopped - yes
	bool Setup(float InTickingTime, TUniqueFunction<void(float)>&& InTickFunc, TUniqueFunction<void(bool)>&& InCallbackFunc = nullptr)
	{
		TickFunc_NoHandle = MoveTemp(InTickFunc);
		return Setup(InTickingTime, [this](float DeltaTime, FECFHandle Handle) 
		{
			if (TickFunc_NoHandle)
			{
				TickFunc_NoHandle(DeltaTime);
			}
		}, MoveTemp(InCallbackFunc));
	}

	// Handle - no, bStopped - no
	bool Setup(float InTickingTime, TUniqueFunction<void(float)>&& InTickFunc, TUniqueFunction<void()>&& InCallbackFunc = nullptr)
	{
		CallbackFunc_NoStopped = MoveTemp(InCallbackFunc);
		TickFunc_NoHandle = MoveTemp(InTickFunc);
		return Setup(InTickingTime, [this](float DeltaTime, FECFHandle Handle)
		{
			if (TickFunc_NoHandle)
			{
				TickFunc_NoHandle(DeltaTime);
			}
		}, [this](bool bStopped)
		{
			if (CallbackFunc_NoStopped)
			{
				CallbackFunc_NoStopped();
			}
		});
	}

	void Init() override
	{
		CurrentTime = 0.f;
	}

	void Reset(bool bCallUpdate) override
	{
		CurrentTime = 0.f;
		// Can't call update, because the DeltaTime is unknown.
		// Updating with DT=0 will cause confusion.
		// The app just need to wait for it's next update.
	}

	void Tick(float DeltaTime) override
	{
#if STATS
		DECLARE_SCOPE_CYCLE_COUNTER(TEXT("Ticker - Tick"), STAT_ECFDETAILS_TICKER, STATGROUP_ECFDETAILS);
#endif
		TickFunc(DeltaTime, HandleId);
		CurrentTime += DeltaTime;

		if (TickingTime > 0.f && CurrentTime >= TickingTime)
		{
			Complete(false);
			MarkAsFinished();
		}
	}

	void Complete(bool bStopped) override
	{
		if (CallbackFunc)
		{
			CallbackFunc(bStopped);
		}
	}
};

ECF_PRAGMA_ENABLE_OPTIMIZATION