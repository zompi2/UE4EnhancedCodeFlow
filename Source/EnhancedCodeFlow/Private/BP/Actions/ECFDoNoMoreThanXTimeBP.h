// Copyright (c) 2022 Damian Nowakowski. All rights reserved.

#pragma once

#include "../ECFActionBP.h"
#include "ECFDoNoMoreThanXTimeBP.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnECFDoNoMoreThanXTimeBPEvent);

UCLASS()
class ENHANCEDCODEFLOW_API UECFDoNoMoreThanXTimeBP : public UECFActionBP
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnECFDoNoMoreThanXTimeBPEvent OnExecute;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = "Settings,MaxExecsEnqueued", ToolTip = "Execute specified action no more often thant the given time. Allow to enqueue actions no more than given times.", DisplayName = "ECF - Do No More Than X Time"), Category = "ECF")
	static UECFDoNoMoreThanXTimeBP* ECFDoNoMoreThanXTime(const UObject* WorldContextObject, float Time, FECFHandleBP& Handle, UPARAM(Ref) FECFInstanceIdBP& InstanceId, FECFActionSettings Settings, int32 MaxExecsEnqueued = 1);

	bool bExecuteOnActivation = false;
	

	void Activate() override;
};
