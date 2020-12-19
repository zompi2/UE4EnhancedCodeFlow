// Copyright 2021 Dark Point Games sp. z o.o. All Rights Reserved.


#include "FlowTestGameModeBase.h"
#include "CFStatics.h"

void AFlowTestGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	// Started testing

	/*
	UFlow::AddRawTicker(this, [this](float DeltaTime)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("TICK!"));
	});
	*/

	UFlow::Delay(this, 2.f, [this]()
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("DELAYED FUNCTION!"));
	});
}