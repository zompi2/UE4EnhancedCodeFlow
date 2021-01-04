// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#include "ECFStatics.h"
#include "ECFSubsystem.h"

#include "CodeFlowNodes/ECFRawTicker.h"
#include "CodeFlowNodes/ECFDelay.h"

uint64 UECFStatics::AddRawTicker(UObject* InOwner, TUniqueFunction<void(float)>&& InFunc)
{
	return UECFSubsystem::Get()->AddNode<UECFRawTicker>(InOwner, MoveTemp(InFunc));
}

uint64 UECFStatics::Delay(UObject* InOwner, float InDelayTime, TUniqueFunction<void()>&& InFunc)
{
	return UECFSubsystem::Get()->AddNode<UECFDelay>(InOwner, InDelayTime, MoveTemp(InFunc));
}
