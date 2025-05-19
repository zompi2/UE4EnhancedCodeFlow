// Copyright (c) 2025 Damian Nowakowski. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ECFHandle.h"
#include "ECFInstanceId.h"
#include "ECFActionSettings.h"
#include "ECFStats.h"
#include "Misc/AssertionMacros.h"
#include "Engine/World.h"
#include "GameFramework/WorldSettings.h"
#include "ECFLogs.h"
#include "ECFActionBase.generated.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UCLASS()
class ENHANCEDCODEFLOW_API UECFActionBase : public UObject
{
	GENERATED_BODY()

	friend class UECFSubsystem;

public:

	/**
	 * Default constructor and destructor. Used only for stats counting.
	 */
	UECFActionBase();
	virtual ~UECFActionBase();

	// Checks if the action has valid owner
	virtual bool HasValidOwner() const
	{
		return Owner.IsValid() && (Owner->HasAnyFlags(RF_BeginDestroyed | RF_FinishDestroyed) == false);
	}

	// Checks if the action is valid
	virtual bool IsValid() const
	{
		return bHasFinished == false && HasValidOwner();
	}

	// Returns this action handle id
	FECFHandle GetHandleId() const
	{
		return HandleId;
	}

	// Returns this action instance id
	FECFInstanceId GetInstanceId() const 
	{
		return InstanceId;
	}

	// Marks this action as finished. It makes it invalid. 
	// This action will be deleted soon.
	void MarkAsFinished()
	{
#if (ECF_LOGS && ECF_LOGS_VERBOSE)
		UE_LOG(LogECF, Verbose, TEXT("Action of class %s marked as finished"), *GetName());
#endif
		bHasFinished = true;
	}

	// Checks if this action has this instance id.
	bool HasInstanceId(const FECFInstanceId& InstanceIdToCheck) const
	{
		if (InstanceId.IsValid() && InstanceIdToCheck.IsValid())
		{
			if (InstanceId == InstanceIdToCheck)
			{
				return true;
			}
		}
		return false;
	}

protected:

	// Setups the action. Validate all given arguments here.
	// It is not a virtual function because any action will require different
	// set of parameters.
	bool Setup() { return true; }
	
	// Sets initial values for this action.
	virtual void Init() {}

	// Ticks this action.
	virtual void Tick(float DeltaTime) {}

	// Function called when the action is requested to be completed before it ends.
	virtual void Complete(bool bStopped) {}

	// Function called when this action is instanced and something tried to call it again.
	virtual void RetriggeredInstancedAction() {}

	// Function called when this action is resetted. Have in mind that not every action has
	// reset functionality.
	// If bCallUpdate is true - the action should run an update event (if there is any) 
	// after it's reset.
	virtual void Reset(bool bCallUpdate) {}

	// For any action that should last only the given time - set this function
	// inside the action's Setup step. 
	// WARNING! This is only to help ticker run ticks with proper delta times.
	// It will not stop the action itself!
	void SetMaxActionTime(float InMaxActionTime)
	{
		MaxActionTime = InMaxActionTime;
	}

	// Pointer to the owner of this action. Owner must be valid all the time, otherwise
	// the action will become invalid and will be deleted.
	UPROPERTY(Transient)
	TWeakObjectPtr<const UObject> Owner;

	// Current handle of this action
	FECFHandle HandleId;

	// InstanceId for actions that are instanced
	FECFInstanceId InstanceId;

	// Settings for this action
	FECFActionSettings Settings;

	// Sets the owner and handle id of this action.
	void SetAction(const UObject* InOwner, const FECFHandle& InHandleId, const FECFInstanceId& InInstanceId, const FECFActionSettings& InSettings)
	{
		Owner = InOwner;
		HandleId = InHandleId;
		InstanceId = InInstanceId;
		Settings = InSettings;

		CurrentActionTime = 0.f;
		AccumulatedTime = 0.f;

		ActionDelayLeft = Settings.FirstDelay;

		bFirstTick = true;

		if (Settings.bStartPaused)
		{
			bIsPaused = true;
		}
	}

private:

	// Performs a tick. Apply any settings to the time step.
	void DoTick(float DeltaTime)
	{
		// If this action is paused - ignore tick.
		if (bIsPaused)
		{
			return;
		}

		// If game is paused and the action does not ignore this pause - ignore tick.
		if (Settings.bIgnorePause == false)
		{
			if (UWorld* World = GetWorld())
			{
				if (World->IsPaused())
				{
					return;
				}
			}
		}

		// If global time dilation is not ignored (by default it is not) apply
		// this time dilation to the delta time.
		if (Settings.bIgnoreGlobalTimeDilation == false)
		{
			float TimeDilation = 1.f;
			if (UWorld* World = GetWorld())
			{
				if (AWorldSettings* WorldSettings = World->GetWorldSettings())
				{
					TimeDilation = WorldSettings->TimeDilation;
				}
			}
			DeltaTime *= TimeDilation;
		}

		// Delay first tick if specified in settings.
		if (ActionDelayLeft > 0.f)
		{
			ActionDelayLeft -= DeltaTime;
			return;
		}
	
		// Append current action time with delta.
		CurrentActionTime += DeltaTime;

		// If this action exceeded it's maximum time.
		if (MaxActionTime > 0.f && CurrentActionTime >= MaxActionTime)
		{
			// Launch last tick. If this was using time intervals - tick with the proper accumulated time.
			// If not, simply do a tick.
			// WARNING! This will not stop the action. Ensure that the action has proper logic in tick function
			// that will stop it when the time has passed.
			if (Settings.TickInterval > 0.f)
			{
				AccumulatedTime += DeltaTime;
				Tick(AccumulatedTime);
				AccumulatedTime = 0;
			}
			else
			{
				Tick(DeltaTime);
			}
		}
		else
		{
			// We shall perform first tick no matter what.
			if (bFirstTick)
			{
				Tick(DeltaTime);
				bFirstTick = false;
			}
			else
			{
				// If we have specified tick intervals accumulate time until it won't reach the desired interval.
				// Tick with the given interval, otherwise do a simple tick.
				// Clear accumulated time (no subtract the interval) as we are interested in ticks 
				// that happens the exact time after the previous one.
				if (Settings.TickInterval > 0.f)
				{
					AccumulatedTime += DeltaTime;
					if (AccumulatedTime >= Settings.TickInterval)
					{
						Tick(Settings.TickInterval);
						AccumulatedTime = 0;
					}
				}
				else
				{
					Tick(DeltaTime);
				}
			}
		}
	}

	// Indicates if this action has finished and will be deleted soon.
	bool bHasFinished = false;

	// Indicates if this is a first tick. First tick should be launched as
	// soon as possible, without consideration of tick interval.
	bool bFirstTick = false;

	// Indicates if this action is paused (by the ECF system).
	bool bIsPaused = false;

	// Timers for this action
	float CurrentActionTime = 0.f;
	float ActionDelayLeft = 0.f;
	float AccumulatedTime = 0.f;
	float MaxActionTime = 0.f;
};

ECF_PRAGMA_ENABLE_OPTIMIZATION