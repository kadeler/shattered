// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SHTRDEnemyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SHTRDCharacterMovementComponent.h"
#include "Components/SHTRDHealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/Controller.h"

// Sets default values
ASHTRDEnemyCharacter::ASHTRDEnemyCharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<USHTRDCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    HealthComponent = CreateDefaultSubobject<USHTRDHealthComponent>("HealthComponent");
    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
    HealthTextComponent->SetupAttachment(GetRootComponent());
    HealthTextComponent->bOwnerNoSee = true;

    GetMesh()->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ASHTRDEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
    check(HealthComponent);
    check(HealthTextComponent);
    check(GetCharacterMovement());

    OnHealthChanged(HealthComponent->GetHealth());

    HealthComponent->OnDeath.AddUObject(this, &ASHTRDEnemyCharacter::OnDeath);
    HealthComponent->OnHealthChanged.AddUObject(this, &ASHTRDEnemyCharacter::OnHealthChanged);
}

void ASHTRDEnemyCharacter::OnHealthChanged(float Health) {
    HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

// Called every frame
void ASHTRDEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASHTRDEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &ASHTRDEnemyCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASHTRDEnemyCharacter::MoveRight);
    PlayerInputComponent->BindAxis("LookX", this, &ASHTRDEnemyCharacter::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookY", this, &ASHTRDEnemyCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASHTRDEnemyCharacter::Jump);
}

void ASHTRDEnemyCharacter::MoveForward(float Amount) {
    IsMovingForward = Amount > 0.0f;
    if (Amount == 0.0f)
        return;
    AddMovementInput(GetActorForwardVector(), Amount);
}

void ASHTRDEnemyCharacter::MoveRight(float Amount) {
    if (Amount == 0.0f)
        return;
    AddMovementInput(GetActorRightVector(), Amount);
}

void ASHTRDEnemyCharacter::OnStartRunning() {
    WantsToRun = true;
}

void ASHTRDEnemyCharacter::OnStopRunning() {
    WantsToRun = false;
}

bool ASHTRDEnemyCharacter::IsRunning() const {
    return WantsToRun && IsMovingForward && !GetVelocity().IsZero();
}

float ASHTRDEnemyCharacter::GetMovementDirection() const {
    if (GetVelocity().IsZero())
        return 0.0f;

    const auto VelocityNormal = GetVelocity().GetSafeNormal();
    const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
    const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
    const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
    return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}

void ASHTRDEnemyCharacter::OnDeath() {
    GetCharacterMovement()->DisableMovement();
    SetLifeSpan(0.5f);
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}