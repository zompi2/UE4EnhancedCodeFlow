// Copyright (c) 2023 Damian Nowakowski. All rights reserved.

#include "ECFCustomTimelineBP.h"
#include "EnhancedCodeFlow.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

UECFCustomTimelineBP* UECFCustomTimelineBP::ECFCustomTimeline(const UObject* WorldContextObject, UCurveFloat* CurveFloat, FECFActionSettings Settings, FECFHandleBP& Handle)
{
	UECFCustomTimelineBP* Proxy = NewObject<UECFCustomTimelineBP>();
	Proxy->Init(WorldContextObject, Settings);

	Proxy->Proxy_Handle = FFlow::AddCustomTimeline(WorldContextObject, CurveFloat,
		[Proxy](float Value, float Time)
		{
			Proxy->OnTick.Broadcast(Value, Time, false);
		},
		[Proxy](float Value, float Time, bool bStopped)
		{
			Proxy->OnFinished.Broadcast(Value, Time, bStopped);
			Proxy->ClearAsyncBPAction();
		},
	Settings);
	Handle = FECFHandleBP(Proxy->Proxy_Handle);

	return Proxy;
}

ECF_PRAGMA_ENABLE_OPTIMIZATION