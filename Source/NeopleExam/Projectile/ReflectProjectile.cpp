// Fill out your copyright notice in the Description page of Project Settings.


#include "ReflectProjectile.h"

AReflectProjectile::AReflectProjectile()
{
	m_ProjectileType = EProjectileType::Reflect;
}

void AReflectProjectile::BeginPlay()
{
	Super::BeginPlay();

	m_MainArrowComponent->SetArrowColor(FLinearColor(0.f, 0.f, 1.f));
}

void AReflectProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AReflectProjectile::OverlapBegin(AActor* OtherActor, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor))
	{
		if (OtherActor != GetOwner()
			&& OtherActor->GetOwner() != GetOwner())
		{
			SetActorRotation(GetActorRotation() + FRotator(0.f, -180.f, 0.f));
		}
	}
}

void AReflectProjectile::EndLifeTime()
{
	Super::EndLifeTime();
}