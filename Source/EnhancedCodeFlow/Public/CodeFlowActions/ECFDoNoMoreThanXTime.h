// Copyright (c) 2025 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFActionBase.h"
#include "ECFDoNoMoreThanXTime.generated.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UCLASS()
class ENHANCEDCODEFLOW_API UECFDoNoMoreThanXTime : public UECFActionBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

protected:

	TUniqueFunction<void()> ExecFunc;
	float LockTime = 0.f;
	float CurrentTime = 0.f;
	int32 ExecsEnqueued = 0;
	int32 MaxExecsEnqueued = 0;


	bool Setup(TUniqueFunction<void()>&& InExecFunc, float InTime, int32 InMaxExecsEnqueued)
	{
		ExecFunc = MoveTemp(InExecFunc);
		LockTime = InTime;
		MaxExecsEnqueued = InMaxExecsEnqueued;

		if (ExecFunc && LockTime > 0 && MaxExecsEnqueued > 0)
		{
			return true;
		}
		else
		{
#if ECF_LOGS
			UE_LOG(LogECF, Error, TEXT("ECF - Do No More Than Times failed to start. Are you sure the Lock time and Max Execs Eneueud are greater than 0 and the Exec Function is set properly?"));
#endif
			return false;
		}
	}

	void Init() override
	{
		CurrentTime = 0.f;
		ExecsEnqueued = 0;
		ExecFunc();
	}

	void Reset(bool bCallUpdate) override
	{
		CurrentTime = 0.f;
		ExecsEnqueued = 0;
		if (bCallUpdate)
		{
			ExecFunc();
		}
	}

	void RetriggeredInstancedAction() override
	{
		if (CurrentTime < LockTime)
		{
			if (ExecsEnqueued < MaxExecsEnqueued)
			{
				ExecsEnqueued++;
			}
		}
		else
		{
			CurrentTime = 0;
			ExecFunc();
		}
	}

	void Tick(float DeltaTime) override
	{
#if STATS
		DECLARE_SCOPE_CYCLE_COUNTER(TEXT("DoNoMoreThanXTime - Tick"), STAT_ECFDETAILS_DONOMORETHANXTIMES, STATGROUP_ECFDETAILS);
#endif

#if ECF_INSIGHT_PROFILING
		TRACE_CPUPROFILER_EVENT_SCOPE("ECF - DoNoMoreThanXTime Tick");
#endif

		if (CurrentTime < LockTime)
		{
			CurrentTime += DeltaTime;
		}
		
		if ((ExecsEnqueued > 0) && (CurrentTime >= LockTime))
		{
			CurrentTime = 0;
			ExecsEnqueued--;
			ExecFunc();
		}
	}
};

ECF_PRAGMA_ENABLE_OPTIMIZATION