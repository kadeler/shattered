// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SHTRDBaseWeapon.generated.h"

UCLASS()
class SHATTERED_API ASHTRDBaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASHTRDBaseWeapon();
	void Fire();
	void StopFire();
	void DestroyWeapon();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FName MuzzleSocketName = "MuzzleSocket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shooting")
    float TraceMaxDistance = 60000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shooting")
    float AttackSpeed = 0.15f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shooting")
    float AttackDamage = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shooting")
    class USoundCue* ShootingSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shooting")
    class USoundCue* HitSound;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MakeShot();

private:
	FTimerHandle ShootTimerHandle;
};
