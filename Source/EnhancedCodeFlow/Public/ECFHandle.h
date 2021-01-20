// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "CoreMinimal.h"

class FECFHandle
{

public:

	FECFHandle()
	: Handle(0)
	{
	}

	bool IsValid() const
	{
		return Handle != 0;
	}

	void Invalidate()
	{
		Handle = 0;
	}

	bool operator==(const FECFHandle& Other) const
	{
		return Handle == Other.Handle;
	}

	bool operator!=(const FECFHandle& Other) const
	{
		return Handle != Other.Handle;
	}

	FECFHandle& operator++()
	{
		Handle++;
		return *this;
	}

	FString ToString() const
	{
		return FString::Printf(TEXT("%ull"), Handle);
	}

private:

	uint64 Handle;
};