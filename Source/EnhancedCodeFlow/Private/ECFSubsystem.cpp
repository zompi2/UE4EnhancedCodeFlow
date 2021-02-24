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
	Actions.RemoveAll([](UECFActionBase* Action){ return Action->IsValid() == false; });

	// There might be a situation the pending action is invalid too.
	PendingAddActions.RemoveAll([&](UECFActionBase* Action) { return Action->IsValid() == false; });
	
	// Add all pending actions
	Actions.Append(PendingAddActions);
	PendingAddActions.Empty();

	// Tick all active actions
	for (UECFActionBase* Action : Actions)
	{
		if (Action->IsValid())
		{
			Action->DoTick(DeltaTime);
		}
	}
}

void UECFSubsystem::RemoveAction(FECFHandle& HandleId)
{
	if (HandleId.IsValid())
	{
		// Find running action and set it as finished
		if (UECFActionBase** ActionFound = Actions.FindByPredicate([&](UECFActionBase* Action) { return Action->GetHandleId() == HandleId; }))
		{
			(*ActionFound)->MarkAsFinished();
			HandleId.Invalidate();
		}
		// If not found, ensure user don't want to stop pending action
		else if (UECFActionBase** PendingActionFound = PendingAddActions.FindByPredicate([&](UECFActionBase* Action) { return Action->GetHandleId() == HandleId; }))
		{
			(*PendingActionFound)->MarkAsFinished();
			HandleId.Invalidate();
		}
	}
}

void UECFSubsystem::RemoveActionsOfClass(TSubclassOf<UECFActionBase> ActionClass, UObject* InOwner/* = nullptr*/)
{
	// Find running actions of given class assigned to a specific owner (if specified) and set it as finished.
	for (UECFActionBase* Action : Actions)
	{
		if (Action->IsA(ActionClass))
		{
			if (InOwner == nullptr || InOwner == Action->Owner)
			{
				Action->MarkAsFinished();
			}
		}
	}

	// Also check pending actions to prevent from launching it.
	for (UECFActionBase* PendingAction : PendingAddActions)
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

void UECFSubsystem::RemoveAllActions()
{
	// Just stop all running and pending actions.
	for (UECFActionBase* Action : Actions)
	{
		Action->MarkAsFinished();
	}
	for (UECFActionBase* PendingAction : PendingAddActions)
	{
		PendingAction->MarkAsFinished();
	}
}

bool UECFSubsystem::HasAction(const FECFHandle& HandleId) const
{
	if (HandleId.IsValid())
	{
		// Find running action and check if it is a valid one
		if (UECFActionBase* const* ActionFound = Actions.FindByPredicate([&](UECFActionBase* Action) { return Action->GetHandleId() == HandleId; }))
		{
			return (*ActionFound)->IsValid();
		}
		// If not found, ensure user don't want to check a pending action
		else if (UECFActionBase* const* PendingActionFound = PendingAddActions.FindByPredicate([&](UECFActionBase* Action) { return Action->GetHandleId() == HandleId; }))
		{
			return (*ActionFound)->IsValid();
		}
	}

	return false;
}
