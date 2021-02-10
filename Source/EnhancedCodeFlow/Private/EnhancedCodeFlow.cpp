// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#include "EnhancedCodeFlow.h"
#include "ECFSubsystem.h"

#include "CodeFlowActions/ECFTicker.h"
#include "CodeFlowActions/ECFTicker2.h"
#include "CodeFlowActions/ECFDelay.h"
#include "CodeFlowActions/ECFWaitAndExecute.h"
#include "CodeFlowActions/ECFTimeline.h"

void UEnhancedCodeFlow::StopAction(FECFHandle& Handle)
{
	UECFSubsystem::Get()->RemoveAction(Handle);
}

bool UEnhancedCodeFlow::IsActionRunning(const FECFHandle& Handle)
{
	return UECFSubsystem::Get()->HasAction(Handle);
}



FECFHandle UEnhancedCodeFlow::AddTicker(UObject* InOwner, TUniqueFunction<void(float)>&& InFunc)
{
	return UECFSubsystem::Get()->AddAction<UECFTicker>(InOwner, MoveTemp(InFunc));
}

FECFHandle UEnhancedCodeFlow::AddTicker(UObject* InOwner, TUniqueFunction<void(float, FECFHandle)>&& InFunc)
{
	return UECFSubsystem::Get()->AddAction<UECFTicker2>(InOwner, MoveTemp(InFunc));
}

FECFHandle UEnhancedCodeFlow::Delay(UObject* InOwner, float InDelayTime, TUniqueFunction<void()>&& InFunc)
{
	return UECFSubsystem::Get()->AddAction<UECFDelay>(InOwner, InDelayTime, MoveTemp(InFunc));
}

FECFHandle UEnhancedCodeFlow::WaitAndExecute(UObject* InOwner, TUniqueFunction<bool()>&& InPredicate, TUniqueFunction<void()>&& InFunc)
{
	return UECFSubsystem::Get()->AddAction<UECFWaitAndExecute>(InOwner, MoveTemp(InPredicate), MoveTemp(InFunc));
}

FECFHandle UEnhancedCodeFlow::AddTimeline(UObject* InOwner, TUniqueFunction<void(float)>&& InTickFunc, TUniqueFunction<void(float)>&& InFunc, float InStartTime, float InStopTime, bool bInLoop)
{
	return UECFSubsystem::Get()->AddAction<UECFTimeline>(InOwner, MoveTemp(InTickFunc), MoveTemp(InFunc), InStartTime, InStopTime, bInLoop);
}
