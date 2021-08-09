// Copyright Epic Games, Inc. All Rights Reserved.

#include "NeopleExamCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NeopleExamGameMode.h"

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
	
	m_ShotTime = 0.f;
	m_CharacterShotType = ECharacterShotType::None;
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

	if (ECharacterShotType::None != m_CharacterShotType)
	{
		m_ShotTime += DeltaTime;

		ANeopleExamGameMode* NeopleExamGameMode = Cast<ANeopleExamGameMode>(GetWorld()->GetAuthGameMode());

		if (IsValid(NeopleExamGameMode))
		{
			NeopleExamGameMode->SetProgressBarTime(m_ShotTime);
		}
	}
}

void ANeopleExamCharacter::ShotReleased()
{
	int32 ShotCount = 0;

	if (ECharacterShotType::Normal == m_CharacterShotType)
	{
		PrintViewport(1.f, FColor::Red, FString::Printf(L"Normal Shot <%.2f>", m_ShotTime));
		LOG(TEXT("Normal Shot <%.2f>"), m_ShotTime);


		ShotCount = 1;
	}
	else if (ECharacterShotType::Split == m_CharacterShotType)
	{
		PrintViewport(1.f, FColor::Red, FString::Printf(L"Split Shot <%.2f>", m_ShotTime));
		LOG(TEXT("Split Shot <%.2f>"), m_ShotTime);

		ShotCount = 2;
	}
	else if (ECharacterShotType::Reflect == m_CharacterShotType)
	{

		PrintViewport(1.f, FColor::Red, FString::Printf(L"Reflect Shot <%.2f>", m_ShotTime));
		LOG(TEXT("Reflect Shot <%.2f>"), m_ShotTime);

		ShotCount = 1;
	}

	m_CharacterShotType = ECharacterShotType::None;

	ANeopleExamGameMode* NeopleExamGameMode = Cast<ANeopleExamGameMode>(GetWorld()->GetAuthGameMode());

	if (IsValid(NeopleExamGameMode))
	{
		NeopleExamGameMode->VisibleProgressBar(false);
		NeopleExamGameMode->AddShotCount(ShotCount);
	}

	m_ShotTime = 0.f;
}

void ANeopleExamCharacter::InputShotPressed()
{
	if (ECharacterShotType::None == m_CharacterShotType)
	{
		m_CharacterShotType = ECharacterShotType::Normal;

		ANeopleExamGameMode* NeopleExamGameMode = Cast<ANeopleExamGameMode>(GetWorld()->GetAuthGameMode());

		if (IsValid(NeopleExamGameMode))
		{
			NeopleExamGameMode->VisibleProgressBar(true);
		}
	}
}

void ANeopleExamCharacter::InputShotReleased()
{
	if (ECharacterShotType::Normal == m_CharacterShotType)
	{
		ShotReleased();
	}
}

void ANeopleExamCharacter::InputReflectShotPressed()
{
	if (ECharacterShotType::None == m_CharacterShotType)
	{
		m_CharacterShotType = ECharacterShotType::Reflect;

		ANeopleExamGameMode* NeopleExamGameMode = Cast<ANeopleExamGameMode>(GetWorld()->GetAuthGameMode());

		if (IsValid(NeopleExamGameMode))
		{
			NeopleExamGameMode->VisibleProgressBar(true);
		}
	}
	else if (ECharacterShotType::Normal == m_CharacterShotType)
	{
		if (m_ShotTime < 1.f)
		{
			m_CharacterShotType = ECharacterShotType::Split;

			ShotReleased();
		}
	}
}

void ANeopleExamCharacter::InputReflectShotReleased()
{
	if (ECharacterShotType::Reflect == m_CharacterShotType)
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

