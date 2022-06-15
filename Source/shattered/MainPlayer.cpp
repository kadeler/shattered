// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/DefaultPawn.h"

// Sets default values
AMainPlayer::AMainPlayer() : BaseLookXRate(45.f), BaseLookYRate(45.f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(RootComponent);
	FollowCamera->bUsePawnControlRotation = true;

	GetMesh()->SetupAttachment(FollowCamera);

	GetCharacterMovement()->JumpZVelocity = 900.f;
	GetCharacterMovement()->AirControl = 0.7f;
	GetCharacterMovement()->GravityScale = 2.f;
}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMainPlayer::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation{ Controller->GetControlRotation() };
		const FRotator YawRotation{ 0,Rotation.Yaw,0 };
		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::X) };
		AddMovementInput(Direction, Value);
	}
}

void AMainPlayer::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation{ Controller->GetControlRotation() };
		const FRotator YawRotation{ 0,Rotation.Yaw,0 };
		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::Y) };
		AddMovementInput(Direction, Value);
	}
}

void AMainPlayer::Fire()
{
	if (shardShootingSound) {
		UGameplayStatics::PlaySound2D(this, shardShootingSound);
	}
	UE_LOG(LogTemp, Warning, TEXT("Pew!"))
	const USkeletalMeshSocket* ShootingSocket = GetMesh()->GetSocketByName("ShootingSocket");
	if (ShootingSocket)
	{
		const FTransform SocketTransform = ShootingSocket->GetSocketTransform(GetMesh());
		if (BulletFlash) {
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletFlash, SocketTransform);
		}
	}
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && ArmAnimationMontage) {
		AnimInstance->Montage_Play(ArmAnimationMontage);
		AnimInstance->Montage_JumpToSection(FName("StartFire"));
	}

}



// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainPlayer::MoveRight);
	PlayerInputComponent->BindAxis("LookX", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookY", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMainPlayer::Fire);
}

