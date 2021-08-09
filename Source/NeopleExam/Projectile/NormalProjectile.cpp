// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalProjectile.h"

ANormalProjectile::ANormalProjectile()
{
	m_ProjectileType = EProjectileType::Normal;
}

void ANormalProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void ANormalProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANormalProjectile::OverlapBegin(AActor* OtherActor, const FHitResult& SweepResult)
{
	Super::OverlapBegin(OtherActor, SweepResult);
}

void ANormalProjectile::EndLifeTime()
{
	Super::EndLifeTime();
}
