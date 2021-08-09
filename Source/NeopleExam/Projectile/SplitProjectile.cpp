// Fill out your copyright notice in the Description page of Project Settings.

#include "SplitProjectile.h"
#include "NormalProjectile.h"

ASplitProjectile::ASplitProjectile()
{
	m_ProjectileType = EProjectileType::Split;
}

void ASplitProjectile::BeginPlay()
{
	Super::BeginPlay();

	m_SplitArrowComponents[0]->SetRelativeRotation(FRotator(0.f, 45.f, 0.f));
	m_SplitArrowComponents[0]->SetVisibility(true);
	m_SplitArrowComponents[1]->SetRelativeRotation(FRotator(0.f, -45.f, 0.f));
	m_SplitArrowComponents[1]->SetVisibility(true);
}

void ASplitProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASplitProjectile::OverlapBegin(AActor* OtherActor, const FHitResult& SweepResult)
{
	Super::OverlapBegin(OtherActor, SweepResult);
}

void ASplitProjectile::EndLifeTime()
{
	FRotator ActorRotator = GetActorRotation();
	FVector ActorLocation = GetActorLocation();

	FActorSpawnParameters param;

	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	param.Owner = GetOwner();

	ANormalProjectile* NormalProjectile = GetWorld()->SpawnActor<ANormalProjectile>(GetActorLocation()
		, ActorRotator + m_MainArrowComponent->GetRelativeRotation(), param);

	for (UArrowComponent* SplitArrowComponent : m_SplitArrowComponents)
	{
		ANormalProjectile* SplitProjectile = GetWorld()->SpawnActor<ANormalProjectile>(GetActorLocation()
			, ActorRotator + SplitArrowComponent->GetRelativeRotation(), param);
	}

	Super::EndLifeTime();
}