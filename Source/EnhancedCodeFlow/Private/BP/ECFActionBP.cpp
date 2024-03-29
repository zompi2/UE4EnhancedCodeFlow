// Copyright (c) 2024 Damian Nowakowski. All rights reserved.

#include "ECFActionBP.h"
#include "EnhancedCodeFlow.h"
#include "ECFStats.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

DEFINE_STAT(STAT_ECF_AsyncBPObjectsCount);

UECFActionBP::UECFActionBP()
{
#if STATS
	INC_DWORD_STAT(STAT_ECF_AsyncBPObjectsCount);
#endif
}

UECFActionBP::~UECFActionBP()
{
#if STATS
	DEC_DWORD_STAT(STAT_ECF_AsyncBPObjectsCount);
#endif
}

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

void UECFActionBP::ClearAsyncBPAction()
{
	SetReadyToDestroy();
#if ((ENGINE_MAJOR_VERSION == 5) && (ENGINE_MINOR_VERSION >= 0))
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

ECF_PRAGMA_ENABLE_OPTIMIZATION