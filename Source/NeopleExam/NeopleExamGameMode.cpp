// Copyright Epic Games, Inc. All Rights Reserved.

#include "NeopleExamGameMode.h"
#include "NeopleExamCharacter.h"
#include "UObject/ConstructorHelpers.h"

ANeopleExamGameMode::ANeopleExamGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScrollerCPP/Blueprints/SideScrollerCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
