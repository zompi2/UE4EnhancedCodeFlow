// Copyright (c) 2024 Damian Nowakowski. All rights reserved.

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

public:

	// Ensure the coroutine handle is properly destroyed.
	// Remember, that the Promise has final_suspend set to always.
	void BeginDestroy() override
	{
		if (bHasValidCoroutineHandle)
		{
			CoroutineHandle.destroy();
			bHasValidCoroutineHandle = false;
		}
		Super::BeginDestroy();
	}

protected:

	// Coroutine handle used to control the coroutine inside the Action.
	FECFCoroutineHandle CoroutineHandle;

	// Flag indicating if the coroutine handle is valid and safe to be destroyed.
	bool bHasValidCoroutineHandle = false;

private:

	// Setting up action. The same as in ActionBase, but it additionally sets the coroutine handle.
	void SetCoroutineAction(const UObject* InOwner, FECFCoroutineHandle InCoroutineHandle, const FECFHandle& InHandleId, const FECFActionSettings& InSettings)
	{
		UECFActionBase::SetAction(InOwner, InHandleId, {}, InSettings);
		CoroutineHandle = InCoroutineHandle;
		bHasValidCoroutineHandle = true;
	}
};

ECF_PRAGMA_ENABLE_OPTIMIZATION