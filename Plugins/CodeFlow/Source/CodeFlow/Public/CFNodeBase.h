// Copyright (c) 2020 Damian Nowakowski. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Tickable.h"
#include "CFNodeBase.generated.h"

UCLASS()
class CODEFLOW_API UCFNodeBase : public UObject
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
