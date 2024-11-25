// Copyright Epic Games, Inc. All Rights Reserved.

#include "Projet_AICharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Actor/RecipeItem.h"
#include "Components/SphereComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AProjet_AICharacter

AProjet_AICharacter::AProjet_AICharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	StimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulis"));

	StimuliSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
	StimuliSource->RegisterWithPerceptionSystem();
	
}

void AProjet_AICharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AProjet_AICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AProjet_AICharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AProjet_AICharacter::Look);

		//Grab
		EnhancedInputComponent->BindAction(GrabAction, ETriggerEvent::Triggered,this,&AProjet_AICharacter::Grab);

		//Drop
		EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Triggered,this,&AProjet_AICharacter::Drop);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

bool AProjet_AICharacter::GetTriggerGrab()
{
	return triggerGrab;
}

bool AProjet_AICharacter::GetTriggerShrug()
{
	return triggerShrug;
}

bool AProjet_AICharacter::GetTriggerNotify()
{
	return  triggerNotify;
}

void AProjet_AICharacter::SetTriggerGrab(bool value)
{
	triggerGrab = value;
}

void AProjet_AICharacter::SetTriggerShrug(bool value)
{
	triggerShrug = value;
}

void AProjet_AICharacter::SetTriggerNotify(bool value)
{
	triggerNotify = value;
}

void AProjet_AICharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AProjet_AICharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AProjet_AICharacter::Grab()
{
	FVector Start, LineTraceEnd, ForwardVector;
	FHitResult HitResult;

	Start = FollowCamera->GetComponentLocation();

	ForwardVector = FollowCamera->GetForwardVector();

	Start = Start + (ForwardVector * 450);
	LineTraceEnd = Start + (ForwardVector * 10000);
	


	
	bool bSuccess = Controller->GetWorld()->LineTraceSingleByChannel(HitResult,Start,LineTraceEnd,ECollisionChannel::ECC_WorldDynamic);
	const FVector impact = FVector(HitResult.ImpactPoint.X,HitResult.ImpactPoint.Y,HitResult.ImpactPoint.Z);
	const TConstArrayView<FVector> points = {Start,impact};
	TArray<FVector> test = {Start,impact};
	DrawCentripetalCatmullRomSpline(GetWorld(),{Start,impact},FColor::Blue,0.5,8,true,2,0,2);
	if(auto hitActor = Cast<ARecipeItem>(HitResult.HitObjectHandle.FetchActor()))
	{
		if(FVector::Dist(hitActor->GetActorLocation(), GetActorLocation()) < 300)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("true"));
			triggerGrab = true;
		}
	}
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Grab"));
	
	
}

void AProjet_AICharacter::attatchIngredient()
{
	FVector Start, LineTraceEnd, ForwardVector;
	FHitResult HitResult;

	Start = FollowCamera->GetComponentLocation();

	ForwardVector = FollowCamera->GetForwardVector();

	Start = Start + (ForwardVector * 450);
	LineTraceEnd = Start + (ForwardVector * 10000);
	


	
	bool bSuccess = Controller->GetWorld()->LineTraceSingleByChannel(HitResult,Start,LineTraceEnd,ECollisionChannel::ECC_WorldDynamic);
	const FVector impact = FVector(HitResult.ImpactPoint.X,HitResult.ImpactPoint.Y,HitResult.ImpactPoint.Z);
	const TConstArrayView<FVector> points = {Start,impact};
	TArray<FVector> test = {Start,impact};
	DrawCentripetalCatmullRomSpline(GetWorld(),{Start,impact},FColor::Blue,0.5,8,true,2,0,2);
	if(auto hitActor = Cast<ARecipeItem>(HitResult.HitObjectHandle.FetchActor()))
	{
		if(FVector::Dist(hitActor->GetActorLocation(), GetActorLocation()) < 300)
		{
			//si on disable pas la physic, il ne bougera pas
			hitActor->SphereCollision->SetSimulatePhysics(false);
			if(hitActor->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,FName("Grab")))
			{
				//hitActor->StaticMesh->AttachToComponent(hitActor->SphereCollision,FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				//hitActor->StaticMesh->AddRelativeLocation(FVector(0,0,-20));
				currentIngredient = hitActor;
			}
		}
	}
	
}

void AProjet_AICharacter::Drop()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Drop"));
	if(currentIngredient)
	{
		
		currentIngredient->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		currentIngredient->SphereCollision->SetSimulatePhysics(true);
		currentIngredient = nullptr;
	}
}
