// Copyright (c) 2025 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFActionBase.h"
#include "ECFWhileTrueExecute.generated.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UCLASS()
class ENHANCEDCODEFLOW_API UECFWhileTrueExecute : public UECFActionBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

protected:

	TUniqueFunction<bool()> Predicate;
	TUniqueFunction<void(float)> TickFunc;
	TUniqueFunction<void(bool, bool)> CompleteFunc;
	TUniqueFunction<void(bool)> CompleteFunc_NoStopped;
	TUniqueFunction<void()> CompleteFunc_NoTimeOut_NoStopped;

	float TimeOut = 0.f;
	float OriginTimeOut = 0.f;
	bool bWithTimeOut = false;
	bool bTimedOut = false;

	bool Setup(TUniqueFunction<bool()>&& InPredicate, TUniqueFunction<void(float)>&& InTickFunc, TUniqueFunction<void(bool, bool)>&& InCompleteFunc, float InTimeOut)
	{
		Predicate = MoveTemp(InPredicate);
		TickFunc = MoveTemp(InTickFunc);
		CompleteFunc = MoveTemp(InCompleteFunc);

		if (Predicate && TickFunc)
		{
			if (Predicate() == false)
			{
				if (CompleteFunc)
				{
					CompleteFunc(false, false);
				}
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
			UE_LOG(LogECF, Error, TEXT("ECF - While True Execute failed to start. Are you sure the Predicate and Function are set properly?"));
#endif
			return false;
		}
	}

	bool Setup(TUniqueFunction<bool()>&& InPredicate, TUniqueFunction<void(float)>&& InTickFunc, TUniqueFunction<void(bool)>&& InCompleteFunc, float InTimeOut)
	{
		CompleteFunc_NoStopped = MoveTemp(InCompleteFunc);
		return Setup(MoveTemp(InPredicate), MoveTemp(InTickFunc), [this](bool bTimeOut, bool bStopped)
		{
			if (CompleteFunc_NoStopped)
			{
				CompleteFunc_NoStopped(bTimeOut);
			}
		}, InTimeOut);
	}

	bool Setup(TUniqueFunction<bool()>&& InPredicate, TUniqueFunction<void(float)>&& InTickFunc, TUniqueFunction<void()>&& InCompleteFunc, float InTimeOut)
	{
		CompleteFunc_NoTimeOut_NoStopped = MoveTemp(InCompleteFunc);
		return Setup(MoveTemp(InPredicate), MoveTemp(InTickFunc), [this](bool bTimeOut, bool bStopped)
		{
			if (CompleteFunc_NoTimeOut_NoStopped)
			{
				CompleteFunc_NoTimeOut_NoStopped();
			}
		}, InTimeOut);
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
		DECLARE_SCOPE_CYCLE_COUNTER(TEXT("WhileTrueExecute - Tick"), STAT_ECFDETAILS_WHILETRUEEXECUTE, STATGROUP_ECFDETAILS);
#endif

#if ECF_INSIGHT_PROFILING
		TRACE_CPUPROFILER_EVENT_SCOPE("ECF - WhileTrueExecute Tick");
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
		if (CompleteFunc)
		{
			CompleteFunc(bTimedOut, bStopped);
		}
	}
};

ECF_PRAGMA_ENABLE_OPTIMIZATION