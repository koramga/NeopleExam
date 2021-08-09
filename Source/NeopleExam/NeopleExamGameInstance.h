// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameInfo.h"
#include "Engine/GameInstance.h"
#include "DataTable/ProjectileTableRow.h"
#include "NeopleExamGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class NEOPLEEXAM_API UNeopleExamGameInstance : public UGameInstance
{
	GENERATED_BODY()

	UNeopleExamGameInstance();

private :
	class UDataTable* m_ProjectileDataTable;

public:
	virtual void Init();
	
public :
	const FProjectileTableRow* FindProjectileTableRow(EProjectileType ProjectileType);
};
