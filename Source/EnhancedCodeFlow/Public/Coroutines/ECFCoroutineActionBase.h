// Copyright (c) 2023 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFActionBase.h"
#include "ECFCoroutine.h"
#include "ECFCoroutineActionBase.generated.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UCLASS()
class ENHANCEDCODEFLOW_API UECFCoroutineActionBase : public UECFActionBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

protected:

	// Coroutine handle used to control the coroutine inside the Action
	FECFCoroutineHandle CoroutineHandle;

private:

	// Additionally sets the coroutine handle
	void SetCoroutineAction(const UObject* InOwner, FECFCoroutineHandle InCoroutineHandle, const FECFHandle& InHandleId, const FECFActionSettings& InSettings)
	{
		UECFActionBase::SetAction(InOwner, HandleId, {}, InSettings);
		CoroutineHandle = InCoroutineHandle;
	}
};

ECF_PRAGMA_ENABLE_OPTIMIZATION