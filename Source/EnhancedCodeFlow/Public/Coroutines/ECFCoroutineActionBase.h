// Copyright (c) 2024 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFActionBase.h"
#include "ECFCoroutine.h"
#include "ECFSubsystem.h"
#include "ECFCoroutineActionBase.generated.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UCLASS()
class ENHANCEDCODEFLOW_API UECFCoroutineActionBase : public UECFActionBase
{
	GENERATED_BODY()

	friend class UECFSubsystem;

protected:

	// Coroutine handle used to control the coroutine inside the Action.
	FECFCoroutineHandle CoroutineHandle;

	// Flag indicating if the coroutine handle has been set.
	bool bHasCoroutineHandle = false;

	void BeginDestroy() override
	{
		// Handling a case when the owner has beed destroyed before the coroutine has been fully finished.
		// In such case the handle must be explicitly destroyed.
		if (bHasCoroutineHandle && (HasValidOwner() == false) && (CoroutineHandle.promise().bHasFinished == false))
		{
			CoroutineHandle.promise().bHasFinished = true;
			CoroutineHandle.destroy();
		}
		Super::BeginDestroy();
	}

private:

	// Setting up action. The same as in ActionBase, but it additionally sets the coroutine handle.
	void SetCoroutineAction(const UObject* InOwner, FECFCoroutineHandle InCoroutineHandle, const FECFHandle& InHandleId, const FECFActionSettings& InSettings)
	{
		UECFActionBase::SetAction(InOwner, InHandleId, {}, InSettings);
		CoroutineHandle = InCoroutineHandle;
		bHasCoroutineHandle = true;
	}
};

ECF_PRAGMA_ENABLE_OPTIMIZATION