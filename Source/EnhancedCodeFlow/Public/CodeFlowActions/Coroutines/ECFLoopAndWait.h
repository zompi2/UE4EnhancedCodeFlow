// Copyright (c) 2026 Damian Nowakowski. All rights reserved.

#pragma once

#include "Coroutines/ECFCoroutineActionBase.h"
#include "ECFLoopAndWait.generated.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UCLASS()
class ENHANCEDCODEFLOW_API UECFLoopAndWait : public UECFCoroutineActionBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

protected:

	TUniqueFunction<bool()> Predicate;
	TUniqueFunction<void(float)> TickFunc;

	float TimeOut = 0.f;
	float OriginTimeOut = 0.f;
	bool bWithTimeOut = false;
	bool bTimedOut = false;

	bool Setup(TUniqueFunction<bool()>&& InPredicate, TUniqueFunction<void(float)>&& InTickFunc, float InTimeOut)
	{
		Predicate = MoveTemp(InPredicate);
		TickFunc = MoveTemp(InTickFunc);

		if (Predicate && TickFunc)
		{
			if (Predicate() == false)
			{
				// Coroutine will resume after Setup has failed
				return false;
			}
			if (InTimeOut > 0.f)
			{
				bWithTimeOut = true;
				TimeOut = InTimeOut;
				OriginTimeOut = InTimeOut;
				bTimedOut = false;
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
#if ECF_LOGS
			UE_LOG(LogECF, Error, TEXT("ECF Coroutine [%s] - Loop and Wait failed to start. Are you sure the Predicate and TickFunction are set properly?"), *Settings.Label);
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
		DECLARE_SCOPE_CYCLE_COUNTER(TEXT("LoopAndWait - Tick"), STAT_ECFDETAILS_LOOPANDWAIT, STATGROUP_ECFDETAILS);
#endif

#if ECF_INSIGHT_PROFILING
		TRACE_CPUPROFILER_EVENT_SCOPE("ECF - LoopAndWait Tick");
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

		if (Predicate())
		{
			TickFunc(DeltaTime);
		}
		else
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