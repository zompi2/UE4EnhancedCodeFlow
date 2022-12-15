// Copyright (c) 2022 Damian Nowakowski. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ECFTypes.h"

class ENHANCEDCODEFLOW_API FECFInstanceId
{

public:

	FECFInstanceId() :
		Id(0)
	{}

	FECFInstanceId(uint64 InId) :
		Id(InId)
	{}

	FECFInstanceId(const FECFInstanceId& Other) :
		Id(Other.Id)
	{}

	FECFInstanceId(FECFInstanceId&& Other) :
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

	// Compare Ids.
	bool operator==(const FECFInstanceId& Other) const
	{
		return Id == Other.Id;
	}

	// Compare (not) Ids.
	bool operator!=(const FECFInstanceId& Other) const
	{
		return Id != Other.Id;
	}

	// Copy.
	FECFInstanceId& operator=(const FECFInstanceId& Other)
	{
		Id = Other.Id;
		return *this;
	}

	// Move.
	FECFInstanceId& operator=(FECFInstanceId&& Other)
	{
		Id = Other.Id;
		Other.Invalidate();
		return *this;
	}

	// Convert the Id  to string.
	FString ToString() const
	{
		return FString::Printf(TEXT("%llu"), Id);
	}

	// Returns a new id.
	static FECFInstanceId NewId();

protected:

	uint64 Id;

private:
	
	static uint64 DynamicIdCounter;
};
