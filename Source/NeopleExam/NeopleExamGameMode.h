// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UI/HUDMainWidget.h"
#include "NeopleExamGameMode.generated.h"

UCLASS(minimalapi)
class ANeopleExamGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ANeopleExamGameMode();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"));
	TSubclassOf<UHUDMainWidget>	m_HUDMainWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"));
	UHUDMainWidget* m_HUDMainWidget;

protected:

	/**
	 * Initialize the game.
	 * The GameMode's InitGame() event is called before any other functions (including PreInitializeComponents() )
	 * and is used by the GameMode to initialize parameters and spawn its helper classes.
	 * @warning: this is called before actors' PreInitializeComponents.
	 */
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public :
	void VisibleProgressBar(bool IsVisible);
	bool IsVisibleProgressBar() const;
	void SetProgressBarTime(float Time);
	void ResetShotCount();
	void AddShotCount(int Count);
};



