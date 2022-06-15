// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SHTRDHUDWidget.h"
#include "Components/SHTRDHealthComponent.h"

float USHTRDHUDWidget::GetHealthPercent() const {
	const auto Player = GetOwningPlayerPawn();
	if (!Player) return 0.0f;

	const auto Component = Player->GetComponentByClass(USHTRDHealthComponent::StaticClass());
	const auto HealthComponent = Cast<USHTRDHealthComponent>(Component);
	if (!HealthComponent) return 0.0f;

	return HealthComponent->GetHealthPercent();
}