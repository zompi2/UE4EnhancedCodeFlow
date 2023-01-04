// Copyright (c) 2022 Damian Nowakowski. All rights reserved.

#include "ECFCustomTimelineBP.h"
#include "EnhancedCodeFlow.h"

UECFCustomTimelineBP* UECFCustomTimelineBP::ECFCustomTimeline(const UObject* WorldContextObject, UCurveFloat* CurveFloat, FECFActionSettings Settings, FECFHandleBP& Handle)
{
	UECFCustomTimelineBP* Proxy = NewObject<UECFCustomTimelineBP>();
	Proxy->Init(WorldContextObject, Settings);

	Proxy->Proxy_Handle = FFlow::AddCustomTimeline(WorldContextObject, CurveFloat,
		[Proxy](float Value, float Time)
		{
			Proxy->OnTick.Broadcast(Value, Time);
		},
		[Proxy](float Value, float Time)
		{
			Proxy->OnFinished.Broadcast(Value, Time);
		},
	Settings);
	Handle = FECFHandleBP(Proxy->Proxy_Handle);

	return Proxy;
}
