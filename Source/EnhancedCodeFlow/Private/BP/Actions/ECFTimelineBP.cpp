// Copyright (c) 2024 Damian Nowakowski. All rights reserved.

#include "ECFTimelineBP.h"
#include "EnhancedCodeFlow.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UECFTimelineBP* UECFTimelineBP::ECFTimeline(const UObject* WorldContextObject, float StartValue, float StopValue, float Time, FECFActionSettings Settings, FECFHandleBP& Handle, EECFBlendFunc BlendFunc /*= EECFBlendFunc::ECFBlend_Linear*/, float BlendExp /*= 1.f*/)
{
	UECFTimelineBP* Proxy = NewObject<UECFTimelineBP>();

	if (Proxy)
	{
		Proxy->Init(WorldContextObject, Settings);
		Proxy->Proxy_Handle = FFlow::AddTimeline(WorldContextObject,
			StartValue, StopValue, Time,
			[Proxy](float Value, float Time)
			{
				if (IsProxyValid(Proxy))
				{
					Proxy->OnTick.Broadcast(Value, Time, false);
				}
			},
			[Proxy](float Value, float Time, bool bStopped)
			{
				if (IsProxyValid(Proxy))
				{
					Proxy->OnFinished.Broadcast(Value, Time, false);
					Proxy->ClearAsyncBPAction();
				}
			},
			BlendFunc, BlendExp, Settings);
		Handle = FECFHandleBP(Proxy->Proxy_Handle);
	}

	return Proxy;
}

ECF_PRAGMA_ENABLE_OPTIMIZATION