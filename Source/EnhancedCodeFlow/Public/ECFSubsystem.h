// Copyright (c) 2025 Damian Nowakowski. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Tickable.h"
#include "ECFHandle.h"
#include "ECFActionBase.h"
#include "ECFInstanceId.h"
#include "ECFActionSettings.h"
#include "ECFStats.h"
#include "Coroutines/ECFCoroutine.h"
#include "ECFLogs.h"
#include "ECFSubsystem.generated.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UCLASS()
class ENHANCEDCODEFLOW_API UECFSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

	friend class FEnhancedCodeFlow;
	friend class FECFCoroutineAwaiter;

protected:

	/** UGameInstanceSubsystem interface implementation */
	void Initialize(FSubsystemCollectionBase& Collection) override;
	void Deinitialize() override;

	/** FTickableGameObject interface implementation */
	void Tick(float DeltaTime) override;
	TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(ECFSubsystem, STATGROUP_Tickables); }
	bool IsTickable() const override { return bCanTick; }
	bool IsTickableWhenPaused() const override { return true; }

	// Add Action to list. Returns the Action id.
	template<typename T, typename ... Ts>
	FECFHandle AddAction(const UObject* InOwner, const FECFActionSettings& Settings, const FECFInstanceId& InstanceId, Ts&& ... Args)
	{
		// Ensure the Action has been started from the Game Thread.
		if (IsInGameThread() == false)
		{
#if ECF_LOGS
			UE_LOG(LogECF, Error, TEXT("ECF Actions must be started from the Game Thread!"));
#endif
			checkf(false, TEXT("ECF Actions must be started from the Game Thread!"));
			return FECFHandle();
		}

		// There can be only one instanced action running at the same time. When trying to add an
		// action with existing instance id - return the currently running action's handle.
		UECFActionBase* PossibleInstancedAction = GetInstancedAction(InstanceId, false);
		if (IsActionValid(PossibleInstancedAction))
		{
			// Re-trigger active instanced action for any extra logic.
			PossibleInstancedAction->RetriggeredInstancedAction();
			return PossibleInstancedAction->GetHandleId();
		}

		// Otherwise, create and set new action.
		T* NewAction = NewObject<T>(this);
		NewAction->SetAction(InOwner, ++LastHandleId, InstanceId, Settings);
		if (NewAction->Setup(Forward<Ts>(Args)...))
		{
			NewAction->Init();
			PendingAddActions.Add(NewAction);
#if (ECF_LOGS && ECF_LOGS_VERBOSE)
			if (InstanceId.IsValid())
			{
				UE_LOG(LogECF, Verbose, TEXT("Started Instanced Action of class: %s, with HandleId: %s, and InstanceId: %s, Label: %s"), *NewAction->GetName(), *LastHandleId.ToString(), *InstanceId.ToString(), *Settings.Label);
			}
			else
			{
				UE_LOG(LogECF, Verbose, TEXT("Started Action of class: %s, with HandleId: %s, Label: %s"), *NewAction->GetName(), *LastHandleId.ToString(), *Settings.Label);
			}
#endif
			return NewAction->GetHandleId();
		}
		else
		{
#if ECF_LOGS
			UE_LOG(LogECF, Error, TEXT("Failed to Setup Action of class: %s, Label: %s"), *NewAction->GetName(), *Settings.Label);
#endif
		}

		// If the action couldn't be created for any reason - return invalid id.
		return FECFHandle();
	}

	// Add Coroutine Action to List.
	template<typename T, typename ... Ts>
	void AddCoroutineAction(const UObject* InOwner, FECFCoroutineHandle InCoroutineHandle, const FECFActionSettings& Settings, Ts&& ... Args)
	{
		// Ensure the Action has been started from the Game Thread.
		if (IsInGameThread() == false)
		{
#if ECF_LOGS
			UE_LOG(LogECF, Error, TEXT("ECF Coroutines must be started from the Game Thread!"));
#endif
			checkf(false, TEXT("ECF Coroutines must be started from the Game Thread!"));
			return;
		}

		// Create and set new coroutine action.
		T* NewAction = NewObject<T>(this);
		NewAction->SetCoroutineAction(InOwner, InCoroutineHandle, ++LastHandleId, Settings);
		if (NewAction->Setup(Forward<Ts>(Args)...))
		{
			NewAction->Init();
#if (ECF_LOGS && ECF_LOGS_VERBOSE)
			UE_LOG(LogECF, Verbose, TEXT("Started Coroutine Action of class: %s, Label: %s"), *NewAction->GetName(), *Settings.Label);
#endif
			PendingAddActions.Add(NewAction);
		}
		else
		{
#if ECF_LOGS
			UE_LOG(LogECF, Error, TEXT("Failed to Setup Coroutine Action of class: %s, Label: %s"), *NewAction->GetName(), *Settings.Label);
#endif
		}
	}

	// Try to find running or pending action based on it's handle.
	UECFActionBase* FindAction(const FECFHandle& HandleId) const;

	// Finds handles of running or pending action of the given Class its FECFHandles.
	TArray<FECFHandle> GetActionsHandlesByClass(TSubclassOf<UECFActionBase> Class) const;
	
	template<typename T>
	TArray<FECFHandle> GetActionsHandlesByClass() const
	{
		return GetActionsHandlesByClass(T::StaticClass());
	}

	// Finds handles of running or pending action of the given Label its FECFHandles.
	TArray<FECFHandle> GetActionsHandlesByLabel(const FString& Label) const;

	// Returns the array of all running and pending actions. Use it mostly for debugging purposes.
	TArray<UECFActionBase*> GetAllActions() const;

	// Returns the number of all running and pending actions. Use it mostly for debugging purposes.
	int32 GetActionsCount() const;

	// Check if the action is running or pending to run.
	bool HasAction(const FECFHandle& HandleId) const;

	// Pause ticking in this action
	void PauseAction(const FECFHandle& HandleId);

	// Resume ticking in this action
	void ResumeAction(const FECFHandle& HandleId);

	// Checks if this action is not paused. Returns false if there is no action.
	bool IsActionPaused(const FECFHandle& HandleId, bool &bIsPaused) const;

	// Resets the action. 
	// Returns false if there was no action or action doesn't support resetting. 
	// If bCallUpdate is true - the action should run an update event (if there is any) after it's reset.
	bool ResetAction(const FECFHandle& HandleId, bool bCallUpdate);

	// Remove Action of given HandleId from list. 
	void RemoveAction(FECFHandle& HandleId, bool bComplete);

	// Remove all Actions of async tasks of given class from list.
	void RemoveActionsOfClass(TSubclassOf<UECFActionBase> ActionClass, bool bComplete, UObject* InOwner);

	// Template version of RemoveActionsOfClass.
	template<typename T>
	void RemoveActionsOfClass(bool bComplete, UObject* InOwner)
	{
		RemoveActionsOfClass(T::StaticClass(), bComplete, InOwner);
	}

	// Remove all Actions of async tasks with the given Label from list.
	void RemoveActionsOfLabel(const FString& Label, bool bComplete, UObject* InOwner);

	// Remove action with the given InstanceId.
	void RemoveInstancedAction(const FECFInstanceId& InstanceId, bool bComplete);

	// Remove all Actions of async tasks.
	void RemoveAllActions(bool bComplete, UObject* InOwner);

	// Check if there is an instanced action running with the given instance id and returns it.
	UECFActionBase* GetInstancedAction(const FECFInstanceId& InstanceId, bool bPrintErrorIfFailed = true) const;
	
	// List of active actions.
	UPROPERTY(Transient)
	TArray<UECFActionBase*> Actions;

	// List of nodes to be add in the future.
	UPROPERTY(Transient)
	TArray<UECFActionBase*> PendingAddActions;

	// Id of the last created node.
	FECFHandle LastHandleId;

	// Indicates if subsystem is paused
	bool bIsECFPaused = false;
	
	// Getter handling.
	static UECFSubsystem* Get(const UObject* WorldContextObject);

	// Helper function to finish action.
	void FinishAction(UECFActionBase* Action, bool bComplete);

	// Utility function to check action validity.
	static bool IsActionValid(UECFActionBase* Action);

private:

	// Indicates if this subsystem should tick
	bool bCanTick = false;
};

ECF_PRAGMA_ENABLE_OPTIMIZATION