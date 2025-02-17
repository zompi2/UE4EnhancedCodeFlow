// Copyright (c) 2025 Damian Nowakowski. All rights reserved.

#include "ECFCustomTimelineLinearColorBP.h"
#include "EnhancedCodeFlow.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UECFCustomTimelineLinearColorBP* UECFCustomTimelineLinearColorBP::ECFCustomTimelineLinearColor(const UObject* WorldContextObject, UCurveLinearColor* CurveLinearColor, FECFActionSettings Settings, FECFHandleBP& Handle)
{
	UECFCustomTimelineLinearColorBP* Proxy = NewObject<UECFCustomTimelineLinearColorBP>();
	if (Proxy)
	{
		Proxy->Init(WorldContextObject, Settings);
		Proxy->Proxy_Handle = FFlow::AddCustomTimelineLinearColor(WorldContextObject, CurveLinearColor,
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