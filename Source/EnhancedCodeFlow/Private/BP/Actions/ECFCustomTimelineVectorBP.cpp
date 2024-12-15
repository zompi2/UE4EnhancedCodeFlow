// Copyright (c) 2024 Damian Nowakowski. All rights reserved.

#include "ECFCustomTimelineVectorBP.h"
#include "EnhancedCodeFlow.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UECFCustomTimelineVectorBP* UECFCustomTimelineVectorBP::ECFCustomTimelineVector(const UObject* WorldContextObject, UCurveVector* CurveVector, FECFActionSettings Settings, FECFHandleBP& Handle)
{
	UECFCustomTimelineVectorBP* Proxy = NewObject<UECFCustomTimelineVectorBP>();
	if (Proxy)
	{
		Proxy->Init(WorldContextObject, Settings);
		Proxy->Proxy_Handle = FFlow::AddCustomTimelineVector(WorldContextObject, CurveVector,
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
					Proxy->OnFinished.Broadcast(Value, Time, bStopped);
					Proxy->ClearAsyncBPAction();
				}
			},
		Settings);
		Handle = FECFHandleBP(Proxy->Proxy_Handle);
	}

	return Proxy;
}

ECF_PRAGMA_ENABLE_OPTIMIZATION