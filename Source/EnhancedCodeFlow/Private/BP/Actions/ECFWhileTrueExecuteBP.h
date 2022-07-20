// Copyright (c) 2022 Damian Nowakowski. All rights reserved.

#pragma once

#include "../ECFActionBP.h"
#include "ECFWhileTrueExecuteBP.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnECFWhileTrueExecuteBPEvent, class UECFWhileTrueExecuteBP*, Action, float, DeltaTime);

UCLASS()
class ENHANCEDCODEFLOW_API UECFWhileTrueExecuteBP : public UECFActionBP
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnECFWhileTrueExecuteBPEvent OnWhile;

	UPROPERTY(BlueprintAssignable)
	FOnECFWhileTrueExecuteBPEvent OnExecute;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = "Settings", ToolTip = "Adds a custom timeline defined by a float curve.", DisplayName = "ECF - While True Execute"), Category = "ECF")
	static UECFWhileTrueExecuteBP* ECFWhileTrueExecute(UObject* WorldContextObject, FECFActionSettings Settings, FECFHandleBP& Handle);

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Pass 'true' to this function in order to continue, 'false' to stop."), Category = "ECF")
	void Predicate(bool bIsTrue);

protected:

	bool Proxy_IsTrue = true;
};
