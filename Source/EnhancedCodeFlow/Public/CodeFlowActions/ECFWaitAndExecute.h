// Copyright (c) 2025 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFActionBase.h"
#include "ECFWaitAndExecute.generated.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UCLASS()
class ENHANCEDCODEFLOW_API UECFWaitAndExecute : public UECFActionBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

protected:

	TUniqueFunction<bool(float)> Predicate;
	TUniqueFunction<bool()> Predicate_NoDeltaTime;
	TUniqueFunction<void(bool, bool)> Func;
	TUniqueFunction<void(bool)> Func_NoStopped;
	TUniqueFunction<void()> Func_NoTimeOut_NoStopped;

	float TimeOut = 0.f;
	float OriginTimeOut = 0.f;
	bool bWithTimeOut = false;
	bool bTimedOut = false;

	// Predicate DeltaTime - YES
	// Callback bStopped - YES
	// Callback bTimedOut - YES
	bool Setup(TUniqueFunction<bool(float)>&& InPredicate, TUniqueFunction<void(bool, bool)>&& InFunc, float InTimeOut)
	{
		Predicate = MoveTemp(InPredicate);
		Func = MoveTemp(InFunc);

		if (Predicate && Func)
		{
			if (Predicate(0.f))
			{
				Func(false, false);
				return false;
			}
			if (InTimeOut > 0.f)
			{
				bWithTimeOut = true;
				bTimedOut = false;
				TimeOut = InTimeOut;
				OriginTimeOut = InTimeOut;
				SetMaxActionTime(TimeOut);
			}
			else
			{
				bWithTimeOut = false;
				bTimedOut = false;
			}
			return true;
		}
		else
		{
			ensureMsgf(false, TEXT("ECF - Wait and Execute failed to start. Are you sure the Predicate and Function are set properly?"));
			return false;
		}
	}

	// Predicate DeltaTime - NO
	// Callback bStopped - YES
	// Callback bTimedOut - YES
	bool Setup(TUniqueFunction<bool()>&& InPredicate, TUniqueFunction<void(bool, bool)>&& InFunc, float InTimeOut)
	{
		Predicate_NoDeltaTime = MoveTemp(InPredicate);
		if (Predicate_NoDeltaTime)
		{
			return Setup([this](float DeltaTime)
			{
				return Predicate_NoDeltaTime();
			}, MoveTemp(InFunc), InTimeOut);
		}
		else
		{
			ensureMsgf(false, TEXT("ECF - Wait and Execute failed to start. Are you sure the Function is set properly?"));
			return false;
		}
	}

	// Predicate DeltaTime - YES
	// Callback bStopped - NO
	// Callback bTimedOut - YES
	bool Setup(TUniqueFunction<bool(float)>&& InPredicate, TUniqueFunction<void(bool)>&& InFunc, float InTimeOut)
	{
		Func_NoStopped = MoveTemp(InFunc);
		if (Func_NoStopped)
		{
			return Setup(MoveTemp(InPredicate), [this](bool bTimeOut, bool bStopped)
			{
				Func_NoStopped(bTimeOut);
			}, InTimeOut);
		}
		else
		{
			ensureMsgf(false, TEXT("ECF - Wait and Execute failed to start. Are you sure the Function is set properly?"));
			return false;
		}
	}

	// Predicate DeltaTime - NO
	// Callback bStopped - NO
	// Callback bTimedOut - YES
	bool Setup(TUniqueFunction<bool()>&& InPredicate, TUniqueFunction<void(bool)>&& InFunc, float InTimeOut)
	{
		Func_NoStopped = MoveTemp(InFunc);
		Predicate_NoDeltaTime = MoveTemp(InPredicate);
		if (Func_NoStopped && Predicate_NoDeltaTime)
		{
			return Setup([this](float DeltaTime)
				{
					return Predicate_NoDeltaTime();
				},
				[this](bool bTimeOut, bool bStopped)
				{
					Func_NoStopped(bTimeOut);
				}, InTimeOut);
		}
		else
		{
			ensureMsgf(false, TEXT("ECF - Wait and Execute failed to start. Are you sure the Function is set properly?"));
			return false;
		}
	}

	// Predicate DeltaTime - YES
	// Callback bStopped - NO
	// Callback bTimedOut - NO
	bool Setup(TUniqueFunction<bool(float)>&& InPredicate, TUniqueFunction<void()>&& InFunc, float InTimeOut)
	{
		Func_NoTimeOut_NoStopped = MoveTemp(InFunc);
		if (Func_NoTimeOut_NoStopped)
		{
			return Setup(MoveTemp(InPredicate), [this](bool bTimeOut, bool bStopped)
			{
				Func_NoTimeOut_NoStopped();
			}, InTimeOut);
		}
		else
		{
			ensureMsgf(false, TEXT("ECF - Wait and Execute failed to start. Are you sure the Function is set properly?"));
			return false;
		}
	}

	// Predicate DeltaTime - NO
	// Callback bStopped - NO
	// Callback bTimedOut - NO
	bool Setup(TUniqueFunction<bool()>&& InPredicate, TUniqueFunction<void()>&& InFunc, float InTimeOut)
	{
		Func_NoTimeOut_NoStopped = MoveTemp(InFunc);
		Predicate_NoDeltaTime = MoveTemp(InPredicate);
		if (Func_NoTimeOut_NoStopped && Predicate_NoDeltaTime)
		{
			return Setup([this](float DeltaTime)
			{
				return Predicate_NoDeltaTime();
			},
			[this](bool bTimeOut, bool bStopped)
			{
				Func_NoTimeOut_NoStopped();
			}, InTimeOut);
		}
		else
		{
			ensureMsgf(false, TEXT("ECF - Wait and Execute failed to start. Are you sure the Function is set properly?"));
			return false;
		}
	}

	void Reset(bool bCallUpdate) override
	{
		if (bWithTimeOut)
		{
			TimeOut = OriginTimeOut;
		}
	}

	void Tick(float DeltaTime) override 
	{
#if STATS
		DECLARE_SCOPE_CYCLE_COUNTER(TEXT("WaitAndExecute - Tick"), STAT_ECFDETAILS_WAITANDEXECUTE, STATGROUP_ECFDETAILS);
#endif
		if (bWithTimeOut)
		{
			TimeOut -= DeltaTime;
			if (TimeOut <= 0.f)
			{
				bTimedOut = true;
				Complete(false);
				MarkAsFinished();
				return;
			}
		}

		if (Predicate(DeltaTime))
		{
			Complete(false);
			MarkAsFinished();
		}
	}

	void Complete(bool bStopped) override
	{
		Func(bTimedOut, bStopped);
	}
};

ECF_PRAGMA_ENABLE_OPTIMIZATION