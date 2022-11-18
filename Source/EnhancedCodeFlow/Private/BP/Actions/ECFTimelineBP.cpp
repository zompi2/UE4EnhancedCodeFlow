// Copyright (c) 2022 Damian Nowakowski. All rights reserved.

#include "ECFTimelineBP.h"
#include "EnhancedCodeFlow.h"

UECFTimelineBP* UECFTimelineBP::ECFTimeline(UObject* WorldContextObject, float StartValue, float StopValue, float Time, FECFActionSettings Settings, FECFHandleBP& Handle, EECFBlendFunc BlendFunc /*= EECFBlendFunc::ECFBlend_Linear*/, float BlendExp /*= 1.f*/)
{
	UECFTimelineBP* Proxy = NewObject<UECFTimelineBP>();
	Proxy->Init(WorldContextObject, Settings);

	Proxy->Proxy_Handle = FFlow::AddTimeline(WorldContextObject,
		StartValue, StopValue, Time, 
		[Proxy](float Value, float Time)
		{
			Proxy->OnTick.Broadcast(Value, Time);
		},
		[Proxy](float Value, float Time)
		{
			Proxy->OnFinished.Broadcast(Value, Time);
		}, 
		BlendFunc, BlendExp, Settings);
	Handle = FECFHandleBP(Proxy->Proxy_Handle);

	return Proxy;
}
