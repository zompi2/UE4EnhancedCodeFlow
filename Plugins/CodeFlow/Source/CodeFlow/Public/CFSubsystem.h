// Copyright (c) 2020 Damian Nowakowski. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Tickable.h"
#include "CFSubsystem.generated.h"

class UCFNodeBase;

UCLASS()
class CODEFLOW_API UCFSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:

	/** Implement this for initialization of instances of the system */
	void Initialize(FSubsystemCollectionBase& Collection) override;

	/** Implement this for deinitialization of instances of the system */
	void Deinitialize() override;

	static UCFSubsystem* Get();

	/** FTickableGameObject interface implementation */
	void Tick(float DeltaTime) override;
	TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(CFSubsystem, STATGROUP_Tickables); }
	bool IsTickable() const override { return true; }
	bool IsTickableWhenPaused() const override { return true; }

	template<typename T, typename ... Ts>
	uint64 AddNode(UObject* InOwner, Ts&& ... Args)
	{
		T* NewNode = NewObject<T>(this);
		NewNode->Owner = InOwner;
		NewNode->HandleId = NextHandleId++;
		NewNode->Setup(Forward<Ts>(Args)...);
		NewNode->Init();
		Nodes.Add(NewNode);

		return NewNode->HandleId;
	}

	void RemoveNode(uint64 HandleId);

private:

	static UCFSubsystem* Singleton;

	UPROPERTY()
	TArray<UCFNodeBase*> Nodes;

	uint64 NextHandleId;
};
