// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseProjectile.h"
#include "../NeopleExamGameInstance.h"
// Sets default values
ABaseProjectile::ABaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Body = CreateDefaultSubobject<USphereComponent>(TEXT("Body"));

	SetRootComponent(m_Body);

	m_MainArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));

	m_MainArrowComponent->SetupAttachment(m_Body);
	m_MainArrowComponent->SetHiddenInGame(false);

	m_SplitArrowComponents.Add(CreateDefaultSubobject<UArrowComponent>(TEXT("SplitArrow00")));
	m_SplitArrowComponents.Add(CreateDefaultSubobject<UArrowComponent>(TEXT("SplitArrow01")));

	for (UArrowComponent* SplitArrowComponent : m_SplitArrowComponents)
	{
		SplitArrowComponent->SetupAttachment(m_Body);
		SplitArrowComponent->SetHiddenInGame(false);
		SplitArrowComponent->SetVisibility(false);
	}

	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	m_Mesh->SetupAttachment(m_Body);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/Game/StaticMesh/Sphere.Sphere'"));

	if (MeshAsset.Succeeded())
	{
		m_Mesh->SetStaticMesh(MeshAsset.Object);

		m_Mesh->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));

		m_Body->SetSphereRadius(11.f);
	}

	if (IsValid(m_Body))
	{
		m_Body->SetCollisionProfileName(TEXT("OverlapAll"));
	}

	if (IsValid(m_Mesh))
	{
		m_Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	m_ProjectileType = EProjectileType::None;

	m_InfiniteLifeTime = false;
}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	m_Body->OnComponentBeginOverlap.AddDynamic(this, &ABaseProjectile::OnOverlapBegin);

	UNeopleExamGameInstance* GameInstance = Cast<UNeopleExamGameInstance>(GetWorld()->GetGameInstance());

	if (IsValid(GameInstance))
	{
		const FProjectileTableRow* ProjectileTableRow = GameInstance->FindProjectileTableRow(m_ProjectileType);

		if (nullptr != ProjectileTableRow)
		{
			m_LifeTime = ProjectileTableRow->LifeTime;
			m_Speed = ProjectileTableRow->Speed;
		}
	}
}

// Called every frame
void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + GetActorForwardVector() * DeltaTime * m_Speed);

	if (false == m_InfiniteLifeTime)
	{
		m_LifeTime -= DeltaTime;

		if (m_LifeTime <= 0.f)
		{
			EndLifeTime();
		}
	}
}

void ABaseProjectile::OverlapBegin(AActor* OtherActor, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor))
	{
		if (OtherActor != GetOwner()
			&& OtherActor->GetOwner() != GetOwner())
		{
			Destroy();
		}
	}
}

void ABaseProjectile::EndLifeTime()
{
	Destroy();
}

void ABaseProjectile::SetInfiniteLifeTime()
{
	m_InfiniteLifeTime = true;
}

void ABaseProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OverlapBegin(OtherActor, SweepResult);
}