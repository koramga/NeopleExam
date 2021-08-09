// Fill out your copyright notice in the Description page of Project Settings.


#include "NeopleExamGameInstance.h"

UNeopleExamGameInstance::UNeopleExamGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> ProjectileDataTable(TEXT("DataTable'/Game/Game/DataTable/DTProjectile.DTProjectile'"));

	if (ProjectileDataTable.Succeeded())
		m_ProjectileDataTable = ProjectileDataTable.Object;
}

void UNeopleExamGameInstance::Init()
{
	Super::Init();
}

const FProjectileTableRow* UNeopleExamGameInstance::FindProjectileTableRow(EProjectileType ProjectileType)
{
	FName Name = ConvertProjectileTypeToName(ProjectileType);

	if (Name.IsNone())
	{
		return nullptr;
	}

	return m_ProjectileDataTable->FindRow<FProjectileTableRow>(Name, TEXT(""));
}