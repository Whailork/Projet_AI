// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"

#include "AIControllerBase.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ExplorationData.h"
#include "InputActionValue.h"
#include "Actor/RecipeItem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AAICharacter::AAICharacter()
{
	// TODO : Enlever le Tick?
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

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
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AAICharacter::Move(const FInputActionValue& Value)
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

void AAICharacter::Look(const FInputActionValue& Value)
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

// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AAICharacter::GetTriggerGrab()
{
	return triggerGrab;
}

bool AAICharacter::GetTriggerShrug()
{
	return triggerShrug;
}

bool AAICharacter::GetTriggerNotify()
{
	return  triggerNotify;
}

void AAICharacter::SetTriggerGrab(bool value)
{
	triggerGrab = value;
}

void AAICharacter::SetTriggerShrug(bool value)
{
	triggerShrug = value;
}

void AAICharacter::SetTriggerNotify(bool value)
{
	triggerNotify = value;
}


bool AAICharacter::Grab(ARecipeItem* targetIngredient)
{
		
	if (FVector::Dist(targetIngredient->GetActorLocation(), GetActorLocation()) < 300)
	{
		triggerGrab = true;
		return true;
	}
	return false;
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Grab"));
}

void AAICharacter::attatchIngredient(ARecipeItem* targetIngredient)
{
	if(FVector::Dist(targetIngredient->GetActorLocation(), GetActorLocation()) < 300)
	{
		//si on disable pas la physic, il ne bougera pas
		targetIngredient->SphereCollision->SetSimulatePhysics(false);
		if(targetIngredient->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,FName("Grab")))
		{
			currentIngredient = targetIngredient;
			AAIControllerBase* controller = Cast<AAIControllerBase>(Controller);
			controller->BlackboardComponent->SetValueAsObject(TEXT("GrabbedIngredient"),targetIngredient);
		}
	}
	
}

void AAICharacter::Drop()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Ai Drop"));
	
	if(currentIngredient)
	{
		AAIControllerBase* controller = Cast<AAIControllerBase>(Controller);
		UExplorationData* data = Cast<UExplorationData>(controller->BlackboardComponent->GetValueAsObject("ExplorationData"));
		if(data)
		{
			data->RemoveItemFromList(currentIngredient);
		}
		
		currentIngredient->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		currentIngredient->SphereCollision->SetSimulatePhysics(true);
		currentIngredient = nullptr;
	}
}

EIngredientType AAICharacter::getFoodType()
{
	return FoodType;
	
}
