// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#include "ECFSubsystem.h"
#include "ECFActionBase.h"

UECFSubsystem* UECFSubsystem::Singleton = nullptr;

void UECFSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		checkf(Singleton == nullptr, TEXT("There can be only one Code Flow Subsystem!"));
		Singleton = this;

		LastHandleId.Invalidate();
	}
}

void UECFSubsystem::Deinitialize()
{
	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		Singleton = nullptr;

		Actions.Empty();
		PendingAddActions.Empty();
	}
}

UECFSubsystem* UECFSubsystem::Get()
{
	// Use pointer to itself as game instance subsystem works like singletons and we can use
	// cached pointer to it without passing a pointer to world object.
	checkf(Singleton != nullptr, TEXT("Code Flow Subsystem used before Initialize!"));
	return Singleton;
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
