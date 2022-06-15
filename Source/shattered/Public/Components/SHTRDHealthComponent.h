// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SHTRDHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeath)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHATTERED_API USHTRDHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USHTRDHealthComponent();

    FOnDeath OnDeath;
    FOnHealthChanged OnHealthChanged;

    UFUNCTION(BlueprintCallable, Category="Health")
    float GetHealthPercent() const { return (Health/MaxHealth-1)*-1; }

    UFUNCTION(BlueprintCallable)
    bool IsDead() const { return FMath::IsNearlyZero(Health); }

    float GetHealth() const { return Health; }
    

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
    float MaxHealth = 100.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Autoheal")
    bool AutoHeal = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Autoheal", meta = (EditCondition = "AutoHeal"))
    float HealUpdateTime = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Autoheal", meta = (EditCondition = "AutoHeal"))
    float HealDelay = 3.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Autoheal", meta = (EditCondition = "AutoHeal"))
    float HealModifier = 5.0f;

	// Called when the game starts
	virtual void BeginPlay() override;

private:	
	float Health = 0.0f;
    FTimerHandle HealTimerHandle;

    UFUNCTION()
    void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
    
	void HealUpdate();
    void SetHealth(float NewHealth);
};
