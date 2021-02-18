// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#include "EnhancedCodeFlow.h"
#include "ECFSubsystem.h"

#include "CodeFlowActions/ECFTicker.h"
#include "CodeFlowActions/ECFTicker2.h"
#include "CodeFlowActions/ECFDelay.h"
#include "CodeFlowActions/ECFWaitAndExecute.h"
#include "CodeFlowActions/ECFTimeline.h"
#include "CodeFlowActions/ECFCustomTimeline.h"

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




FECFHandle UEnhancedCodeFlow::AddTicker(UObject* InOwner, TUniqueFunction<void(float)>&& InTickFunc)
{
	return UECFSubsystem::Get(InOwner)->AddAction<UECFTicker>(InOwner, MoveTemp(InTickFunc));
}

FECFHandle UEnhancedCodeFlow::AddTicker(UObject* InOwner, TUniqueFunction<void(float, FECFHandle)>&& InTickFunc)
{
	return UECFSubsystem::Get(InOwner)->AddAction<UECFTicker2>(InOwner, MoveTemp(InTickFunc));
}

void UEnhancedCodeFlow::RemoveAllTickers(const UObject* WorldContextObject)
{
	UECFSubsystem::Get(WorldContextObject)->RemoveActionsOfClass<UECFTicker>();
	UECFSubsystem::Get(WorldContextObject)->RemoveActionsOfClass<UECFTicker2>();
}



FECFHandle UEnhancedCodeFlow::Delay(UObject* InOwner, float InDelayTime, TUniqueFunction<void()>&& InCallbackFunc)
{
	return UECFSubsystem::Get(InOwner)->AddAction<UECFDelay>(InOwner, InDelayTime, MoveTemp(InCallbackFunc));
}

void UEnhancedCodeFlow::RemoveAllDelays(const UObject* WorldContextObject)
{
	UECFSubsystem::Get(WorldContextObject)->RemoveActionsOfClass<UECFDelay>();
}



FECFHandle UEnhancedCodeFlow::WaitAndExecute(UObject* InOwner, TUniqueFunction<bool()>&& InPredicate, TUniqueFunction<void()>&& InCallbackFunc)
{
	return UECFSubsystem::Get(InOwner)->AddAction<UECFWaitAndExecute>(InOwner, MoveTemp(InPredicate), MoveTemp(InCallbackFunc));
}

void UEnhancedCodeFlow::RemoveAllWaitAndExecutes(const UObject* WorldContextObject)
{
	UECFSubsystem::Get(WorldContextObject)->RemoveActionsOfClass<UECFWaitAndExecute>();
}



FECFHandle UEnhancedCodeFlow::AddTimeline(UObject* InOwner, float InStartValue, float InStopValue, float InTime, TUniqueFunction<void(float)>&& InTickFunc, TUniqueFunction<void(float)>&& InCallbackFunc/* = nullptr*/, EECFBlendFunc InBlendFunc/* = EECFBlendFunc::ECFBlend_Linear*/, float InBlendExp/* = 0.f*/)
{
	return UECFSubsystem::Get(InOwner)->AddAction<UECFTimeline>(InOwner, InStartValue, InStopValue, InTime, MoveTemp(InTickFunc), MoveTemp(InCallbackFunc), InBlendFunc, InBlendExp);
}

void UEnhancedCodeFlow::RemoveAllTimelines(const UObject* WorldContextObject)
{
	UECFSubsystem::Get(WorldContextObject)->RemoveActionsOfClass<UECFTimeline>();
}



FECFHandle UEnhancedCodeFlow::AddCustomTimeline(UObject* InOwner, UCurveFloat* CurveFloat, TUniqueFunction<void(float)>&& InTickFunc, TUniqueFunction<void(float)>&& InCallbackFunc/* = nullptr*/)
{
	return UECFSubsystem::Get(InOwner)->AddAction<UECFCustomTimeline>(InOwner, CurveFloat, MoveTemp(InTickFunc), MoveTemp(InCallbackFunc));
}

void UEnhancedCodeFlow::RemoveAllCustomTimelines(const UObject* WorldContextObject)
{
	UECFSubsystem::Get(WorldContextObject)->RemoveActionsOfClass<UECFCustomTimeline>();
}
