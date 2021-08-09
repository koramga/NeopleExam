// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInfo.h"


DEFINE_LOG_CATEGORY(UE8)

void PrintViewport(float Duration, const FColor& Color, const FString& strText)
{
	GEngine->AddOnScreenDebugMessage(-1, Duration, Color, strText);
}