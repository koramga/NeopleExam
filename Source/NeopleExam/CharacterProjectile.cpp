// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterProjectile.h"

// Sets default values
ACharacterProjectile::ACharacterProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Body = CreateDefaultSubobject<USphereComponent>(TEXT("Body"));

	SetRootComponent(m_Body);

	m_ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));

	m_ArrowComponent->SetupAttachment(m_Body);

	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	m_Mesh->SetupAttachment(m_Body);

	//m_Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
}

// Called when the game starts or when spawned
void ACharacterProjectile::BeginPlay()
{
	Super::BeginPlay();

	m_Body->OnComponentBeginOverlap.AddDynamic(this, &ACharacterProjectile::OnOverlapBegin);
}

// Called every frame
void ACharacterProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + GetActorForwardVector() * DeltaTime * 100.f);

	m_LifeTime -= DeltaTime;
	
	if (m_LifeTime <= 0.f)
	{
		Destroy();
	}
}

void ACharacterProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* Actor = SweepResult.GetActor();

	if (IsValid(OtherActor))
	{
		if (OtherActor != GetOwner()
			&& OtherActor != this)
		{
			Destroy();
			//LOG(TEXT("Overlap Destroy <%s>"), *OtherActor->GetName());
		}
	}
}

void ACharacterProjectile::SetProjectileType(ECharacterProjectileType ProjectileType)
{
	m_CharacterProjectileType = ProjectileType;

	if (ECharacterProjectileType::Normal == m_CharacterProjectileType)
	{

	}
	else if (ECharacterProjectileType::Split == m_CharacterProjectileType)
	{

	}
	else if (ECharacterProjectileType::Reflect == m_CharacterProjectileType)
	{
		m_ArrowComponent->SetArrowColor(FLinearColor(0.f, 0.f, 1.f));
	}
}

void ACharacterProjectile::SetLifeTime(float LifeTime)
{
	m_LifeTime = LifeTime;
}

void ACharacterProjectile::SetChargeTime(float ChargeTime)
{
	m_ChargeTime = ChargeTime;

	if (ECharacterProjectileType::Normal == m_CharacterProjectileType)
	{
		if (m_ChargeTime >= 3.f)
		{
			m_ArrowComponent->SetWorldScale3D(FVector(3.f, 3.f, 3.f));
		}
	}
	else if (ECharacterProjectileType::Split == m_CharacterProjectileType)
	{

	}
	else if (ECharacterProjectileType::Reflect == m_CharacterProjectileType)
	{

	}
}
