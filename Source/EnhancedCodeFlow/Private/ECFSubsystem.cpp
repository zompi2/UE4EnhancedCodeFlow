// Copyright (c) 2025 Damian Nowakowski. All rights reserved.

#include "ECFSubsystem.h"
#include "ECFActionBase.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

DEFINE_STAT(STAT_ECF_ActionsCount);
DEFINE_STAT(STAT_ECF_InstancesCount);

void UECFSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	// Only the subsystem from the Game World can tick.
	bCanTick = false;
	if (UWorld* ThisWorld = GetWorld())
	{
		switch (ThisWorld->WorldType)
		{
			case EWorldType::Game:
			case EWorldType::GamePreview:
			case EWorldType::GameRPC:
			case EWorldType::PIE:
				bCanTick = true;
		}
	}

	// Reset the HandleId counter
	LastHandleId.Invalidate();
}

void UECFSubsystem::Deinitialize()
{
	Actions.Empty();
	PendingAddActions.Empty();
}

UECFSubsystem* UECFSubsystem::Get(const UObject* WorldContextObject)
{
	UWorld* ThisWorld = nullptr;
	if (GEngine)
	{
		ThisWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	}

	ensureAlwaysMsgf(ThisWorld, TEXT("Can't obtain ThisWorld from WorldContextObject in ECF!"));
	if (ThisWorld)
	{
		UGameInstance* GameInstance = ThisWorld->GetGameInstance();
		ensureAlwaysMsgf(GameInstance, TEXT("Can't obtain GameInstance from WorldContextObject in ECF!"));
		if (GameInstance)
		{
			return GameInstance->GetSubsystem<UECFSubsystem>();
		}
		else
		{
#if ECF_LOGS
			UE_LOG(LogECF, Error, TEXT("Can't obtain GameInstance from WorldContextObject in ECF!"));
#endif
		}
	}
	else
	{
#if ECF_LOGS
		UE_LOG(LogECF, Error, TEXT("Can't obtain ThisWorld from WorldContextObject in ECF!"));
#endif
	}

	return nullptr;
}	

void UECFSubsystem::Tick(float DeltaTime)
{
	// Do nothing when the whole subsystem is paused
	if (bIsECFPaused)
	{
		return;
	}

#if STATS
	DECLARE_SCOPE_CYCLE_COUNTER(TEXT("Tick"), STAT_ECF_TickAll, STATGROUP_ECF);
#endif

#if ECF_INSIGHT_PROFILING
	TRACE_CPUPROFILER_EVENT_SCOPE("ECF - Subsystem Tick");
#endif

	// Remove all expired actions first
	Actions.RemoveAll([](UECFActionBase* Action) { return IsActionValid(Action) == false; });

	// There might be a situation the pending action is invalid too
	PendingAddActions.RemoveAll([&](UECFActionBase* PendingAddAction) { return IsActionValid(PendingAddAction) == false; });

	// Add all pending actions
	Actions.Append(PendingAddActions);
	PendingAddActions.Empty();

#if STATS
	SET_DWORD_STAT(STAT_ECF_ActionsCount, Actions.Num());
	SET_DWORD_STAT(STAT_ECF_InstancesCount, 0);
#endif

	// Tick all active actions
	for (UECFActionBase* Action : Actions)
	{
		if (IsActionValid(Action))
		{
#if STATS
			if (Action->InstanceId.IsValid())
			{
				INC_DWORD_STAT(STAT_ECF_InstancesCount);
			}
#endif
			Action->DoTick(DeltaTime);
		}
	}
}

UECFActionBase* UECFSubsystem::FindAction(const FECFHandle& HandleId) const
{
	if (HandleId.IsValid())
	{
		if (UECFActionBase* const* ActionFound = Actions.FindByPredicate([&](UECFActionBase* Action) { return (IsActionValid(Action) && (Action->GetHandleId() == HandleId)); }))
		{
			return *ActionFound;
		}
		else if (UECFActionBase* const* PendingActionFound = PendingAddActions.FindByPredicate([&](UECFActionBase* PendingAddAction) { return (IsActionValid(PendingAddAction) && (PendingAddAction->GetHandleId() == HandleId)); }))
		{
			return *PendingActionFound;
		}
	}

#if ECF_LOGS
	UE_LOG(LogECF, Error, TEXT("Couldn't find action of handle %s"), *HandleId.ToString());
#endif

	return nullptr;
}

void UECFSubsystem::PauseAction(const FECFHandle& HandleId)
{
	if (UECFActionBase* ActionFound = FindAction(HandleId))
	{
#if (ECF_LOGS && ECF_LOGS_VERBOSE)
		UE_LOG(LogECF, Verbose, TEXT("Paused Action of class: %s"), *ActionFound->GetName());
#endif
		ActionFound->bIsPaused = true;
	}
	else
	{
#if ECF_LOGS
		UE_LOG(LogECF, Error, TEXT("Can't find Action of id %s to pause"), *HandleId.ToString());
#endif
	}
}

void UECFSubsystem::ResumeAction(const FECFHandle& HandleId)
{
	if (UECFActionBase* ActionFound = FindAction(HandleId))
	{
#if (ECF_LOGS && ECF_LOGS_VERBOSE)
		UE_LOG(LogECF, Verbose, TEXT("Resume Action of class: %s"), *ActionFound->GetName());
#endif
		ActionFound->bIsPaused = false;
	}
	else
	{
#if ECF_LOGS
		UE_LOG(LogECF, Error, TEXT("Can't find Action of id %s to resume"), *HandleId.ToString());
#endif
	}
}

bool UECFSubsystem::IsActionPaused(const FECFHandle& HandleId, bool& bIsPaused) const
{
	if (UECFActionBase* ActionFound = FindAction(HandleId))
	{
		bIsPaused = ActionFound->bIsPaused;
		return true;
	}
	return false;
}

void UECFSubsystem::ResetAction(const FECFHandle& HandleId, bool bCallUpdate)
{
	if (UECFActionBase* ActionFound = FindAction(HandleId))
	{
		if (IsActionValid(ActionFound))
		{
#if (ECF_LOGS && ECF_LOGS_VERBOSE)
			UE_LOG(LogECF, Verbose, TEXT("Reset Action of class: %s"), *ActionFound->GetName());
#endif
			ActionFound->Reset(bCallUpdate);
			return;
		}
	}

#if ECF_LOGS
	UE_LOG(LogECF, Error, TEXT("Can't find Action of id %s to reset"), *HandleId.ToString());
#endif
}

void UECFSubsystem::RemoveAction(FECFHandle& HandleId, bool bComplete)
{
	if (UECFActionBase* ActionFound = FindAction(HandleId))
	{
#if (ECF_LOGS && ECF_LOGS_VERBOSE)
		UE_LOG(LogECF, Verbose, TEXT("Remove Action of class: %s"), *ActionFound->GetName());
#endif
		FinishAction(ActionFound, bComplete);
		HandleId.Invalidate();
	}
	else
	{
#if ECF_LOGS
		UE_LOG(LogECF, Error, TEXT("Can't find Action of id %s to remove"), *HandleId.ToString());
#endif
	}
}

void UECFSubsystem::RemoveActionsOfClass(TSubclassOf<UECFActionBase> ActionClass, bool bComplete, UObject* InOwner)
{
#if (ECF_LOGS && ECF_LOGS_VERBOSE)
	UE_LOG(LogECF, Verbose, TEXT("Removing Actions of class: %s"), *ActionClass->GetName());
#endif

	// Find running actions of given class assigned to a specific owner (if specified) and set it as finished.
	for (UECFActionBase* Action : Actions)
	{
		if (IsActionValid(Action))
		{
			if (Action->IsA(ActionClass))
			{
				if (InOwner == nullptr || InOwner == Action->Owner)
				{
					FinishAction(Action, bComplete);
				}
			}
		}
	}

	// Also check pending actions to prevent from launching it.
	for (UECFActionBase* PendingAction : PendingAddActions)
	{
		if (IsActionValid(PendingAction))
		{
			if (PendingAction->IsA(ActionClass))
			{
				if (InOwner == nullptr || InOwner == PendingAction->Owner)
				{
					FinishAction(PendingAction, bComplete);
				}
			}
		}
	}
}

void UECFSubsystem::RemoveInstancedAction(const FECFInstanceId& InstanceId, bool bComplete)
{
#if (ECF_LOGS && ECF_LOGS_VERBOSE)
	UE_LOG(LogECF, Verbose, TEXT("Removing Instanced Action of InstanceId: %s"), *InstanceId.ToString());
#endif

	// Stop all running and pending actions with the given InstanceId.
	for (UECFActionBase* Action : Actions)
	{
		if (IsActionValid(Action))
		{
			if (Action->HasInstanceId(InstanceId))
			{
				FinishAction(Action, bComplete);
			}
		}
	}
	for (UECFActionBase* PendingAction : PendingAddActions)
	{
		if (IsActionValid(PendingAction))
		{
			if (PendingAction->HasInstanceId(InstanceId))
			{
				FinishAction(PendingAction, bComplete);
			}
		}
	}
}

void UECFSubsystem::RemoveAllActions(bool bComplete, UObject* InOwner)
{
#if (ECF_LOGS && ECF_LOGS_VERBOSE)
	UE_LOG(LogECF, Verbose, TEXT("Removing All Actions"));
#endif

	// Stop all running and pending actions.
	for (UECFActionBase* Action : Actions)
	{
		if (IsActionValid(Action))
		{
			if (InOwner == nullptr || InOwner == Action->Owner)
			{
				FinishAction(Action, bComplete);
			}
		}
	}
	for (UECFActionBase* PendingAction : PendingAddActions)
	{
		if (IsActionValid(PendingAction))
		{
			if (InOwner == nullptr || InOwner == PendingAction->Owner)
			{
				FinishAction(PendingAction, bComplete);
			}
		}
	}
}

bool UECFSubsystem::HasAction(const FECFHandle& HandleId) const
{
	if (UECFActionBase* ActionFound = FindAction(HandleId))
	{
		return true;
	}
	return false;
}

UECFActionBase* UECFSubsystem::GetInstancedAction(const FECFInstanceId& InstanceId, bool bPrintErrorIfFailed/* = true*/) const
{
	if (InstanceId.IsValid())
	{
		if (UECFActionBase* const* ActionFound = Actions.FindByPredicate([&](UECFActionBase* Action) { return IsActionValid(Action) && Action->HasInstanceId(InstanceId); }))
		{
			return *ActionFound;
		}
		else if (UECFActionBase* const* PendingActionFound = PendingAddActions.FindByPredicate([&](UECFActionBase* PendingAction) { return IsActionValid(PendingAction) && PendingAction->HasInstanceId(InstanceId); }))
		{
			return *PendingActionFound;
		}
	}

	if (bPrintErrorIfFailed)
	{
#if ECF_LOGS
		UE_LOG(LogECF, Error, TEXT("Failed to Get Instanced Action from InstanceId: %s"), *InstanceId.ToString());
#endif
	}

	return nullptr;
}

void UECFSubsystem::FinishAction(UECFActionBase* Action, bool bComplete)
{
	if (IsActionValid(Action))
	{
		Action->MarkAsFinished();
		if (bComplete)
		{
			Action->Complete(true);
		}
	}
}

bool UECFSubsystem::IsActionValid(UECFActionBase* Action)
{
	return IsValid(Action) && (Action->HasAnyFlags(RF_BeginDestroyed | RF_FinishDestroyed) == false) && Action->IsValid();
}

ECF_PRAGMA_ENABLE_OPTIMIZATION
