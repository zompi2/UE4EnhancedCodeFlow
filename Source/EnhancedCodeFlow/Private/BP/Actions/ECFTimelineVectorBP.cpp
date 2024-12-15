// Copyright (c) 2024 Damian Nowakowski. All rights reserved.

#include "ECFTimelineVectorBP.h"
#include "EnhancedCodeFlow.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UECFTimelineVectorBP* UECFTimelineVectorBP::ECFTimelineVector(const UObject* WorldContextObject, FVector StartValue, FVector StopValue, float Time, FECFActionSettings Settings, FECFHandleBP& Handle, EECFBlendFunc BlendFunc /*= EECFBlendFunc::ECFBlend_Linear*/, float BlendExp /*= 1.f*/)
{
	UECFTimelineVectorBP* Proxy = NewObject<UECFTimelineVectorBP>();
	if (Proxy)
	{
		Proxy->Init(WorldContextObject, Settings);
		Proxy->Proxy_Handle = FFlow::AddTimelineVector(WorldContextObject,
			StartValue, StopValue, Time,
			[Proxy](FVector Value, float Time)
			{
				if (IsProxyValid(Proxy))
				{
					Proxy->OnTick.Broadcast(Value, Time, false);
				}
			},
			[Proxy](FVector Value, float Time, bool bStopped)
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