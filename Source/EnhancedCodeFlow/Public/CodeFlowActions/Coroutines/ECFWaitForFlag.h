// Copyright (c) 2026 Damian Nowakowski. All rights reserved.

#pragma once

#include "Coroutines/ECFCoroutineActionBase.h"
#include "ECFWaitForFlag.generated.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UCLASS()
class ENHANCEDCODEFLOW_API UECFWaitForFlag : public UECFCoroutineActionBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

protected:

	bool* bFlag = nullptr;
	float TimeOut = 0.f;
	float OriginTimeOut = 0.f;
	bool bWithTimeOut = false;
	bool bTimedOut = false;

	bool Setup(bool* bInFlag, float InTimeOut)
	{
		bFlag = bInFlag;
		if (bFlag)
		{
			if (*bFlag)
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
			UE_LOG(LogECF, Error, TEXT("ECF Coroutine [%s] - Wait For Flag failed to start. Are you sure the Flag is set properly?"), *Settings.Label);
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
		DECLARE_SCOPE_CYCLE_COUNTER(TEXT("WaitForFlag - Tick"), STAT_ECFDETAILS_WAITFORFLAG, STATGROUP_ECFDETAILS);
#endif

#if ECF_INSIGHT_PROFILING
		TRACE_CPUPROFILER_EVENT_SCOPE("ECF - WaitForFlag Tick");
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

		if (bFlag && *bFlag)
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