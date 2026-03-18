// Copyright (c) 2025 Damian Nowakowski. All rights reserved.

#pragma once

#include "Coroutines/ECFCoroutineActionBase.h"
#include "ECFWaitUntil.generated.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UCLASS()
class ENHANCEDCODEFLOW_API UECFWaitUntil : public UECFCoroutineActionBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

protected:

	TUniqueFunction<bool(float)> Predicate;
	TUniqueFunction<bool()> Predicate_NoDeltaTime;
	float TimeOut = 0.f;
	float OriginTimeOut = 0.f;
	bool bWithTimeOut = false;
	bool bTimedOut = false;

	bool Setup(TUniqueFunction<bool(float)>&& InPredicate, float InTimeOut)
	{
		Predicate = MoveTemp(InPredicate);

		if (Predicate)
		{
			if (Predicate(0.f))
			{
				CoroutineHandle.resume();
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
			}
			return true;
		}
		else
		{
#if ECF_LOGS
			UE_LOG(LogECF, Error, TEXT("ECF Coroutine [%s] - Wait Until failed to start. Are you sure the Predicate is set properly?"), *Settings.Label);
#endif
			return false;
		}
	}

	bool Setup(TUniqueFunction<bool()>&& InPredicate, float InTimeOut)
	{
		Predicate_NoDeltaTime = MoveTemp(InPredicate);
		if (Predicate_NoDeltaTime)
		{
			return Setup([this](float DeltaTime)
			{
				return Predicate_NoDeltaTime();
			}, InTimeOut);
		}
		else
		{
#if ECF_LOGS
			UE_LOG(LogECF, Error, TEXT("ECF Coroutine [%s] - Wait Until failed to start. Are you sure the Predicate is set properly?"), *Settings.Label);
#endif
			return false;
		}
	}

	bool Reset(bool bCallUpdate) override
	{
		if (bWithTimeOut)
		{
			TimeOut = OriginTimeOut;
		}
		return true;
	}

	void Tick(float DeltaTime) override
	{
#if STATS
		DECLARE_SCOPE_CYCLE_COUNTER(TEXT("WaitUntil - Tick"), STAT_ECFDETAILS_WAITUNTIL, STATGROUP_ECFDETAILS);
#endif

#if ECF_INSIGHT_PROFILING
		TRACE_CPUPROFILER_EVENT_SCOPE("ECF - WaitUntil Tick");
#endif

		if (bWithTimeOut)
		{
			TimeOut -= DeltaTime;
			if (TimeOut <= 0.f)
			{
				bTimedOut = true;
				MarkAsFinished();
				Complete(false);
				return;
			}
		}

		if (Predicate(DeltaTime))
		{
			MarkAsFinished();
			Complete(false);
		}
	}

	void Complete(bool bStopped) override
	{
		CoroutineHandle.promise().bTimedOut = bTimedOut;
		CoroutineHandle.promise().bStopped = bStopped;
		CoroutineHandle.resume();
	}
};

ECF_PRAGMA_ENABLE_OPTIMIZATION