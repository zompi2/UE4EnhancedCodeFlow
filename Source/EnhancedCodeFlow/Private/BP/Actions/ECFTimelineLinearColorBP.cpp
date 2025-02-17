// Copyright (c) 2025 Damian Nowakowski. All rights reserved.

#include "ECFTimelineLinearColorBP.h"
#include "EnhancedCodeFlow.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UECFTimelineLinearColorBP* UECFTimelineLinearColorBP::ECFTimelineLinearColor(const UObject* WorldContextObject, FLinearColor StartValue, FLinearColor StopValue, float Time, FECFActionSettings Settings, FECFHandleBP& Handle, EECFBlendFunc BlendFunc /*= EECFBlendFunc::ECFBlend_Linear*/, float BlendExp /*= 1.f*/)
{
	UECFTimelineLinearColorBP* Proxy = NewObject<UECFTimelineLinearColorBP>();
	if (Proxy)
	{
		Proxy->Init(WorldContextObject, Settings);
		Proxy->Proxy_Handle = FFlow::AddTimelineLinearColor(WorldContextObject,
			StartValue, StopValue, Time,
			[Proxy](FLinearColor Value, float Time)
			{
				if (IsProxyValid(Proxy))
				{
					Proxy->OnTick.Broadcast(Value, Time, false);
				}
			},
			[Proxy](FLinearColor Value, float Time, bool bStopped)
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