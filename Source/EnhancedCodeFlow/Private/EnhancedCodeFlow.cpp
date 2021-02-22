// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#include "EnhancedCodeFlow.h"
#include "ECFSubsystem.h"

#include "CodeFlowActions/ECFTicker.h"
#include "CodeFlowActions/ECFTicker2.h"
#include "CodeFlowActions/ECFDelay.h"
#include "CodeFlowActions/ECFWaitAndExecute.h"
#include "CodeFlowActions/ECFWhileTrueExecute.h"
#include "CodeFlowActions/ECFTimeline.h"
#include "CodeFlowActions/ECFCustomTimeline.h"

void FFlow::StopAction(const UObject* WorldContextObject, FECFHandle& Handle)
{
	UECFSubsystem::Get(WorldContextObject)->RemoveAction(Handle);
}

bool FFlow::IsActionRunning(const UObject* WorldContextObject, const FECFHandle& Handle)
{
	return UECFSubsystem::Get(WorldContextObject)->HasAction(Handle);
}

void FFlow::StopAllActions(const UObject* WorldContextObject)
{
	UECFSubsystem::Get(WorldContextObject)->RemoveAllActions();
}




FECFHandle FFlow::AddTicker(UObject* InOwner, TUniqueFunction<void(float)>&& InTickFunc)
{
	return UECFSubsystem::Get(InOwner)->AddAction<UECFTicker>(InOwner, MoveTemp(InTickFunc));
}

FECFHandle FFlow::AddTicker(UObject* InOwner, TUniqueFunction<void(float, FECFHandle)>&& InTickFunc)
{
	return UECFSubsystem::Get(InOwner)->AddAction<UECFTicker2>(InOwner, MoveTemp(InTickFunc));
}

void FFlow::RemoveAllTickers(const UObject* WorldContextObject, UObject* InOwner/* = nullptr*/)
{
	UECFSubsystem::Get(WorldContextObject)->RemoveActionsOfClass<UECFTicker>(InOwner);
	UECFSubsystem::Get(WorldContextObject)->RemoveActionsOfClass<UECFTicker2>(InOwner);
}



FECFHandle FFlow::Delay(UObject* InOwner, float InDelayTime, TUniqueFunction<void()>&& InCallbackFunc)
{
	return UECFSubsystem::Get(InOwner)->AddAction<UECFDelay>(InOwner, InDelayTime, MoveTemp(InCallbackFunc));
}

void FFlow::RemoveAllDelays(const UObject* WorldContextObject, UObject* InOwner/* = nullptr*/)
{
	UECFSubsystem::Get(WorldContextObject)->RemoveActionsOfClass<UECFDelay>(InOwner);
}



FECFHandle FFlow::WaitAndExecute(UObject* InOwner, TUniqueFunction<bool()>&& InPredicate, TUniqueFunction<void()>&& InCallbackFunc)
{
	return UECFSubsystem::Get(InOwner)->AddAction<UECFWaitAndExecute>(InOwner, MoveTemp(InPredicate), MoveTemp(InCallbackFunc));
}

void FFlow::RemoveAllWaitAndExecutes(const UObject* WorldContextObject, UObject* InOwner/* = nullptr*/)
{
	UECFSubsystem::Get(WorldContextObject)->RemoveActionsOfClass<UECFWaitAndExecute>(InOwner);
}



FECFHandle FFlow::WhileTrueExecute(UObject* InOwner, TUniqueFunction<bool()>&& InPredicate, TUniqueFunction<void(float)>&& InTickFunc)
{
	return UECFSubsystem::Get(InOwner)->AddAction<UECFWhileTrueExecute>(InOwner, MoveTemp(InPredicate), MoveTemp(InTickFunc));
}

void FFlow::RemoveAllWhileTrueExecutes(const UObject* WorldContextObject, UObject* InOwner/* = nullptr*/)
{
	UECFSubsystem::Get(WorldContextObject)->RemoveActionsOfClass<UECFWhileTrueExecute>(InOwner);
}



FECFHandle FFlow::AddTimeline(UObject* InOwner, float InStartValue, float InStopValue, float InTime, TUniqueFunction<void(float)>&& InTickFunc, TUniqueFunction<void(float)>&& InCallbackFunc/* = nullptr*/, EECFBlendFunc InBlendFunc/* = EECFBlendFunc::ECFBlend_Linear*/, float InBlendExp/* = 0.f*/)
{
	return UECFSubsystem::Get(InOwner)->AddAction<UECFTimeline>(InOwner, InStartValue, InStopValue, InTime, MoveTemp(InTickFunc), MoveTemp(InCallbackFunc), InBlendFunc, InBlendExp);
}

void FFlow::RemoveAllTimelines(const UObject* WorldContextObject, UObject* InOwner/* = nullptr*/)
{
	UECFSubsystem::Get(WorldContextObject)->RemoveActionsOfClass<UECFTimeline>(InOwner);
}



FECFHandle FFlow::AddCustomTimeline(UObject* InOwner, UCurveFloat* CurveFloat, TUniqueFunction<void(float)>&& InTickFunc, TUniqueFunction<void(float)>&& InCallbackFunc/* = nullptr*/)
{
	return UECFSubsystem::Get(InOwner)->AddAction<UECFCustomTimeline>(InOwner, CurveFloat, MoveTemp(InTickFunc), MoveTemp(InCallbackFunc));
}

void FFlow::RemoveAllCustomTimelines(const UObject* WorldContextObject, UObject* InOwner/* = nullptr*/)
{
	UECFSubsystem::Get(WorldContextObject)->RemoveActionsOfClass<UECFCustomTimeline>(InOwner);
}
