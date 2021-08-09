// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameInfo.h"
#include "GameFramework/PlayerController.h"
#include "NeopleExamPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class NEOPLEEXAM_API ANeopleExamPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
