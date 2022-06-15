// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SHTRDBaseCharacter.generated.h"

class UCameraComponent;
class USHTRDHealthComponent;
class USHTRDWeaponComponent;
class USpringArmComponent;
class UTextRenderComponent;

UCLASS()
class SHATTERED_API ASHTRDBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASHTRDBaseCharacter(const FObjectInitializer& ObjInit);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USHTRDHealthComponent* HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UTextRenderComponent* HealthTextComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USHTRDWeaponComponent* WeaponComponent;

    UPROPERTY(EditDefaultsOnly, Category = "DeathMontageCategory")
    UAnimMontage* DeathAnimMontage;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    bool IsRunning() const;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    bool WantsToRun = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    bool IsMovingForward = false;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMovementDirection() const;

private:
    void MoveForward(float Amount);
    void MoveRight(float Amount);
    void OnStartRunning();
    void OnStopRunning();
    void OnDeath();
    void OnHealthChanged(float Health);

};
