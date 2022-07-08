// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "ECFActionSettings.h"
#include "BP/ECFHandleBP.h"
#include "ECFWaitAndExecuteBP.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnECFWaitAndExecuteBPCheck, class UECFWaitAndExecuteBP*, Action);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnECFWaitAndExecuteBPFinished);

UCLASS()
class ENHANCEDCODEFLOW_API UECFWaitAndExecuteBP : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnECFWaitAndExecuteBPCheck OnCheck;

	UPROPERTY(BlueprintAssignable)
	FOnECFWaitAndExecuteBPFinished OnFinished;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = "Settings"), Category = "ECF")
	static UECFWaitAndExecuteBP* ECFWaitAndExecute(UObject* WorldContextObject, FECFActionSettings Settings);

	UFUNCTION(BlueprintCallable, Category = "ECF")
	void Predicate(bool bHasFinished);

	UFUNCTION(BlueprintCallable, Category = "ECF")
	FECFHandleBP GetHandle();

	void Activate() override;

protected:

	UPROPERTY(Transient)
	class UObject* Proxy_WorldContextObject;

	FECFActionSettings Proxy_Settings;

	bool Proxy_HasFinished = false;
	FECFHandle Proxy_Handle;
};
