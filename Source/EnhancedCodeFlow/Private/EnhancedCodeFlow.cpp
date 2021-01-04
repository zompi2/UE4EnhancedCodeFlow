// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#include "EnhancedCodeFlow.h"
#include "ECFSubsystem.h"

#include "CodeFlowNodes/ECFRawTicker.h"
#include "CodeFlowNodes/ECFDelay.h"

void UEnhancedCodeFlow::StopTask(FECFNodeHandle Handle)
{
	UECFSubsystem::Get()->RemoveNode(Handle);
}

FECFNodeHandle UEnhancedCodeFlow::AddRawTicker(UObject* InOwner, TUniqueFunction<void(float)>&& InFunc)
{
	return UECFSubsystem::Get()->AddNode<UECFRawTicker>(InOwner, MoveTemp(InFunc));
}

FECFNodeHandle UEnhancedCodeFlow::Delay(UObject* InOwner, float InDelayTime, TUniqueFunction<void()>&& InFunc)
{
	return UECFSubsystem::Get()->AddNode<UECFDelay>(InOwner, InDelayTime, MoveTemp(InFunc));
}
