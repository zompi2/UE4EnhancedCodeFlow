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
	if (UECFSubsystem* ECF = UECFSubsystem::Get(WorldContextObject))
		ECF->RemoveAction(Handle);
}

bool FFlow::IsActionRunning(const UObject* WorldContextObject, const FECFHandle& Handle)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(WorldContextObject))
		return ECF->HasAction(Handle);
	else
		return false;
}

void FFlow::StopAllActions(const UObject* WorldContextObject, UObject* InOwner/* = nullptr*/)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(WorldContextObject))
		ECF->RemoveAllActions();
}

/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

FECFHandle FFlow::AddTicker(UObject* InOwner, TUniqueFunction<void(float)>&& InTickFunc)
{
	return FFlow::AddTicker(InOwner, -1.f, MoveTemp(InTickFunc));
}

FECFHandle FEnhancedCodeFlow::AddTicker(UObject* InOwner, float InTickingTime, TUniqueFunction<void(float)>&& InTickFunc)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(InOwner))
		return ECF->AddAction<UECFTicker>(InOwner, MoveTemp(InTickFunc), InTickingTime);
	else
		return FECFHandle();
}

FECFHandle FFlow::AddTicker(UObject* InOwner, TUniqueFunction<void(float, FECFHandle)>&& InTickFunc)
{
	return FFlow::AddTicker(InOwner, -1.f, MoveTemp(InTickFunc));
}

FECFHandle FEnhancedCodeFlow::AddTicker(UObject* InOwner, float InTickingTime, TUniqueFunction<void(float, FECFHandle)>&& InTickFunc)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(InOwner))
		return ECF->AddAction<UECFTicker2>(InOwner, MoveTemp(InTickFunc), InTickingTime);
	else
		return FECFHandle();
}

void FFlow::RemoveAllTickers(const UObject* WorldContextObject, UObject* InOwner/* = nullptr*/)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(InOwner))
	{
		ECF->RemoveActionsOfClass<UECFTicker>(InOwner);
		ECF->RemoveActionsOfClass<UECFTicker2>(InOwner);
	}
}

/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

FECFHandle FFlow::Delay(UObject* InOwner, float InDelayTime, TUniqueFunction<void()>&& InCallbackFunc)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(InOwner))
		return ECF->AddAction<UECFDelay>(InOwner, InDelayTime, MoveTemp(InCallbackFunc));
	else
		return FECFHandle();
}

void FFlow::RemoveAllDelays(const UObject* WorldContextObject, UObject* InOwner/* = nullptr*/)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(InOwner))
		ECF->RemoveActionsOfClass<UECFDelay>(InOwner);
}

/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

FECFHandle FFlow::WaitAndExecute(UObject* InOwner, TUniqueFunction<bool()>&& InPredicate, TUniqueFunction<void()>&& InCallbackFunc)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(InOwner))
		return ECF->AddAction<UECFWaitAndExecute>(InOwner, MoveTemp(InPredicate), MoveTemp(InCallbackFunc));
	else
		return FECFHandle();
}

void FFlow::RemoveAllWaitAndExecutes(const UObject* WorldContextObject, UObject* InOwner/* = nullptr*/)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(InOwner))
		ECF->RemoveActionsOfClass<UECFWaitAndExecute>(InOwner);
}

/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

FECFHandle FFlow::WhileTrueExecute(UObject* InOwner, TUniqueFunction<bool()>&& InPredicate, TUniqueFunction<void(float)>&& InTickFunc)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(InOwner))
		return ECF->AddAction<UECFWhileTrueExecute>(InOwner, MoveTemp(InPredicate), MoveTemp(InTickFunc));
	else
		return FECFHandle();
}

void FFlow::RemoveAllWhileTrueExecutes(const UObject* WorldContextObject, UObject* InOwner/* = nullptr*/)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(InOwner))
		ECF->RemoveActionsOfClass<UECFWhileTrueExecute>(InOwner);
}

/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

FECFHandle FFlow::AddTimeline(UObject* InOwner, float InStartValue, float InStopValue, float InTime, TUniqueFunction<void(float, float)>&& InTickFunc, TUniqueFunction<void(float, float)>&& InCallbackFunc/* = nullptr*/, EECFBlendFunc InBlendFunc/* = EECFBlendFunc::ECFBlend_Linear*/, float InBlendExp/* = 0.f*/)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(InOwner))
		return ECF->AddAction<UECFTimeline>(InOwner, InStartValue, InStopValue, InTime, MoveTemp(InTickFunc), MoveTemp(InCallbackFunc), InBlendFunc, InBlendExp);
	else
		return FECFHandle();
}

void FFlow::RemoveAllTimelines(const UObject* WorldContextObject, UObject* InOwner/* = nullptr*/)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(InOwner))
		ECF->RemoveActionsOfClass<UECFTimeline>(InOwner);
}

/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

FECFHandle FFlow::AddCustomTimeline(UObject* InOwner, UCurveFloat* CurveFloat, TUniqueFunction<void(float, float)>&& InTickFunc, TUniqueFunction<void(float, float)>&& InCallbackFunc/* = nullptr*/)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(InOwner))
		return ECF->AddAction<UECFCustomTimeline>(InOwner, CurveFloat, MoveTemp(InTickFunc), MoveTemp(InCallbackFunc));
	else
		return FECFHandle();
}

void FFlow::RemoveAllCustomTimelines(const UObject* WorldContextObject, UObject* InOwner/* = nullptr*/)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(InOwner))
		ECF->RemoveActionsOfClass<UECFCustomTimeline>(InOwner);
}
