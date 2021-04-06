// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Tickable.h"
#include "ECFHandle.h"
#include "ECFActionSettings.h"
#include "ECFActionBase.generated.h"

UCLASS()
class ENHANCEDCODEFLOW_API UECFActionBase : public UObject
{
	GENERATED_BODY()

	friend class UECFSubsystem;

public:

	// Checks if the action is valid
	virtual bool IsValid() const
	{
		return bHasFinished == false && Owner.IsValid();
	}

	// Returns this action handle id
	FECFHandle GetHandleId() const
	{
		return HandleId;
	}

	// Marks this action as finished. It makes it invalid. 
	// This action will be deleted soon.
	void MarkAsFinished()
	{
		bHasFinished = true;
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
	UPROPERTY()
	TWeakObjectPtr<UObject> Owner;

	// Current handle of this action
	FECFHandle HandleId;

	// Settings for this action
	FECFActionSettings Settings;

private:

	// Sets the owner and handle id of this action.
	void SetAction(const TWeakObjectPtr<UObject>& InOwner, const FECFHandle& InHandleId, const FECFActionSettings& InSettings)
	{
		Owner = InOwner;
		HandleId = InHandleId;
		Settings = InSettings;

		CurrentActionTime = 0.f;
		AccumulatedTime = 0.f;
	}

	// Performs a tick. Apply any settings to the time step.
	void DoTick(float DeltaTime)
	{
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
	
		// Append current action time with delta
		CurrentActionTime += DeltaTime;

		// If this action exceeded it's maximum time
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
			// If we have specified tick intervals accumulate time until it won't reach the desired interval.
			// Tick with the given interval and decrease the accumulation with this interval.
			// Otherwise do a simple tick.
			if (Settings.TickInterval > 0.f)
			{
				AccumulatedTime += DeltaTime;
				if (AccumulatedTime >= Settings.TickInterval)
				{
					Tick(Settings.TickInterval);
					while (AccumulatedTime >= Settings.TickInterval)
					{
						AccumulatedTime -= Settings.TickInterval;
					}
				}
			}
			else
			{
				Tick(DeltaTime);
			}
		}
	}

	// Indicates if this action has finished and will be deleted soon.
	bool bHasFinished = false;

	// Timers for this action
	float CurrentActionTime = 0.f;
	float AccumulatedTime = 0.f;
	float MaxActionTime = 0.f;
};
