// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SHTRDCharacterMovementComponent.h"
#include "Player/SHTRDBaseCharacter.h"

float USHTRDCharacterMovementComponent::GetMaxSpeed() const {
    const float MaxSpeed = Super::GetMaxSpeed();
    const ASHTRDBaseCharacter* Player = Cast<ASHTRDBaseCharacter>(GetPawnOwner());
    return Player && Player->IsRunning() ? MaxSpeed * RunModifier : MaxSpeed;
}

