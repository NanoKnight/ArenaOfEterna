// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestIndicator.generated.h"

/**
 * 
 */
class UImage;
class AQuestActor;

UCLASS()
class PROJECTX_API UQuestIndicator : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget) , BlueprintReadWrite)
	UImage* Icon;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	AQuestActor* QuestActor;

	void UpdateIndicator();
	virtual void NativeTick(const FGeometry& MyGeometry,float InDeltaTime)override;

protected:

	FTimerHandle IndicatorTimer;
	virtual void NativeConstruct() override;

	void SetQuestActor();

	UPROPERTY(EditAnywhere, Category = "Quest")
	float VerticalOffset = 100.f;

private:


	
};
