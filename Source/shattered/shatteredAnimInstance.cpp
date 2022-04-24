// Fill out your copyright notice in the Description page of Project Settings.


#include "shatteredAnimInstance.h"
#include "MainPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

void UshatteredAnimInstance::UpdateAnimationProperties(float DeltaTime) {
	if (MainPlayer == nullptr) {
		MainPlayer = Cast<AMainPlayer>(TryGetPawnOwner());
	}
	if (MainPlayer) {
		FVector Velocity{ MainPlayer->GetVelocity() };
		Velocity.Z = 0;
		Speed = Velocity.Size();

		bIsInAir = MainPlayer->GetCharacterMovement()->IsFalling();

		bIsAccelerating = (MainPlayer->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f) ? true : false;
	}
}

void UshatteredAnimInstance::NativeInitializeAnimation()
{
	MainPlayer = Cast<AMainPlayer>(TryGetPawnOwner());
}
