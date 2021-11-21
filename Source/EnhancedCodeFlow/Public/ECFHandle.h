// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "CoreMinimal.h"

class FECFHandle
{

public:

	FECFHandle() :
		Handle(0)
	{
	}

	FECFHandle(const FECFHandle& Other) :
		Handle(Other.Handle)
	{
	}

	FECFHandle(FECFHandle&& Other) :
		Handle(Other.Handle)
	{
		Other.Invalidate();
	}

	// Checks if the handle is valid.
	bool IsValid() const
	{
		return Handle != 0;
	}

	// Makes this handle invalid (invalidates it).
	void Invalidate()
	{
		Handle = 0;
	}

	// Compare handles.
	bool operator==(const FECFHandle& Other) const
	{
		return Handle == Other.Handle;
	}

	// Compare (not) handles.
	bool operator!=(const FECFHandle& Other) const
	{
		return Handle != Other.Handle;
	}

	// Copy.
	FECFHandle& operator=(const FECFHandle& Other)
	{
		Handle = Other.Handle;
		return *this;
	}

	// Move.
	FECFHandle& operator=(FECFHandle&& Other)
	{
		Handle = Other.Handle;
		Other.Invalidate();
		return *this;
	}

	// Increase handle.
	FECFHandle& operator++()
	{
		Handle++;
		return *this;
	}

	// Convert the handle to string.
	FString ToString() const
	{
		return FString::Printf(TEXT("%ull"), Handle);
	}

protected:

	uint64 Handle;
};