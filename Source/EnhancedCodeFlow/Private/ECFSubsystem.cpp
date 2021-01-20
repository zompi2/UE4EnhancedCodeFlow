// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#include "ECFSubsystem.h"
#include "ECFNodeBase.h"

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

		Nodes.Empty();
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
	// Remove all expired nodes first
	Nodes.RemoveAll([](UECFNodeBase* Node){ return Node->IsValid() == false; });
	
	// Add all pending nodes
	Nodes.Append(PendingAddNodes);
	PendingAddNodes.Empty();

	// Tick all active nodes
	for (UECFNodeBase* Node : Nodes)
	{
		if (Node->IsValid())
		{
			Node->Tick(DeltaTime);
		}
	}
}

void UECFSubsystem::RemoveNode(FECFHandle& HandleId)
{
	if (HandleId.IsValid())
	{
		if (UECFNodeBase** NodeFound = Nodes.FindByPredicate([&](UECFNodeBase* Node) { return Node->GetHandleId() == HandleId; }))
		{
			(*NodeFound)->MarkAsFinished();
			HandleId.Invalidate();
		}
	}
}
