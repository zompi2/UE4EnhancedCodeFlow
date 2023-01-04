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

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = "Settings", ToolTip = "While the specific condition is true the function will tick.\nUse Action's 'Predicate' function to control it.", DisplayName = "ECF - While True Execute"), Category = "ECF")
	static UECFWhileTrueExecuteBP* ECFWhileTrueExecute(const UObject* WorldContextObject, FECFActionSettings Settings, FECFHandleBP& Handle);

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Pass 'true' to this function in order to continue 'While True Execute' Action. Pass 'false' to stop it."), Category = "ECF")
	void Predicate(bool bIsTrue);

protected:

	bool Proxy_IsTrue = true;
};
