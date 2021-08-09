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

	static ConstructorHelpers::FClassFinder<UHUDMainWidget> MainHUDWidget(TEXT("WidgetBlueprint'/Game/Game/UI/HUDMain.HUDMain_C'"));

	if (MainHUDWidget.Succeeded())
	{
		m_HUDMainWidgetClass = MainHUDWidget.Class;
	}
}


void ANeopleExamGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void ANeopleExamGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(m_HUDMainWidgetClass))
	{
		m_HUDMainWidget = CreateWidget<UHUDMainWidget>(GetWorld(), m_HUDMainWidgetClass);

		if (IsValid(m_HUDMainWidget))
		{
			m_HUDMainWidget->AddToViewport();

			m_HUDMainWidget->VisibleProgressBar(false);
		}
	}
}

void ANeopleExamGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANeopleExamGameMode::VisibleProgressBar(bool IsVisible)
{
	if (IsValid(m_HUDMainWidget))
	{
		m_HUDMainWidget->VisibleProgressBar(IsVisible);
	}
}

bool ANeopleExamGameMode::IsVisibleProgressBar() const
{
	if (IsValid(m_HUDMainWidget))
	{
		return m_HUDMainWidget->IsVisibleProgressBar();
	}

	return false;
}

void ANeopleExamGameMode::SetProgressBarTime(float Time)
{
	if (IsValid(m_HUDMainWidget))
	{
		m_HUDMainWidget->SetProgressBarTime(Time);
	}
}

void ANeopleExamGameMode::ResetShotCount()
{
	if (IsValid(m_HUDMainWidget))
	{
		m_HUDMainWidget->ResetShotCount();
	}
}

void ANeopleExamGameMode::AddShotCount(int Count)
{
	if (IsValid(m_HUDMainWidget))
	{
		m_HUDMainWidget->AddShotCount(Count);
	}
}