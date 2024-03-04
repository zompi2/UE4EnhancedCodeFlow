// Copyright (c) 2024 Damian Nowakowski. All rights reserved.

#include "ECFInstanceId.h"

uint64 FECFInstanceId::DynamicIdCounter = 0;

FECFInstanceId FECFInstanceId::NewId()
{
	DynamicIdCounter++;
	return FECFInstanceId(DynamicIdCounter);
}