// Copyright (c) 2026 Damian Nowakowski. All rights reserved.

#pragma once

#include "../ECFActionBP.h"
#include "ECFLoadObjectsAsyncBP.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnECFLoadObjectsAsyncBPEvent, bool, bStopped);

UCLASS()
class ENHANCEDCODEFLOW_API UECFLoadObjectsAsyncBP : public UECFActionBP
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnECFLoadObjectsAsyncBPEvent OnComplete;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = "Settings", ToolTip = "Asynchronously load a list of assets and execute callback when all are loaded.", DisplayName = "ECF - Load Objects Async"), Category = "ECF")
	static UECFLoadObjectsAsyncBP* ECFLoadObjectsAsync(const UObject* WorldContextObject, const TArray<FSoftObjectPath>& ObjectsToLoad, FECFActionSettings Settings, FECFHandleBP& Handle);
};
