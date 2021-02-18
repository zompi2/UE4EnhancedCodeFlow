// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#include "EnhancedCodeFlow.h"
#include "ECFSubsystem.h"

#include "CodeFlowActions/ECFTicker.h"
#include "CodeFlowActions/ECFTicker2.h"
#include "CodeFlowActions/ECFDelay.h"
#include "CodeFlowActions/ECFWaitAndExecute.h"
#include "CodeFlowActions/ECFTimeline.h"

void UEnhancedCodeFlow::StopAction(const UObject* WorldContextObject, FECFHandle& Handle)
{
	UECFSubsystem::Get(WorldContextObject)->RemoveAction(Handle);
}

bool UEnhancedCodeFlow::IsActionRunning(const UObject* WorldContextObject, const FECFHandle& Handle)
{
	return UECFSubsystem::Get(WorldContextObject)->HasAction(Handle);
}

void UEnhancedCodeFlow::StopAllActions(const UObject* WorldContextObject)
{
	UECFSubsystem::Get(WorldContextObject)->RemoveAllActions();
}




FECFHandle UEnhancedCodeFlow::AddTicker(UObject* InOwner, TUniqueFunction<void(float)>&& InFunc)
{
	return UECFSubsystem::Get(InOwner)->AddAction<UECFTicker>(InOwner, MoveTemp(InFunc));
}

FECFHandle UEnhancedCodeFlow::AddTicker(UObject* InOwner, TUniqueFunction<void(float, FECFHandle)>&& InFunc)
{
	return UECFSubsystem::Get(InOwner)->AddAction<UECFTicker2>(InOwner, MoveTemp(InFunc));
}

void UEnhancedCodeFlow::RemoveAllTickers(const UObject* WorldContextObject)
{
	UECFSubsystem::Get(WorldContextObject)->RemoveActionsOfClass<UECFTicker>();
	UECFSubsystem::Get(WorldContextObject)->RemoveActionsOfClass<UECFTicker2>();
}



FECFHandle UEnhancedCodeFlow::Delay(UObject* InOwner, float InDelayTime, TUniqueFunction<void()>&& InFunc)
{
	return UECFSubsystem::Get(InOwner)->AddAction<UECFDelay>(InOwner, InDelayTime, MoveTemp(InFunc));
}

void UEnhancedCodeFlow::RemoveAllDelays(const UObject* WorldContextObject)
{
	UECFSubsystem::Get(WorldContextObject)->RemoveActionsOfClass<UECFDelay>();
}



FECFHandle UEnhancedCodeFlow::WaitAndExecute(UObject* InOwner, TUniqueFunction<bool()>&& InPredicate, TUniqueFunction<void()>&& InFunc)
{
	return UECFSubsystem::Get(InOwner)->AddAction<UECFWaitAndExecute>(InOwner, MoveTemp(InPredicate), MoveTemp(InFunc));
}

void UEnhancedCodeFlow::RemoveAllWaitAndExecutes(const UObject* WorldContextObject)
{
	UECFSubsystem::Get(WorldContextObject)->RemoveActionsOfClass<UECFWaitAndExecute>();
}



FECFHandle UEnhancedCodeFlow::AddTimeline(UObject* InOwner, TUniqueFunction<void(float)>&& InTickFunc, TUniqueFunction<void(float)>&& InFunc, float InStartValue, float InStopValue, float InTime, EECFBlendFunc InBlendFunc/* = EECFBlendFunc::ECFBlend_Linear*/, float InBlendExp/* = 0.f*/)
{
	return UECFSubsystem::Get(InOwner)->AddAction<UECFTimeline>(InOwner, MoveTemp(InTickFunc), MoveTemp(InFunc), InStartValue, InStopValue, InTime, InBlendFunc, InBlendExp);
}

void UEnhancedCodeFlow::RemoveAllTimelines(const UObject* WorldContextObject)
{
	UECFSubsystem::Get(WorldContextObject)->RemoveActionsOfClass<UECFTimeline>();
}
