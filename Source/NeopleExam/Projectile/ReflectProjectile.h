// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "BaseProjectile.h"
#include "ReflectProjectile.generated.h"

/**
 * 
 */
UCLASS()
class NEOPLEEXAM_API AReflectProjectile : public ABaseProjectile
{
	GENERATED_BODY()

public :
	AReflectProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void OverlapBegin(class AActor* OtherActor, const FHitResult& SweepResult) override;
	virtual void EndLifeTime() override;
};
