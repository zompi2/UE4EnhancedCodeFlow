// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "ECFActionSettings.h"
#include "BP/ECFHandleBP.h"
#include "ECFWaitAndExecuteBP.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnECFWaitAndExecuteCheck, class UECFWaitAndExecuteBP*, ActionHandler);
DECLARE_DYNAMIC_DELEGATE(FOnECFWaitAndExecuteBPFinished);

UCLASS()
class ENHANCEDCODEFLOW_API UECFWaitAndExecuteBP : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnECFWaitAndExecuteCheck OnCheck;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = "Settings"))
	static UECFWaitAndExecuteBP* ECFWaitAndExecute(UObject* WorldContextObject, const FOnECFWaitAndExecuteBPFinished& OnFinished, FECFActionSettings Settings);

	UFUNCTION(BlueprintCallable)
	void Predicate(bool bHasFinished);

	UFUNCTION(BlueprintCallable)
	FECFHandleBP GetHandle();

	void Activate() override;

protected:

	UPROPERTY(Transient)
	class UObject* Proxy_WorldContextObject;

	FOnECFWaitAndExecuteBPFinished Proxy_OnFinished;
	FECFActionSettings Proxy_Settings;

	bool Proxy_HasFinished = false;
	FECFHandle Proxy_Handle;
};
