// Copyright (c) 2022 Damian Nowakowski. All rights reserved.

#include "EnhancedCodeFlow.h"
#include "ECFSubsystem.h"

#include "CodeFlowActions/ECFTicker.h"
#include "CodeFlowActions/ECFTicker2.h"
#include "CodeFlowActions/ECFDelay.h"
#include "CodeFlowActions/ECFWaitAndExecute.h"
#include "CodeFlowActions/ECFWhileTrueExecute.h"
#include "CodeFlowActions/ECFTimeline.h"
#include "CodeFlowActions/ECFCustomTimeline.h"
#include "CodeFlowActions/ECFTimeLock.h"
#include "CodeFlowActions/ECFDoOnce.h"
#include "CodeFlowActions/ECFDoNTimes.h"
#include "CodeFlowActions/ECFDoNoMoreThanXTime.h"

/*^^^ ECF Flow Control Functions ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

bool FFlow::IsActionRunning(const UObject* WorldContextObject, const FECFHandle& Handle)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(WorldContextObject))
		return ECF->HasAction(Handle);
	else
		return false;
}

void FEnhancedCodeFlow::PauseAction(const UObject* WorldContextObject, const FECFHandle& Handle)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(WorldContextObject))
		ECF->PauseAction(Handle);
}

void FEnhancedCodeFlow::ResumeAction(const UObject* WorldContextObject, const FECFHandle& Handle)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(WorldContextObject))
		ECF->ResumeAction(Handle);
}

bool FEnhancedCodeFlow::IsActionPaused(const UObject* WorldContextObject, const FECFHandle& Handle, bool& bIsPaused)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(WorldContextObject))
		return ECF->IsActionPaused(Handle, bIsPaused);
	else
		return false;
}

void FEnhancedCodeFlow::SetPause(const UObject* WorldContextObject, bool bPaused)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(WorldContextObject))
		ECF->bIsECFPaused = bPaused;
}

bool FEnhancedCodeFlow::GetPause(const UObject* WorldContextObject)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(WorldContextObject))
		return ECF->bIsECFPaused;
	else
		return false;
}

/*^^^ Stop ECF Functions ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

void FFlow::StopAction(const UObject* WorldContextObject, FECFHandle& Handle, bool bComplete/* = false*/)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(WorldContextObject))
		ECF->RemoveAction(Handle, bComplete);
}

void FEnhancedCodeFlow::StopInstancedAction(const UObject* WorldContextObject, FECFInstanceId InstanceId, bool bComplete /*= false*/)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(WorldContextObject))
		ECF->RemoveInstancedAction(InstanceId, bComplete);
}

void FFlow::StopAllActions(const UObject* WorldContextObject, bool bComplete/* = false*/, UObject* InOwner/* = nullptr*/)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(WorldContextObject))
		ECF->RemoveAllActions(bComplete, InOwner);
}

/*^^^ Ticker ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

FECFHandle FFlow::AddTicker(UObject* InOwner, TUniqueFunction<void(float)>&& InTickFunc, TUniqueFunction<void()>&& InCallbackFunc/* = nullptr*/, const FECFActionSettings& Settings/* = {}*/)
{
	return FFlow::AddTicker(InOwner, -1.f, MoveTemp(InTickFunc), MoveTemp(InCallbackFunc), Settings);
}

FECFHandle FFlow::AddTicker(UObject* InOwner, float InTickingTime, TUniqueFunction<void(float)>&& InTickFunc, TUniqueFunction<void()>&& InCallbackFunc/* = nullptr*/, const FECFActionSettings& Settings/* = {}*/)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(InOwner))
		return ECF->AddAction<UECFTicker>(InOwner, Settings, FECFInstanceId(), InTickingTime, MoveTemp(InTickFunc), MoveTemp(InCallbackFunc));
	else
		return FECFHandle();
}

FECFHandle FFlow::AddTicker(UObject* InOwner, TUniqueFunction<void(float, FECFHandle)>&& InTickFunc, TUniqueFunction<void()>&& InCallbackFunc/* = nullptr*/, const FECFActionSettings& Settings/* = {}*/)
{
	return FFlow::AddTicker(InOwner, -1.f, MoveTemp(InTickFunc), MoveTemp(InCallbackFunc), Settings);
}

FECFHandle FFlow::AddTicker(UObject* InOwner, float InTickingTime, TUniqueFunction<void(float, FECFHandle)>&& InTickFunc, TUniqueFunction<void()>&& InCallbackFunc/* = nullptr*/, const FECFActionSettings& Settings/* = {}*/)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(InOwner))
		return ECF->AddAction<UECFTicker2>(InOwner, Settings, FECFInstanceId(), InTickingTime, MoveTemp(InTickFunc), MoveTemp(InCallbackFunc));
	else
		return FECFHandle();
}

void FFlow::RemoveAllTickers(const UObject* WorldContextObject, bool bComplete/* = false*/, UObject* InOwner/* = nullptr*/)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(WorldContextObject))
	{
		ECF->RemoveActionsOfClass<UECFTicker>(bComplete, InOwner);
		ECF->RemoveActionsOfClass<UECFTicker2>(bComplete, InOwner);
	}
}

/*^^^ Delay ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

FECFHandle FFlow::Delay(UObject* InOwner, float InDelayTime, TUniqueFunction<void()>&& InCallbackFunc, const FECFActionSettings& Settings/* = {}*/)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(InOwner))
		return ECF->AddAction<UECFDelay>(InOwner, Settings, FECFInstanceId(), InDelayTime, MoveTemp(InCallbackFunc));
	else
		return FECFHandle();
}

void FFlow::RemoveAllDelays(const UObject* WorldContextObject, bool bComplete/* = false*/, UObject* InOwner/* = nullptr*/)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(WorldContextObject))
		ECF->RemoveActionsOfClass<UECFDelay>(bComplete, InOwner);
}

/*^^^ Wait And Execute ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

FECFHandle FFlow::WaitAndExecute(UObject* InOwner, TUniqueFunction<bool()>&& InPredicate, TUniqueFunction<void()>&& InCallbackFunc, const FECFActionSettings& Settings/* = {}*/)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(InOwner))
		return ECF->AddAction<UECFWaitAndExecute>(InOwner, Settings, FECFInstanceId(), MoveTemp(InPredicate), MoveTemp(InCallbackFunc));
	else
		return FECFHandle();
}

void FFlow::RemoveAllWaitAndExecutes(const UObject* WorldContextObject, bool bComplete/* = false*/, UObject* InOwner/* = nullptr*/)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(WorldContextObject))
		ECF->RemoveActionsOfClass<UECFWaitAndExecute>(bComplete, InOwner);
}

/*^^^ While True Execute ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

FECFHandle FFlow::WhileTrueExecute(UObject* InOwner, TUniqueFunction<bool()>&& InPredicate, TUniqueFunction<void(float)>&& InTickFunc, const FECFActionSettings& Settings/* = {}*/)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(InOwner))
		return ECF->AddAction<UECFWhileTrueExecute>(InOwner, Settings, FECFInstanceId(), MoveTemp(InPredicate), MoveTemp(InTickFunc));
	else
		return FECFHandle();
}

void FFlow::RemoveAllWhileTrueExecutes(const UObject* WorldContextObject, bool bComplete/* = false*/, UObject* InOwner/* = nullptr*/)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(WorldContextObject))
		ECF->RemoveActionsOfClass<UECFWhileTrueExecute>(bComplete, InOwner);
}

/*^^^ Timeline ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

FECFHandle FFlow::AddTimeline(UObject* InOwner, float InStartValue, float InStopValue, float InTime, TUniqueFunction<void(float, float)>&& InTickFunc, TUniqueFunction<void(float, float)>&& InCallbackFunc/* = nullptr*/, EECFBlendFunc InBlendFunc/* = EECFBlendFunc::ECFBlend_Linear*/, float InBlendExp/* = 0.f*/, const FECFActionSettings& Settings/* = {}*/)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(InOwner))
		return ECF->AddAction<UECFTimeline>(InOwner, Settings, FECFInstanceId(), InStartValue, InStopValue, InTime, MoveTemp(InTickFunc), MoveTemp(InCallbackFunc), InBlendFunc, InBlendExp);
	else
		return FECFHandle();
}

void FFlow::RemoveAllTimelines(const UObject* WorldContextObject, bool bComplete/* = false*/, UObject* InOwner/* = nullptr*/)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(WorldContextObject))
		ECF->RemoveActionsOfClass<UECFTimeline>(bComplete, InOwner);
}

/*^^^ Custom Timeline ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

FECFHandle FFlow::AddCustomTimeline(UObject* InOwner, UCurveFloat* CurveFloat, TUniqueFunction<void(float, float)>&& InTickFunc, TUniqueFunction<void(float, float)>&& InCallbackFunc/* = nullptr*/, const FECFActionSettings& Settings/* = {}*/)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(InOwner))
		return ECF->AddAction<UECFCustomTimeline>(InOwner, Settings, FECFInstanceId(), CurveFloat, MoveTemp(InTickFunc), MoveTemp(InCallbackFunc));
	else
		return FECFHandle();
}

void FFlow::RemoveAllCustomTimelines(const UObject* WorldContextObject, bool bComplete/* = false*/, UObject* InOwner/* = nullptr*/)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(WorldContextObject))
		ECF->RemoveActionsOfClass<UECFCustomTimeline>(bComplete, InOwner);
}

/*^^^ Time Lock ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

FECFHandle FEnhancedCodeFlow::TimeLock(UObject* InOwner, float InLockTime, TUniqueFunction<void()>&& InExecFunc, const FECFInstanceId& InstanceId, const FECFActionSettings& Settings /*= {}*/)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(InOwner))
		return ECF->AddAction<UECFTimeLock>(InOwner, Settings, InstanceId, InLockTime, MoveTemp(InExecFunc));
	else
		return FECFHandle();
}

void FEnhancedCodeFlow::RemoveAllTimeLocks(const UObject* WorldContextObject, UObject* InOwner /*= nullptr*/)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(WorldContextObject))
		ECF->RemoveActionsOfClass<UECFTimeLock>(false, InOwner);
}

/*^^^ Do Once ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

FECFHandle FEnhancedCodeFlow::DoOnce(UObject* InOwner, TUniqueFunction<void()>&& InExecFunc, const FECFInstanceId& InstanceId)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(InOwner))
		return ECF->AddAction<UECFDoOnce>(InOwner, {}, InstanceId, MoveTemp(InExecFunc));
	else
		return FECFHandle();
}

void FEnhancedCodeFlow::RemoveAllDoOnce(const UObject* WorldContextObject, UObject* InOwner /*= nullptr*/)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(WorldContextObject))
		ECF->RemoveActionsOfClass<UECFDoOnce>(false, InOwner);
}

/*^^^ Do N Times ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

FECFHandle FEnhancedCodeFlow::DoNTimes(UObject* InOwner, const uint32 InTimes, TUniqueFunction<void(int32)>&& InExecFunc, const FECFInstanceId& InstanceId)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(InOwner))
		return ECF->AddAction<UECFDoNTimes>(InOwner, {}, InstanceId, InTimes, MoveTemp(InExecFunc));
	else
		return FECFHandle();
}

void FEnhancedCodeFlow::RemoveAllDoNTimes(const UObject* WorldContextObject, UObject* InOwner /*= nullptr*/)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(WorldContextObject))
		ECF->RemoveActionsOfClass<UECFDoNTimes>(false, InOwner);
}

/*^^^ Do No More Than X Time ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

FECFHandle FEnhancedCodeFlow::DoNoMoreThanXTime(UObject* InOwner, TUniqueFunction<void()>&& InExecFunc, float InTime, int32 InMaxExecsEnqueue, FECFInstanceId& InstanceId, const FECFActionSettings& Settings /*= {}*/)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(InOwner))
		return ECF->AddAction<UECFDoNoMoreThanXTime>(InOwner, Settings, InstanceId, MoveTemp(InExecFunc), InTime, InMaxExecsEnqueue);
	else
		return FECFHandle();
}

void FEnhancedCodeFlow::RemoveAllDoNoMoreThanXTimes(const UObject* WorldContextObject, UObject* InOwner /*= nullptr*/)
{
	if (UECFSubsystem* ECF = UECFSubsystem::Get(WorldContextObject))
		ECF->RemoveActionsOfClass<UECFDoNoMoreThanXTime>(false, InOwner);
}
