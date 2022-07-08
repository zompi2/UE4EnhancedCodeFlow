// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "ECFActionSettings.h"
#include "BP/ECFHandleBP.h"
#include "ECFActionBP.generated.h"

UCLASS()
class ENHANCEDCODEFLOW_API UECFActionBP : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	virtual void Init(UObject* WorldContextObject, FECFActionSettings& Settings);
	void Activate() override;

protected:

	UPROPERTY(Transient)
	class UObject* Proxy_WorldContextObject;

	FECFHandle Proxy_Handle;
	bool Proxy_IsPausedAtStart;
};
