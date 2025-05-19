// Copyright (c) 2025 Damian Nowakowski. All rights reserved.

#include "ECFActionBP.h"
#include "EnhancedCodeFlow.h"
#include "ECFStats.h"
#include "ECFLogs.h"
#include "Runtime/Launch/Resources/Version.h"

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
#if (ENGINE_MAJOR_VERSION == 5)
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

bool UECFActionBP::IsProxyValid(const UObject* ProxyObject)
{
	const bool bIsValid = (IsValid(ProxyObject) && (ProxyObject->HasAnyFlags(RF_BeginDestroyed | RF_FinishDestroyed) == false));
#if ECF_LOGS
	if (bIsValid == false)
	{
		UE_LOG(LogECF, Error, TEXT("Invalid Proxy Object occurred!"));
	}
#endif
	ensureAlwaysMsgf(bIsValid, TEXT("Invalid Proxy Object occurred!"));
	return bIsValid;
}

ECF_PRAGMA_ENABLE_OPTIMIZATION