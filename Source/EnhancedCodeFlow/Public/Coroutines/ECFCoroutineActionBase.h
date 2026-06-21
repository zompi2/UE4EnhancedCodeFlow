// Copyright (c) 2026 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFActionBase.h"
#include "ECFCoroutine.h"
#include "ECFLogs.h"
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
		// Handling a case when the owner had been destroyed before the coroutine was fully finished.
		// In such case the handle must be explicitly destroyed.
		// Ensure the action object handle is the same as the coroutine handle, as coroutines can be reused and the handle can be assigned to a different action.
		if (	bHasCoroutineHandle && 
				(HasValidOwner() == false) &&
				(CoroutineHandle.promise().bHasFinished == false) &&
				(CoroutineHandle.promise().ActionHandle.IsValid()) && 
				(CoroutineHandle.promise().ActionHandle == HandleId)
			)
		{
#if (ECF_LOGS && ECF_LOGS_VERBOSE)
			UE_LOG(LogECF, Log, TEXT("Destroying dangling coroutine when Destroying Coroutine Action for Handle: %s"), *HandleId.ToString());
#endif
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
		CoroutineHandle.promise().AssignHandle(InHandleId);
		bHasCoroutineHandle = true;

#if (ECF_LOGS && ECF_LOGS_VERBOSE)
		UE_LOG(LogECF, Log, TEXT("Sets Coroutine Action for Handle: %s"), *InHandleId.ToString());
#endif
	}
};

ECF_PRAGMA_ENABLE_OPTIMIZATION