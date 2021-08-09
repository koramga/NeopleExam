// Fill out your copyright notice in the Description page of Project Settings.


#include "ChargeProjectile.h"

AChargeProjectile::AChargeProjectile()
{
	m_ProjectileType = EProjectileType::Charge;
}

void AChargeProjectile::BeginPlay()
{
	Super::BeginPlay();

	m_MainArrowComponent->SetWorldScale3D(FVector(3.f, 3.f, 3.f));
}

void AChargeProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AChargeProjectile::OverlapBegin(AActor* OtherActor, const FHitResult& SweepResult)
{
	Super::OverlapBegin(OtherActor, SweepResult);
}

void AChargeProjectile::EndLifeTime()
{
	Super::EndLifeTime();
}
