// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInfo.h"


DEFINE_LOG_CATEGORY(UE8)

void PrintViewport(float Duration, const FColor& Color, const FString& strText)
{
	GEngine->AddOnScreenDebugMessage(-1, Duration, Color, strText);
}

FName ConvertProjectileTypeToName(EProjectileType ProjectileType)
{
	switch (ProjectileType)
	{
	case EProjectileType::Normal :
		return TEXT("Normal");
	case EProjectileType::Charge:
		return TEXT("Charge");
	case EProjectileType::Split:
		return TEXT("Split");
	case EProjectileType::Reflect:
		return TEXT("Reflect");
	}

	return TEXT("");
}