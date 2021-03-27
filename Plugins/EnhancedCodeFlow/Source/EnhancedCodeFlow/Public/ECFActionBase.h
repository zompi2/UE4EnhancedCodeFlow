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

	// Performs a tick. Apply global time dilation to delta time and pass it to the 
	// virtual tick function.
	void DoTick(float DeltaTime)
	{
		if (Settings.bIgnoreTimeDilation == false)
		{
			float TimeDilation = 1.f;
			if (Owner.IsValid())
			{
				if (UWorld* World = Owner->GetWorld())
				{
					if (AWorldSettings* WorldSettings = World->GetWorldSettings())
					{
						TimeDilation = WorldSettings->TimeDilation;
					}
				}
			}
			DeltaTime *= TimeDilation;
		}
	

		CurrentActionTime += DeltaTime;
		if (MaxActionTime > 0.f && CurrentActionTime >= MaxActionTime)
		{
			if (Settings.TickInterval > 0.f)
			{
				AccumulatedTime += DeltaTime;
				Tick(AccumulatedTime);
			}
			else
			{
				Tick(DeltaTime);
			}
		}
		else
		{
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

	float CurrentActionTime = 0.f;
	float AccumulatedTime = 0.f;
	float MaxActionTime = 0.f;
};
