// Copyright (c) 2022 Damian Nowakowski. All rights reserved.

#include "ECFActionBP.h"
#include "EnhancedCodeFlow.h"

void UECFActionBP::Init(UObject* WorldContextObject, FECFActionSettings& Settings)
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
