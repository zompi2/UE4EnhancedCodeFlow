// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Tickable.h"
#include "ECFHandle.h"
#include "ECFSubsystem.generated.h"

class UECFActionBase;

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
	FECFHandle AddAction(UObject* InOwner, Ts&& ... Args)
	{
		T* NewNode = NewObject<T>(this);
		NewNode->SetOwner(InOwner, ++LastHandleId);
		if (NewNode->Setup(Forward<Ts>(Args)...))
		{
			NewNode->Init();
			PendingAddActions.Add(NewNode);
			return NewNode->GetHandleId();
		}
		return FECFHandle();
	}

	// Remove Node of async task from list.
	void RemoveAction(FECFHandle& HandleId);

	// Check if the action is running
	bool HasAction(const FECFHandle& HandleId) const;
	
	// List of active actions.
	UPROPERTY()
	TArray<UECFActionBase*> Actions;

	// List of nodes to be add in the future.
	UPROPERTY()
	TArray<UECFActionBase*> PendingAddActions;

	// Id of the last created node.
	FECFHandle LastHandleId;
	
	/** Singleton handling */
	static UECFSubsystem* Singleton;
	static UECFSubsystem* Get();
};
