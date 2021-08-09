// Fill out your copyright notice in the Description page of Project Settings.


#include "NeopleExamPlayerController.h"

void ANeopleExamPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;

	FInputModeGameAndUI	inputMode;

	SetInputMode(inputMode); 
}