// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "CoreMinimal.h"

enum class EECFInstanceType : uint8
{
	Dynamic,
	Static
};

class FECFInstanceId
{

public:

	FECFInstanceId() : 
		Type(EECFInstanceType::Dynamic),
		Id(0)
	{}

	FECFInstanceId(EECFInstanceType InType, const uint64 InId) :
		Type(InType),
		Id(InId)
	{}

	FECFInstanceId(const FECFInstanceId& Other) :
		Type(Other.Type),
		Id(Other.Id)
	{}

	FECFInstanceId(FECFInstanceId&& Other) :
		Type(Other.Type),
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

	// Makes this Id valid (if not valid)
	FECFInstanceId& Validate()
	{
		if (Id == 0)
		{
			static uint64 DynamicIdCounter = 0;
			Id = ++DynamicIdCounter;
			Type = EECFInstanceType::Dynamic;
		}
		return *this;
	}

	// Compare Ids.
	bool operator==(const FECFInstanceId& Other) const
	{
		return Type == Other.Type && Id == Other.Id;
	}

	// Compare (not) Ids.
	bool operator!=(const FECFInstanceId& Other) const
	{
		return Type != Other.Type || Id != Other.Id;
	}

	// Copy.
	FECFInstanceId& operator=(const FECFInstanceId& Other)
	{
		Id = Other.Id;
		Type = Other.Type;
		return *this;
	}

	// Move.
	FECFInstanceId& operator=(FECFInstanceId&& Other)
	{
		Id = Other.Id;
		Type = Other.Type;
		Other.Invalidate();
		return *this;
	}

	// Returns a static id. User need to give the desired Id value.
	static FECFInstanceId GetStaticId(const uint64 Id)
	{
		return FECFInstanceId(EECFInstanceType::Static, Id);
	}

	// Returns a dynamic id, which is the
	static FECFInstanceId GetDynamicId()
	{
		static uint64 DynamicIdCounter = 0;
		return FECFInstanceId(EECFInstanceType::Dynamic, ++DynamicIdCounter);
	}

protected:

	EECFInstanceType Type;
	uint64 Id;
};
