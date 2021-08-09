// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Engine.h"

/**
 * 
 */

#define FULL_CHARGETIME		3.f

DECLARE_LOG_CATEGORY_EXTERN(UE8, Log, All)
#define LOG_CALLINFO		(FString(__FUNCTION__) + TEXT("{") + FString::FromInt(__LINE__) + TEXT("}"))
#define LOG(Format, ...)	UE_LOG(UE8, Warning, TEXT("%s : %s"), *LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

void PrintViewport(float Duration, const FColor& Color, const FString& strText);


UENUM(BlueprintType)
enum class EProjectileType : uint8
{
	None,
	Normal,
	Charge,
	Split,
	Reflect,
};

FName ConvertProjectileTypeToName(EProjectileType ProjectileType);