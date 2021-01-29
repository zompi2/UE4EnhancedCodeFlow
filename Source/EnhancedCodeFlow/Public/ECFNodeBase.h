// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Tickable.h"
#include "ECFHandle.h"
#include "ECFNodeBase.generated.h"

UCLASS()
class ENHANCEDCODEFLOW_API UECFNodeBase : public UObject
{
	GENERATED_BODY()

	friend class UECFSubsystem;

public:

	virtual bool IsValid() const
	{
		return bHasFinished == false && Owner.IsValid();
	}

	FECFHandle GetHandleId() const
	{
		return HandleId;
	}

	void MarkAsFinished()
	{
		bHasFinished = true;
	}

protected:

	bool Setup() { return true; }
	virtual void Init() {}
	virtual void Tick(float DeltaTime) {}

	void SetOwner(const TWeakObjectPtr<UObject>& InOwner, const FECFHandle& InHandleId)
	{
		Owner = InOwner;
		HandleId = InHandleId;
	}

	UPROPERTY()
	TWeakObjectPtr<UObject> Owner;

	FECFHandle HandleId;

private:

	void DoTick(float DeltaTime)
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

		Tick(DeltaTime * TimeDilation);
	}

	bool bHasFinished = false;
};
