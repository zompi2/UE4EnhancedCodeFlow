// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#include "EnhancedCodeFlow.h"
#include "ECFSubsystem.h"

#include "CodeFlowNodes/ECFTicker.h"
#include "CodeFlowNodes/ECFTicker2.h"
#include "CodeFlowNodes/ECFDelay.h"
#include "CodeFlowNodes/ECFWaitAndExecute.h"

void UEnhancedCodeFlow::StopTask(FECFHandle& Handle)
{
	UECFSubsystem::Get()->RemoveNode(Handle);
}

FECFHandle UEnhancedCodeFlow::AddTicker(UObject* InOwner, TUniqueFunction<void(float)>&& InFunc)
{
	return UECFSubsystem::Get()->AddNode<UECFTicker>(InOwner, MoveTemp(InFunc));
}

FECFHandle UEnhancedCodeFlow::AddTicker(UObject* InOwner, TUniqueFunction<void(float, FECFHandle)>&& InFunc)
{
	return UECFSubsystem::Get()->AddNode<UECFTicker2>(InOwner, MoveTemp(InFunc));
}

FECFHandle UEnhancedCodeFlow::Delay(UObject* InOwner, float InDelayTime, TUniqueFunction<void()>&& InFunc)
{
	return UECFSubsystem::Get()->AddNode<UECFDelay>(InOwner, InDelayTime, MoveTemp(InFunc));
}

FECFHandle UEnhancedCodeFlow::WaitAndExecute(UObject* InOwner, TUniqueFunction<bool()>&& InPredicate, TUniqueFunction<void()>&& InFunc)
{
	return UECFSubsystem::Get()->AddNode<UECFWaitAndExecute>(InOwner, MoveTemp(InPredicate), MoveTemp(InFunc));
}
