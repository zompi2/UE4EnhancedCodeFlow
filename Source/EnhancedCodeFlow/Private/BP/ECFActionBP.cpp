// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#include "ECFActionBP.h"
#include "EnhancedCodeFlow.h"

void UECFActionBP::Init(UObject* WorldContextObject, FECFActionSettings& Settings)
{
	Proxy_WorldContextObject = WorldContextObject;
	Proxy_IsPausedAtStart = Settings.bStartPaused;
	Settings.bStartPaused = true;
}

void UECFActionBP::Activate()
{
	if (Proxy_IsPausedAtStart == false)
	{
		FFlow::ResumeAction(Proxy_WorldContextObject, Proxy_Handle);
	}
}
