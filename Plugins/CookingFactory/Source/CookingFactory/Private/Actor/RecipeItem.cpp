﻿#include "Actor/RecipeItem.h"

#include "Components/SphereComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

void ARecipeItem::SetInDroppingZone(bool value)
{
	InDroppingZone = value;
}

bool ARecipeItem::IsInDroppingZone()
{
	return InDroppingZone;
}

ARecipeItem::ARecipeItem()
{
	PrimaryActorTick.bCanEverTick = true;


	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = SphereCollision;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(SphereCollision);

	StimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulis"));
	StimuliSource->RegisterForSense(UAISense_Sight::StaticClass());
	StimuliSource->RegisterWithPerceptionSystem();
	
	//on set les channels de colision
	SphereCollision->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
	SphereCollision->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);
	SphereCollision->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Block);
	SphereCollision->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECC_PhysicsBody,ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECC_Vehicle,ECR_Block);
	SphereCollision->SetCollisionResponseToChannel(ECC_Destructible,ECR_Block);
	SphereCollision->SetCollisionResponseToChannel(ECC_GameTraceChannel1,ECR_Block); // ingredient channel
	SphereCollision->SetCollisionResponseToChannel(ECC_GameTraceChannel2,ECR_Ignore); // ai character channel
	SphereCollision->SetCollisionResponseToChannel(ECC_GameTraceChannel3,ECR_Ignore); // dropbox channel

	StaticMesh->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
	StaticMesh->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	StaticMesh->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);
	StaticMesh->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Block);
	StaticMesh->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
	StaticMesh->SetCollisionResponseToChannel(ECC_PhysicsBody,ECR_Block);
	StaticMesh->SetCollisionResponseToChannel(ECC_Vehicle,ECR_Block);
	StaticMesh->SetCollisionResponseToChannel(ECC_Destructible,ECR_Block);
	StaticMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1,ECR_Ignore); // ingredient channel
	StaticMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel2,ECR_Ignore); // ai character channel
	StaticMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel3,ECR_Ignore); // dropbox channel

}

void ARecipeItem::BeginPlay()
{
	Super::BeginPlay();
	SetMesh(IngredientMesh);
	StaticMesh->SetRelativeScale3D(FVector(1.5,1.5,1.5));
	SphereCollision->SetSimulatePhysics(true);
	
}
