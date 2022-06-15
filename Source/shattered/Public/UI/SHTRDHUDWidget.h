// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SHTRDHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHATTERED_API USHTRDHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	float GetHealthPercent() const;
};
