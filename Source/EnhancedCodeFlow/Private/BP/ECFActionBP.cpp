// Copyright (c) 2023 Damian Nowakowski. All rights reserved.

#include "ECFActionBP.h"
#include "EnhancedCodeFlow.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

void UECFActionBP::Init(const UObject* WorldContextObject, FECFActionSettings& Settings)
{
	Proxy_WorldContextObject = WorldContextObject;
	Proxy_IsPausedAtStart = Settings.bStartPaused;
	Settings.bStartPaused = true;
}

UWorld* UECFActionBP::GetWorld() const
{
	return Proxy_WorldContextObject ? Proxy_WorldContextObject->GetWorld() : nullptr;
}

void UECFActionBP::Activate()
{
	bActivated = true;
	if (Proxy_IsPausedAtStart == false)
	{
		FFlow::ResumeAction(Proxy_WorldContextObject, Proxy_Handle);
	}
}

ECF_PRAGMA_ENABLE_OPTIMIZATION