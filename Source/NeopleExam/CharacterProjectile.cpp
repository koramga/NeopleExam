// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterProjectile.h"
#include "NeopleExamGameMode.h"

// Sets default values
ACharacterProjectile::ACharacterProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Body = CreateDefaultSubobject<USphereComponent>(TEXT("Body"));

	SetRootComponent(m_Body);

	m_MainArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	
	m_MainArrowComponent->SetupAttachment(m_Body);

	m_SplitArrowComponents.Add(CreateDefaultSubobject<UArrowComponent>(TEXT("SplitArrow00")));
	m_SplitArrowComponents.Add(CreateDefaultSubobject<UArrowComponent>(TEXT("SplitArrow01")));

	for (UArrowComponent* SplitArrowComponent : m_SplitArrowComponents)
	{
		SplitArrowComponent->SetupAttachment(m_Body);
		SplitArrowComponent->SetVisibility(false);
	}

	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	m_Mesh->SetupAttachment(m_Body);

	m_CharacterProjectileType = ECharacterProjectileType::Normal;
	m_LifeTime = 3.f;

	//m_Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
}

// Called when the game starts or when spawned
void ACharacterProjectile::BeginPlay()
{
	Super::BeginPlay();

	ANeopleExamGameMode* NeopleExamGameMode = Cast<ANeopleExamGameMode>(GetWorld()->GetAuthGameMode());

	NeopleExamGameMode->AddShotCount(1);

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
		if (ECharacterProjectileType::Split == m_CharacterProjectileType)
		{
			//3개를 만들어야한다.

			if (IsValid(m_CharacterProjectileClass))
			{
				FRotator ActorRotator = GetActorRotation();
				FVector ActorLocation = GetActorLocation();

				FActorSpawnParameters param;

				param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				param.Owner = GetOwner();

				ACharacterProjectile* MainProjectile = GetWorld()->SpawnActor<ACharacterProjectile>(m_CharacterProjectileClass, GetActorLocation()
					, ActorRotator + m_MainArrowComponent->GetRelativeRotation(), param);

				MainProjectile->InitProjectileType(ECharacterProjectileType::Normal, m_CharacterProjectileClass);

				for (UArrowComponent* SplitArrowComponent : m_SplitArrowComponents)
				{
					ACharacterProjectile* SplitProjectile = GetWorld()->SpawnActor<ACharacterProjectile>(m_CharacterProjectileClass, GetActorLocation()
						, ActorRotator + SplitArrowComponent->GetRelativeRotation(), param);

					SplitProjectile->InitProjectileType(ECharacterProjectileType::Normal, m_CharacterProjectileClass);
				}
			}
		}

		Destroy();
	}
}

void ACharacterProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* Actor = SweepResult.GetActor();

	if (IsValid(OtherActor))
	{
		if (OtherActor != GetOwner()
			&& OtherActor->GetOwner() != GetOwner())
		{
			if (ECharacterProjectileType::Reflect == m_CharacterProjectileType)
			{
				SetActorRotation(GetActorRotation() + FRotator(0.f, -180.f, 0.f));
			}
			else
			{
				Destroy();
			}
		}
	}
}

void ACharacterProjectile::InitProjectileType(ECharacterProjectileType ProjectileType, TSubclassOf<ACharacterProjectile>& ProjectileClass)
{
	m_CharacterProjectileType = ProjectileType;
	m_CharacterProjectileClass = ProjectileClass;

	if (ECharacterProjectileType::Normal == m_CharacterProjectileType)
	{

	}
	else if (ECharacterProjectileType::Charge == m_CharacterProjectileType)
	{
		m_MainArrowComponent->SetWorldScale3D(FVector(3.f, 3.f, 3.f));
		m_LifeTime = 5.f;
	}
	else if (ECharacterProjectileType::Split == m_CharacterProjectileType)
	{
		m_SplitArrowComponents[0]->SetRelativeRotation(FRotator(0.f, 45.f, 0.f));
		m_SplitArrowComponents[0]->SetVisibility(true);
		m_SplitArrowComponents[1]->SetRelativeRotation(FRotator(0.f, -45.f, 0.f));
		m_SplitArrowComponents[1]->SetVisibility(true);
	}
	else if (ECharacterProjectileType::Reflect == m_CharacterProjectileType)
	{
		m_MainArrowComponent->SetArrowColor(FLinearColor(0.f, 0.f, 1.f));
		m_LifeTime = 5.f;
	}
}