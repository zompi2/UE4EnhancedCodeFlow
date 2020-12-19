// Copyright (c) 2020 Damian Nowakowski. All rights reserved.

#include "CFStatics.h"
#include "CFSubsystem.h"

#include "FlowNodes/CFRawTicker.h"
#include "FlowNodes/CFDelay.h"

uint64 UCFStatics::AddRawTicker(UObject* InOwner, TUniqueFunction<void(float)>&& InFunc)
{
	return UCFSubsystem::Get()->AddNode<UCFRawTicker>(InOwner, [&](UCFRawTicker* NewNode) 
	{
		NewNode->Func = MoveTemp(InFunc);
	});
}

uint64 UCFStatics::Delay(UObject* InOwner, float InDelayTime, TUniqueFunction<void()>&& InFunc)
{
	return UCFSubsystem::Get()->AddNode<UCFDelay>(InOwner, [&](UCFDelay* NewNode)
	{
		NewNode->Func = MoveTemp(InFunc);
		NewNode->DelayTime = InDelayTime;
	});
}
