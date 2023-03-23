// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SAttributeComponent.h"
#include "SActionComponent.h"
#include "SAction_SpawnProjectile.h"
#include "SAction_Sprint.h"
#include "SInteractionComponent.h"


ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>(TEXT("InteractionComp"));

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComp"));

	ActionComp = CreateDefaultSubobject<USActionComponent>(TEXT("ActionComp"));
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	RageIncreasePercent = 0.5f;
}


void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealtChanged);
}


void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
}


// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASCharacter::SprintStop);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
	
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("UltimateAttack", IE_Pressed, this, &ASCharacter::UltimateAttack);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ASCharacter::DashAbility);
}


void ASCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	
	AddMovementInput(ControlRot.Vector(), Value);
}


void ASCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	
	AddMovementInput(RightVector, Value);
}


void ASCharacter::SprintStart()
{
	ActionComp->StartActionByName(this, "Sprint");
}


void ASCharacter::SprintStop()
{
	ActionComp->StopActionByName(this, "Sprint");
}


void ASCharacter::PrimaryAttack()
{
	ActionComp->StartActionByName(this, "PrimaryAttack");
}


void ASCharacter::UltimateAttack()
{
	ActionComp->StartActionByName(this, "BlackHole");
}


void ASCharacter::DashAbility()
{
	ActionComp->StartActionByName(this, "Dash");
}


void ASCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteraction();
	}
}


void ASCharacter::GetActorEyesViewPoint(FVector& OutLocation, FRotator& Rotation) const
{
	Super::GetActorEyesViewPoint(OutLocation, Rotation);

	OutLocation = CameraComp->GetComponentLocation();
}


FVector ASCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}


void ASCharacter::OnHealtChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);

		if (NewHealth <= 0.0f)
		{
			APlayerController* PC = Cast<APlayerController>(GetController());
			if (PC)
			{
				DisableInput(PC);
			}
		}
		
		AttributeComp->SetRage(Delta * RageIncreasePercent);
	}
}


void ASCharacter::HealSelf(float Amount /* = 100.0f */)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}