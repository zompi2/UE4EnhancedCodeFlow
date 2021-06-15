// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#include "ECFSubsystem.h"
#include "ECFActionBase.h"

void UECFSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
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
		return ThisWorld->GetSubsystem<UECFSubsystem>(ThisWorld);
	}

	return nullptr;
}	

void UECFSubsystem::Tick(float DeltaTime)
{
	// Remove all expired actions first
	Actions.RemoveAll([](UECFActionBase* Action) { return IsActionValid(Action) == false; });

	// There might be a situation the pending action is invalid too.
	PendingAddActions.RemoveAll([&](UECFActionBase* PendingAddAction) { return IsActionValid(PendingAddAction) == false; });

	// Add all pending actions
	Actions.Append(PendingAddActions);
	PendingAddActions.Empty();

	// Tick all active actions
	for (UECFActionBase* Action : Actions)
	{
		if (IsActionValid(Action))
		{
			Action->DoTick(DeltaTime);
		}
	}
}

void UECFSubsystem::RemoveAction(FECFHandle& HandleId, bool bComplete)
{
	if (HandleId.IsValid())
	{
		// Find running action and set it as finished
		if (UECFActionBase** ActionFound = Actions.FindByPredicate([&](UECFActionBase* Action) { return (IsActionValid(Action) && (Action->GetHandleId() == HandleId)); }))
		{
			(*ActionFound)->MarkAsFinished();
			HandleId.Invalidate();
		}
		// If not found, ensure user don't want to stop pending action
		else if (UECFActionBase** PendingActionFound = PendingAddActions.FindByPredicate([&](UECFActionBase* PendingAddAction) { return (IsActionValid(PendingAddAction) && (PendingAddAction->GetHandleId() == HandleId)); }))
		{
			(*PendingActionFound)->MarkAsFinished();
			HandleId.Invalidate();
		}
	}
}

void UECFSubsystem::RemoveActionsOfClass(TSubclassOf<UECFActionBase> ActionClass, bool bComplete, UObject* InOwner)
{
	// Find running actions of given class assigned to a specific owner (if specified) and set it as finished.
	for (UECFActionBase* Action : Actions)
	{
		if (IsActionValid(Action))
		{
			if (Action->IsA(ActionClass))
			{
				if (InOwner == nullptr || InOwner == Action->Owner)
				{
					Action->MarkAsFinished();
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
					PendingAction->MarkAsFinished();
				}
			}
		}
	}
}

void UECFSubsystem::RemoveAllActions(bool bComplete, UObject* InOwner)
{
	// Stop all running and pending actions.
	for (UECFActionBase* Action : Actions)
	{
		if (IsActionValid(Action))
		{
			if (InOwner == nullptr || InOwner == Action->Owner)
			{
				Action->MarkAsFinished();
			}
		}
	}
	for (UECFActionBase* PendingAction : PendingAddActions)
	{
		if (IsActionValid(PendingAction))
		{
			if (InOwner == nullptr || InOwner == PendingAction->Owner)
			{
				PendingAction->MarkAsFinished();
			}
		}
	}
}

bool UECFSubsystem::HasAction(const FECFHandle& HandleId) const
{
	if (HandleId.IsValid())
	{
		// Find running action and check if it is a valid one
		if (UECFActionBase* const* ActionFound = Actions.FindByPredicate([&](UECFActionBase* Action) { return IsActionValid(Action) && Action->GetHandleId() == HandleId; }))
		{
			return (*ActionFound)->IsValid();
		}
		// If not found, ensure user don't want to check a pending action
		else if (UECFActionBase* const* PendingActionFound = PendingAddActions.FindByPredicate([&](UECFActionBase* PendingAction) { return IsActionValid(PendingAction) && PendingAction->GetHandleId() == HandleId; }))
		{
			return (*PendingActionFound)->IsValid();
		}
	}

	return false;
}

void UECFSubsystem::FinishAction(UECFActionBase* Action, bool bComplete)
{
	if (IsActionValid(Action))
	{
		if (bComplete)
		{
			Action->Complete();
		}
		Action->MarkAsFinished();
	}
}

bool UECFSubsystem::IsActionValid(UECFActionBase* Action)
{
	return IsValid(Action) && (Action->HasAnyFlags(RF_BeginDestroyed | RF_FinishDestroyed) == false) && Action->IsValid();
}