// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ECFTypes.h"
#include "ExampleWidget.generated.h"

UCLASS()
class ECFEXAMPLE_API UExampleWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

	void AddToLog_Internal(FString Log);

	UFUNCTION(BlueprintImplementableEvent)
	void AddToLog(const FString& Log);

	/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

	UFUNCTION(BlueprintCallable)
	void DelayTest();

	UFUNCTION(BlueprintImplementableEvent)
	void DelayTestFinished();

	/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

	UFUNCTION(BlueprintCallable)
	void TickerTest();

	UFUNCTION(BlueprintImplementableEvent)
	void TickerTestFinished();

	UFUNCTION(BlueprintImplementableEvent)
	void SetTickerValue_BP(float NewValue);

	float TickerValue = 0.f;

	/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

	UFUNCTION(BlueprintCallable)
	void TimelineTest(EECFBlendFunc TimelineFunc, float StartValue, float StopValue);

	UFUNCTION(BlueprintImplementableEvent)
	void TimelineTestFinished();

	UFUNCTION(BlueprintImplementableEvent)
	void SetTimelineValue_BP(float NewValue, float NewTime);

	/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

	UFUNCTION(BlueprintCallable)
	void CustomTimelineTest(UCurveFloat* Curve);

	UFUNCTION(BlueprintImplementableEvent)
	void CustomTimelineTestFinished();

	UFUNCTION(BlueprintImplementableEvent)
	void SetCustomTimelineValue_BP(float NewValue, float NewTime);

	/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

	UFUNCTION(BlueprintCallable)
	void WaitAndExecuteTest();

	UFUNCTION(BlueprintImplementableEvent)
	void WaitAndExecuteTestFinished();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bWaitAndExecuteConditional = false;

	/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

	UFUNCTION(BlueprintCallable)
	void WhileTrueExecuteTest();

	UFUNCTION(BlueprintImplementableEvent)
	void WhileTrueExecuteTestFinished();

	UFUNCTION(BlueprintImplementableEvent)
	void SetWhileTrueExecuteTickerValue_BP(float NewValue);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bWhileTrueExecuteConditional = true;

	float WhileTrueExecuteTickerValue = 0.f;

	/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
};
