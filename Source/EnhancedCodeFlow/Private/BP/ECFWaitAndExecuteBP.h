// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFActionBP.h"
#include "ECFWaitAndExecuteBP.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnECFWaitAndExecuteBPEvent, class UECFWaitAndExecuteBP*, Action);

UCLASS()
class ENHANCEDCODEFLOW_API UECFWaitAndExecuteBP : public UECFActionBP
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnECFWaitAndExecuteBPEvent OnWait;

	UPROPERTY(BlueprintAssignable)
	FOnECFWaitAndExecuteBPEvent OnExecute;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = "Settings"), Category = "ECF")
	static UECFWaitAndExecuteBP* ECFWaitAndExecute(UObject* WorldContextObject, FECFActionSettings Settings, FECFHandleBP& Handle);

	UFUNCTION(BlueprintCallable, Category = "ECF")
	void Predicate(bool bHasFinished);

protected:

	bool Proxy_HasFinished = false;
};
