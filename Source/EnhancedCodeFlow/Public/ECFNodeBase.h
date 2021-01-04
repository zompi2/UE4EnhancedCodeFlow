// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Tickable.h"
#include "ECFNodeBase.generated.h"

UCLASS()
class ENHANCEDCODEFLOW_API UECFNodeBase : public UObject
{
	GENERATED_BODY()

public:

	void Setup() {}
	virtual void Init() {}
	virtual void Tick(float DeltaTime) {}

	UPROPERTY()
	TWeakObjectPtr<UObject> Owner;

	uint64 HandleId;
	bool bHasFinished = false;

	virtual bool IsValid()
	{
		return bHasFinished == false && Owner.IsValid();
	}
};
