// Copyright (c) 2020 Damian Nowakowski. All rights reserved.


#include "FlowTestActor.h"
#include "ECFStatics.h"

// Sets default values
AFlowTestActor::AFlowTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFlowTestActor::BeginPlay()
{
	Super::BeginPlay();

	UFlow::AddRawTicker(this, [](float DelataTime)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("ACTOR FLOW TICK"));
	});
	
}

// Called every frame
void AFlowTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("ACTOR NATIVE TICK"));
}

