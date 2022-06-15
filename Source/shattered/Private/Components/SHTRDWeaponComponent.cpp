// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SHTRDWeaponComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/Character.h"
#include "Weapon/SHTRDBaseWeapon.h"

// Sets default values for this component's properties
USHTRDWeaponComponent::USHTRDWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void USHTRDWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
    GEngine->AddOnScreenDebugMessage(2, 3.0f, FColor::Blue, "Weapon Spawned", true);
	SpawnWeapon();
}

// Called every frame
void USHTRDWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USHTRDWeaponComponent::SpawnWeapon() {
    if (!GetWorld()) return;

    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;

    CurrentWeapon = GetWorld()->SpawnActor<ASHTRDBaseWeapon>(WeaponClass);
    if (!CurrentWeapon) return;

    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    CurrentWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, WeaponAttachPointName);
    CurrentWeapon->SetOwner(Character);
}

void USHTRDWeaponComponent::Fire() {
    if (!CurrentWeapon) return;
    CurrentWeapon->Fire();
}

void USHTRDWeaponComponent::StopFire() {
    if (!CurrentWeapon) return;
    CurrentWeapon->StopFire();
}

void USHTRDWeaponComponent::DestroyWeapon() {
    if (!CurrentWeapon) return;
    CurrentWeapon->DestroyWeapon();
}
