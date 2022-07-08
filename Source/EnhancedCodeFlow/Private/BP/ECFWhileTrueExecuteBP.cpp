// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#include "ECFWhileTrueExecuteBP.h"
#include "EnhancedCodeFlow.h"

UECFWhileTrueExecuteBP* UECFWhileTrueExecuteBP::ECFWhileTrueExecute(UObject* WorldContextObject, const FOnECFWhileTrueExecuteBPTick& OnTick, FECFActionSettings Settings)
{
	UECFWhileTrueExecuteBP* Proxy = NewObject<UECFWhileTrueExecuteBP>();

	Proxy->Proxy_WorldContextObject = WorldContextObject;
	Proxy->Proxy_OnTick = OnTick;
	Proxy->Proxy_Settings = Settings;
	Proxy->Proxy_IsTrue = true;

	return Proxy;
}

void UECFWhileTrueExecuteBP::Activate()
{
	Proxy_Handle = FFlow::WhileTrueExecute(Proxy_WorldContextObject, 
	[this]()
	{
		OnCheck.Broadcast(this);
		return Proxy_IsTrue;
	},
	[this](float DeltaTime)
	{
		Proxy_OnTick.Execute(DeltaTime);
	}, Proxy_Settings);
}

void UECFWhileTrueExecuteBP::Predicate(bool bIsTrue)
{
	Proxy_IsTrue = bIsTrue;
}

FECFHandleBP UECFWhileTrueExecuteBP::GetHandle()
{
	return FECFHandleBP(Proxy_Handle);
}