// Copyright (c) 2020 Damian Nowakowski. All rights reserved.

#include "CFSubsystem.h"
#include "CFNodeBase.h"

UCFSubsystem* UCFSubsystem::Singleton = nullptr;

void UCFSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		checkf(Singleton == nullptr, TEXT("There can be only one Code Flow Subsystem!"));
		Singleton = this;

		NextHandleId = 0;
	}
}

void UCFSubsystem::Deinitialize()
{
	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		Singleton = nullptr;

		Nodes.Empty();
	}
}

UCFSubsystem* UCFSubsystem::Get()
{
	// Use pointer to itself as game instance subsystem works like singletons and we can use
	// cached pointer to it without passing a pointer to world object.
	checkf(Singleton != nullptr, TEXT("Code Flow Subsystem used before Initialize!"));
	return Singleton;
}	

void UCFSubsystem::Tick(float DeltaTime)
{
	Nodes.RemoveAll([](UCFNodeBase* Node){ return Node->IsValid() == false; });
	for (UCFNodeBase* Node : Nodes)
	{
		if (Node->IsValid())
		{
			Node->Tick(DeltaTime);
		}
	}
}

void UCFSubsystem::RemoveNode(uint64 HandleId)
{
	if (UCFNodeBase** NodeFound = Nodes.FindByPredicate([&](UCFNodeBase* Node) {return Node->HandleId; }))
	{
		Nodes.Remove(*NodeFound);
	}
}
