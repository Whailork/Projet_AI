#include "Actor/RecipeItem.h"

#include "Components/SphereComponent.h"

ARecipeItem::ARecipeItem()
{
	PrimaryActorTick.bCanEverTick = true;


	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = SphereCollision;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(SphereCollision);
}

void ARecipeItem::BeginPlay()
{
	Super::BeginPlay();
	SetMesh(IngredientMesh);
}
