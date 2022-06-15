// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SHTRDHealthComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"

// Sets default values for this component's properties
USHTRDHealthComponent::USHTRDHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void USHTRDHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	SetHealth(MaxHealth);

	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner) {
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USHTRDHealthComponent::OnTakeAnyDamage);
	}
}

void USHTRDHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser) {

    if (Damage <= 0.0f || IsDead() || !GetWorld()) return;

    SetHealth(Health - Damage);

    GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);

    if (IsDead()) {
        OnDeath.Broadcast();
    }
    else if (AutoHeal) {
        GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &USHTRDHealthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
    }
}

void USHTRDHealthComponent::HealUpdate() {
    SetHealth(Health + HealModifier);

    if (FMath::IsNearlyEqual(Health, MaxHealth) && GetWorld()) {
        GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
    }
}

void USHTRDHealthComponent::SetHealth(float NewHealth) {
    Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    OnHealthChanged.Broadcast(Health);
}
