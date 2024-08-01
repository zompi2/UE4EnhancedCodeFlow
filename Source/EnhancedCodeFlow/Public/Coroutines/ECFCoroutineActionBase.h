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

#ifdef __cpp_impl_coroutine
#if ECF_USE_EXPLICIT_CORO_DESTROY
	bool bHasValidCoroHandle = false;
	void BeginDestroy() override
	{
		if (bHasValidCoroHandle)
		{
			if (CoroutineHandle.promise().bDestroyed == false)
			{
				CoroutineHandle.promise().HandleCounter--;
				if (CoroutineHandle.promise().HandleCounter <= 0)
				{
					CoroutineHandle.promise().bDestroyed = true;
					CoroutineHandle.destroy();
				}
			}
		}
		Super::BeginDestroy();
	}
#endif
#endif

private:

	// Setting up action. The same as in ActionBase, but it additionally sets the coroutine handle.
	void SetCoroutineAction(const UObject* InOwner, FECFCoroutineHandle InCoroutineHandle, const FECFHandle& InHandleId, const FECFActionSettings& InSettings)
	{
		UECFActionBase::SetAction(InOwner, InHandleId, {}, InSettings);
		CoroutineHandle = InCoroutineHandle;

#ifdef __cpp_impl_coroutine
#if ECF_USE_EXPLICIT_CORO_DESTROY
		CoroutineHandle.promise().HandleCounter++;
		bHasValidCoroHandle = true;
#endif
#endif
	}
};

ECF_PRAGMA_ENABLE_OPTIMIZATION