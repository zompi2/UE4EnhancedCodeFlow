// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Tickable.h"
#include "ECFNodeHandle.h"
#include "ECFSubsystem.generated.h"

class UECFNodeBase;

UCLASS()
class ENHANCEDCODEFLOW_API UECFSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

	friend class UEnhancedCodeFlow;

protected:

	/** UGameInstanceSubsystem interface implementation */
	void Initialize(FSubsystemCollectionBase& Collection) override;
	void Deinitialize() override;

	/** FTickableGameObject interface implementation */
	void Tick(float DeltaTime) override;
	TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(ECFSubsystem, STATGROUP_Tickables); }
	bool IsTickable() const override { return true; }
	bool IsTickableWhenPaused() const override { return true; }

	// Add Node of async task to list. Returns the task id.
	template<typename T, typename ... Ts>
	FECFNodeHandle AddNode(UObject* InOwner, Ts&& ... Args)
	{
		T* NewNode = NewObject<T>(this);
		NewNode->SetOwner(InOwner, ++LastHandleId);
		if (NewNode->Setup(Forward<Ts>(Args)...))
		{
			NewNode->Init();
			PendingAddNodes.Add(NewNode);
			return NewNode->GetHandleId();
		}
		return FECFNodeHandle();
	}

	// Remove Node of async task from list.
	void RemoveNode(FECFNodeHandle HandleId);
	
	// List of active nodes.
	UPROPERTY()
	TArray<UECFNodeBase*> Nodes;

	// List of nodes to be add in the future.
	UPROPERTY()
	TArray<UECFNodeBase*> PendingAddNodes;

	// Id of the last created node.
	FECFNodeHandle LastHandleId;
	
	/** Singleton handling */
	static UECFSubsystem* Singleton;
	static UECFSubsystem* Get();
};
