// Copyright 2021 Dark Point Games sp. z o.o. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FlowTestGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class FLOWTEST_API AFlowTestGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	void BeginPlay() override;
};
