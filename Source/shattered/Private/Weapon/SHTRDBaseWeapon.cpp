// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/SHTRDBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "TimerManager.h"

// Sets default values
ASHTRDBaseWeapon::ASHTRDBaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);
}

// Called when the game starts or when spawned
void ASHTRDBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponMesh);
	GEngine->AddOnScreenDebugMessage(2, 3.0f, FColor::Blue, WeaponMesh->GetName(), true);
}

void ASHTRDBaseWeapon::Fire() {
	//Automatic Fire Mode
	//GetWorld()->GetTimerManager().SetTimer(ShootTimerHandle, this, &ASHTRDBaseWeapon::MakeShot, AttackSpeed, true, 0.0f);
	//Single Fire Mode
	MakeShot();
}

void ASHTRDBaseWeapon::StopFire() {
	GetWorld()->GetTimerManager().ClearTimer(ShootTimerHandle);
}

void ASHTRDBaseWeapon::MakeShot() {
	if (!GetWorld()) return;

	const auto Player = Cast<ACharacter>(GetOwner());
	if (!Player) return;

	const auto Controller = Player->GetController<APlayerController>();
	if (!Controller) return;

	UGameplayStatics::PlaySound2D(GetWorld(), ShootingSound);

	FVector ViewLocation;
	FRotator ViewRotation;
	Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);

	const FTransform SocketTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);
	const FVector TraceStart = ViewLocation;
	const FVector ShootDirection = ViewRotation.Vector();
	const FVector TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);

	if (HitResult.bBlockingHit) {
		DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), HitResult.ImpactPoint, FColor::Red, false, 10.0f, 0, 1.0f);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Blue, false, 10.0f);

		const auto DebugStringBone = "Hit the ";
		GEngine->AddOnScreenDebugMessage(0, 3.0f, FColor::Red, DebugStringBone + HitResult.BoneName.ToString(), true);

		const auto HitTargetActor = HitResult.Actor.Get();

		GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Blue, "Owner class: " + Player->GetClass()->GetName(), true);

		if (!HitTargetActor) {
			GEngine->AddOnScreenDebugMessage(2, 3.0f, FColor::Blue, "No class", true);
			return;
		}

		const auto HitTargetClassName = HitTargetActor->GetClass()->GetName();

		GEngine->AddOnScreenDebugMessage(2, 3.0f, FColor::Blue, "Hit class: " + HitTargetClassName, true);

		if (HitTargetClassName == "BP_SHTRDEnemyCharacter_C") {
			UGameplayStatics::PlaySound2D(this, HitSound);
			UGameplayStatics::ApplyDamage(HitResult.Actor.Get(), AttackDamage, Controller, Player, nullptr);
		}
	}
	else {
		DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), TraceEnd, FColor::Red, false, 10.0f, 0, 1.0f);
	}
}

void ASHTRDBaseWeapon::DestroyWeapon() {
	Destroy();
}