// Fill out your copyright notice in the Description page of Project Settings.


#include "SHTRDGameModeBase.h"
#include "Player/SHTRDBaseCharacter.h"
#include "Player/SHTRDPlayerController.h"
#include "UI/SHRTDHUD.h"

ASHTRDGameModeBase::ASHTRDGameModeBase() {
    DefaultPawnClass = ASHTRDBaseCharacter::StaticClass();
    PlayerControllerClass = ASHTRDPlayerController::StaticClass();
    HUDClass = ASHRTDHUD::StaticClass();
}