// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ECFTypes.h"

class FECFInstanceId
{

public:

	FECFInstanceId() : 
		Scope(EECFInstanceIdScope::Object),
		Id(0)
	{}

	FECFInstanceId(EECFInstanceIdScope InScope, uint64 InId) :
		Scope(InScope),
		Id(InId)
	{}

	FECFInstanceId(const FECFInstanceId& Other) :
		Scope(Other.Scope),
		Id(Other.Id)
	{}

	FECFInstanceId(FECFInstanceId&& Other) :
		Scope(Other.Scope),
		Id(Other.Id)
	{
		Other.Invalidate();
	}

	// Checks if the Id is valid.
	bool IsValid() const
	{
		return Id != 0;
	}

	// Makes this Id invalid (invalidates it).
	void Invalidate()
	{
		Id = 0;
	}

	EECFInstanceIdScope GetScope() const
	{
		return Scope;
	}

	// Compare Ids.
	bool operator==(const FECFInstanceId& Other) const
	{
		return Id == Other.Id && Scope == Other.Scope;
	}

	// Compare (not) Ids.
	bool operator!=(const FECFInstanceId& Other) const
	{
		return Id != Other.Id || Scope != Other.Scope;
	}

	// Copy.
	FECFInstanceId& operator=(const FECFInstanceId& Other)
	{
		Id = Other.Id;
		Scope = Other.Scope;
		return *this;
	}

	// Move.
	FECFInstanceId& operator=(FECFInstanceId&& Other)
	{
		Id = Other.Id;
		Scope = Other.Scope;
		Other.Invalidate();
		return *this;
	}

	// Returns a new id.
	static FECFInstanceId NewId(EECFInstanceIdScope InScope = EECFInstanceIdScope::Object)
	{
		static uint64 DynamicIdCounter = 0;
		return FECFInstanceId(InScope, ++DynamicIdCounter);
	}

protected:

	EECFInstanceIdScope Scope;
	uint64 Id;
};
