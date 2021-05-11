// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "ECFActionSettings.h"
#include "BP/ECFHandleBP.h"
#include "ECFWhileTrueExecuteBP.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnECFWhileTrueExecuteBPCheck, class UECFWhileTrueExecuteBP*, ActionHandler);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnECFWhileTrueExecuteBPTick, float, DeltaTime);

UCLASS()
class ENHANCEDCODEFLOW_API UECFWhileTrueExecuteBP : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnECFWhileTrueExecuteBPCheck OnCheck;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = "Settings"))
	static UECFWhileTrueExecuteBP* ECFWhileTrueExecute(UObject* WorldContextObject, const FOnECFWhileTrueExecuteBPTick& OnTick, FECFActionSettings Settings);

	UFUNCTION(BlueprintCallable)
	void Predicate(bool bIsTrue);

	UFUNCTION(BlueprintCallable)
	FECFHandleBP GetHandle();

	void Activate() override;

protected:

	UPROPERTY(Transient)
	class UObject* Proxy_WorldContextObject;

	FOnECFWhileTrueExecuteBPTick Proxy_OnTick;
	FECFActionSettings Proxy_Settings;

	bool Proxy_IsTrue = true;
	FECFHandle Proxy_Handle;
};
