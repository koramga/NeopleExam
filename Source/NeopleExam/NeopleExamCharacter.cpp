// Copyright Epic Games, Inc. All Rights Reserved.

#include "NeopleExamCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NeopleExamGameMode.h"
#include "CharacterProjectile.h"

ANeopleExamCharacter::ANeopleExamCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SocketOffset = FVector(0.f,0.f,75.f);
	CameraBoom->SetRelativeRotation(FRotator(0.f,180.f,0.f));

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	
	m_ProjectileChargeTime = 0.f;
	m_CharacterProjectileType = ECharacterProjectileType::None;

	static ConstructorHelpers::FClassFinder<ACharacterProjectile> CharacterProjectileClass(TEXT("Blueprint'/Game/Game/StaticMesh/BPSphereProjectile.BPSphereProjectile_C'"));

	if (CharacterProjectileClass.Succeeded())
	{
		m_CharacterProjectileClass = CharacterProjectileClass.Class;
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ANeopleExamCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Shot", IE_Pressed, this, &ANeopleExamCharacter::InputShotPressed);
	PlayerInputComponent->BindAction("Shot", IE_Released, this, &ANeopleExamCharacter::InputShotReleased);
	PlayerInputComponent->BindAction("ReflectShot", IE_Pressed, this, &ANeopleExamCharacter::InputReflectShotPressed);
	PlayerInputComponent->BindAction("ReflectShot", IE_Released, this, &ANeopleExamCharacter::InputReflectShotReleased);
	PlayerInputComponent->BindAction("ResetShotCount", IE_Pressed, this, &ANeopleExamCharacter::InputResetShotCount);

	PlayerInputComponent->BindAxis("MoveRight", this, &ANeopleExamCharacter::MoveRight);

	//PlayerInputComponent->BindTouch(IE_Pressed, this, &ANeopleExamCharacter::TouchStarted);
	//PlayerInputComponent->BindTouch(IE_Released, this, &ANeopleExamCharacter::TouchStopped);
}

void ANeopleExamCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ANeopleExamCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ANeopleExamCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ECharacterProjectileType::None != m_CharacterProjectileType)
	{
		m_ProjectileChargeTime += DeltaTime;

		ANeopleExamGameMode* NeopleExamGameMode = Cast<ANeopleExamGameMode>(GetWorld()->GetAuthGameMode());

		if (IsValid(NeopleExamGameMode))
		{
			NeopleExamGameMode->SetProgressBarTime(m_ProjectileChargeTime);
		}
	}
}

void ANeopleExamCharacter::ShotReleased()
{
	int32 ShotCount = 0;
	float LifeTime = 3.f;

	if (ECharacterProjectileType::Normal == m_CharacterProjectileType)
	{
		PrintViewport(1.f, FColor::Red, FString::Printf(L"Normal Shot <%.2f>", m_ProjectileChargeTime));
		LOG(TEXT("Normal Shot <%.2f>"), m_ProjectileChargeTime);

		ShotCount = 1;
	}
	else if (ECharacterProjectileType::Split == m_CharacterProjectileType)
	{
		PrintViewport(1.f, FColor::Red, FString::Printf(L"Split Shot <%.2f>", m_ProjectileChargeTime));
		LOG(TEXT("Split Shot <%.2f>"), m_ProjectileChargeTime);

		ShotCount = 2;
	}
	else if (ECharacterProjectileType::Reflect == m_CharacterProjectileType)
	{
		PrintViewport(1.f, FColor::Red, FString::Printf(L"Reflect Shot <%.2f>", m_ProjectileChargeTime));
		LOG(TEXT("Reflect Shot <%.2f>"), m_ProjectileChargeTime);

		ShotCount = 1;
	}

	if (IsValid(m_CharacterProjectileClass))
	{
		LOG(TEXT("Create Projectile"));

		FActorSpawnParameters param;

		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		param.Owner = this;

		FVector ActorLocation = GetMesh()->GetSocketLocation(TEXT("ik_foot_root"));


		ActorLocation.X += GetActorForwardVector().X * 20;
		ActorLocation.Y += GetActorForwardVector().Y * 20;
		ActorLocation.Z += 50.f;

		ACharacterProjectile* Projectile = GetWorld()->SpawnActor<ACharacterProjectile>(m_CharacterProjectileClass, ActorLocation
			, GetActorRotation(), param);

		Projectile->SetProjectileType(m_CharacterProjectileType);
		Projectile->SetChargeTime(m_ProjectileChargeTime);

		if (m_ProjectileChargeTime >= 3.f)
		{
			LifeTime = 5.f;
		}

		Projectile->SetLifeTime(LifeTime);
	}

	m_CharacterProjectileType = ECharacterProjectileType::None;

	ANeopleExamGameMode* NeopleExamGameMode = Cast<ANeopleExamGameMode>(GetWorld()->GetAuthGameMode());

	if (IsValid(NeopleExamGameMode))
	{
		NeopleExamGameMode->VisibleProgressBar(false);
		NeopleExamGameMode->AddShotCount(ShotCount);
	}

	m_ProjectileChargeTime = 0.f;
}

void ANeopleExamCharacter::InputShotPressed()
{
	if (ECharacterProjectileType::None == m_CharacterProjectileType)
	{
		m_CharacterProjectileType = ECharacterProjectileType::Normal;

		ANeopleExamGameMode* NeopleExamGameMode = Cast<ANeopleExamGameMode>(GetWorld()->GetAuthGameMode());

		if (IsValid(NeopleExamGameMode))
		{
			NeopleExamGameMode->VisibleProgressBar(true);
		}
	}
}

void ANeopleExamCharacter::InputShotReleased()
{
	if (ECharacterProjectileType::Normal == m_CharacterProjectileType)
	{
		ShotReleased();
	}
}

void ANeopleExamCharacter::InputReflectShotPressed()
{
	if (ECharacterProjectileType::None == m_CharacterProjectileType)
	{
		m_CharacterProjectileType = ECharacterProjectileType::Reflect;

		ANeopleExamGameMode* NeopleExamGameMode = Cast<ANeopleExamGameMode>(GetWorld()->GetAuthGameMode());

		if (IsValid(NeopleExamGameMode))
		{
			NeopleExamGameMode->VisibleProgressBar(true);
		}
	}
	else if (ECharacterProjectileType::Normal == m_CharacterProjectileType)
	{
		if (m_ProjectileChargeTime < 1.f)
		{
			m_CharacterProjectileType = ECharacterProjectileType::Split;

			ShotReleased();
		}
	}
}

void ANeopleExamCharacter::InputReflectShotReleased()
{
	if (ECharacterProjectileType::Reflect == m_CharacterProjectileType)
	{
		ShotReleased();
	}
}

void ANeopleExamCharacter::InputResetShotCount()
{
	ANeopleExamGameMode* NeopleExamGameMode = Cast<ANeopleExamGameMode>(GetWorld()->GetAuthGameMode());

	if (IsValid(NeopleExamGameMode))
	{
		NeopleExamGameMode->ResetShotCount();
	}
}

void ANeopleExamCharacter::MoveRight(float Value)
{
	// add movement in that direction
	AddMovementInput(FVector(0.f,-1.f,0.f), Value);
}

void ANeopleExamCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// jump on any touch
	Jump();
}

void ANeopleExamCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}

