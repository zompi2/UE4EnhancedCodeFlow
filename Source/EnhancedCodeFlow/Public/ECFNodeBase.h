// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Tickable.h"
#include "ECFNodeHandle.h"
#include "ECFNodeBase.generated.h"

UCLASS()
class ENHANCEDCODEFLOW_API UECFNodeBase : public UObject
{
	GENERATED_BODY()

public:

	bool Setup() { return true; }
	virtual void Init() {}
	virtual void Tick(float DeltaTime) {}

	virtual bool IsValid()
	{
		return bHasFinished == false && Owner.IsValid();
	}

	void SetOwner(const TWeakObjectPtr<UObject>& InOwner, const FECFNodeHandle& InHandleId)
	{
		Owner = InOwner;
		HandleId = InHandleId;
	}

	FECFNodeHandle GetHandleId() const
	{
		return HandleId;
	}

	void MarkAsFinished()
	{
		bHasFinished = true;
	}

private:

	bool bHasFinished = false;

protected:

	UPROPERTY()
	TWeakObjectPtr<UObject> Owner;

	FECFNodeHandle HandleId;
};
