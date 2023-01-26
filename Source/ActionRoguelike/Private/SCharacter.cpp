// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SAttributeComponent.h"
#include "SInteractionComponent.h"


// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComp"));

	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

}
	
// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

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

FTransform ASCharacter::CalculateTransform()
{
	/* Variables needed for LineTrace */
	
	FVector CamVector = CameraComp->GetComponentLocation();
	FRotator CamRotation = CameraComp->GetComponentRotation();
	FVector EndVector = CamVector + (CamRotation.Vector() * 10000);
	GetWorld()->LineTraceSingleByChannel(Hit, CamVector, EndVector, ECC_WorldStatic);
	if (Hit.bBlockingHit == false)
	{
		Hit.ImpactPoint = Hit.TraceEnd;
	}

	DrawDebugLine
	(
		GetWorld(),
		CamVector,
		EndVector,
		FColor::Red,
		true,
		0.0f,
		0,
		2.5f
	);


	/* Variables needed for SpawnActor */
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	FRotator SpawnRotation = (Hit.ImpactPoint - HandLocation).Rotation();
	
	DrawDebugLine
	(
		GetWorld(),
		HandLocation,
		Hit.ImpactPoint,
		FColor::Blue,
		true,
		0.0f,
		0,
		2.5f
	);
	
	return FTransform(SpawnRotation, HandLocation);
}

void ASCharacter::PrimaryAttack()
{
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f);

	PlayAnimMontage(AttackAnim);
}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
	FTransform SpawnTM = CalculateTransform();

	GetWorld()->SpawnActor<AActor>(MagicProjectileClass, SpawnTM, SpawnParams);
}

void ASCharacter::UltimateAttack()
{
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::UltimateAttack_TimeElapsed, 0.2f);

	PlayAnimMontage(AttackAnim);
}

void ASCharacter::UltimateAttack_TimeElapsed()
{
	FTransform SpawnTM = CalculateTransform();

	GetWorld()->SpawnActor<AActor>(DarkholeProjectileClass, SpawnTM, SpawnParams);
}

void ASCharacter::DashAbility()
{
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::DashAbility_TimeElapsed_SpawnProjectile, 0.2f);

	PlayAnimMontage(AttackAnim);
}

void ASCharacter::DashAbility_TimeElapsed_SpawnProjectile()
{
	FTransform SpawnTM = CalculateTransform();

	DashObjPtr = GetWorld()->SpawnActor<AActor>(DashProjectileClass, SpawnTM, SpawnParams);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::DashAbility_TimeElapsed_Teleport, 0.2f);
}

void ASCharacter::DashAbility_TimeElapsed_Teleport()
{
	if (DashObjPtr)
	{
		if (!DashObjPtr->IsHidden())
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), VFX_TeleportOut, DashObjPtr->GetActorLocation());
			DashObjPtr->AActor::SetActorHiddenInGame(true);
			
			/* Getting SProjectile's ProjectileMovementComponent */
			UProjectileMovementComponent* MovementComp = Cast<UProjectileMovementComponent>(DashObjPtr->GetComponentByClass(UProjectileMovementComponent::StaticClass()));
			if (MovementComp)
			{
				MovementComp->StopMovementImmediately();
			}

			GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::DashAbility_TimeElapsed_Teleport, 0.2f);
		}

		else
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), VFX_TeleportIn, GetActorLocation());
			TeleportTo(DashObjPtr->GetActorLocation(), GetActorRotation());
			DashObjPtr->Destroy();
		}
	}
}

void ASCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteraction();
	}

}


