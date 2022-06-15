// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SHTRDBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SHTRDCharacterMovementComponent.h"
#include "Components/SHTRDHealthComponent.h"
#include "Components/SHTRDWeaponComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Weapon/SHTRDBaseWeapon.h"

// Sets default values
ASHTRDBaseCharacter::ASHTRDBaseCharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<USHTRDCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(GetRootComponent());
    CameraComponent->bUsePawnControlRotation = true;

    HealthComponent = CreateDefaultSubobject<USHTRDHealthComponent>("HealthComponent");
    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
    HealthTextComponent->SetupAttachment(GetRootComponent());
    HealthTextComponent->bOwnerNoSee = true;

    WeaponComponent = CreateDefaultSubobject<USHTRDWeaponComponent>("WeaponComponent");

    GetMesh()->SetupAttachment(CameraComponent);
}

// Called when the game starts or when spawned
void ASHTRDBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

    check(HealthComponent);
    check(HealthTextComponent);
    check(GetCharacterMovement());
    
    OnHealthChanged(HealthComponent->GetHealth());
    
    HealthComponent->OnDeath.AddUObject(this, &ASHTRDBaseCharacter::OnDeath);
    HealthComponent->OnHealthChanged.AddUObject(this, &ASHTRDBaseCharacter::OnHealthChanged);
}

void ASHTRDBaseCharacter::OnHealthChanged(float Health) {
    HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

// Called every frame
void ASHTRDBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASHTRDBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &ASHTRDBaseCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASHTRDBaseCharacter::MoveRight);
    PlayerInputComponent->BindAxis("LookX", this, &ASHTRDBaseCharacter::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookY", this, &ASHTRDBaseCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASHTRDBaseCharacter::Jump);
    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASHTRDBaseCharacter::OnStartRunning);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &ASHTRDBaseCharacter::OnStopRunning);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &USHTRDWeaponComponent::Fire);
    PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &USHTRDWeaponComponent::StopFire);
}

void ASHTRDBaseCharacter::MoveForward(float Amount) {
    IsMovingForward = Amount > 0.0f;
    if (Amount == 0.0f)
        return;
    AddMovementInput(GetActorForwardVector(), Amount);
}

void ASHTRDBaseCharacter::MoveRight(float Amount) {
    if (Amount == 0.0f)
        return;
    AddMovementInput(GetActorRightVector(), Amount);
}

void ASHTRDBaseCharacter::OnStartRunning() {
    WantsToRun = true;
}

void ASHTRDBaseCharacter::OnStopRunning() {
    WantsToRun = false;
}

bool ASHTRDBaseCharacter::IsRunning() const {
    return WantsToRun && IsMovingForward && !GetVelocity().IsZero();
}

float ASHTRDBaseCharacter::GetMovementDirection() const {
    if (GetVelocity().IsZero())
        return 0.0f;

    const auto VelocityNormal = GetVelocity().GetSafeNormal();
    const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
    const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
    const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
    return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}

void ASHTRDBaseCharacter::OnDeath() {

    WeaponComponent->DestroyWeapon();
    PlayAnimMontage(DeathAnimMontage);
    GetCharacterMovement()->DisableMovement();
    SetLifeSpan(5.0f);
    if (Controller) {
        Controller->ChangeState(NAME_Spectating);
    }
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}