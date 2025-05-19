// Copyright (c) 2025 Damian Nowakowski. All rights reserved.

#pragma once

#include "../ECFActionBP.h"
#include "ECFTypes.h"
#include "ECFRunAsyncThenBP.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnECFRunAsyncThenBPEvent, bool, bTimedOut, bool, bStopped);

UCLASS()
class ENHANCEDCODEFLOW_API UECFRunAsyncThenBP : public UECFActionBP
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnECFRunAsyncThenBPEvent AsyncTask;

	UPROPERTY(BlueprintAssignable)
	FOnECFRunAsyncThenBPEvent OnExecute;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = "Settings", ToolTip = "Runs the given task function on a separate thread and calls the callback function when this task ends.\nSet TimeOut greater than 0 to stop action after this time.", DisplayName = "ECF - Run Async Then"), Category = "ECF")
	static UECFRunAsyncThenBP* ECFRunAsyncThen(const UObject* WorldContextObject, float InTimeOut, EECFAsyncPrio Priority, FECFActionSettings Settings, FECFHandleBP& Handle);
};
