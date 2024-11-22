#include "DropBoxActor.h"

#include "Actor/RecipeItem.h"
#include "Components/BoxComponent.h"
#include "GameMde/CookingGameMode.h"

ADropBoxActor::ADropBoxActor()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	RootComponent = BoxCollision;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(BoxCollision);
}

void ADropBoxActor::BeginPlay()
{
	Super::BeginPlay();

	this->OnActorBeginOverlap.AddDynamic(this, &ADropBoxActor::OnBeginOverlap);
	this->OnActorEndOverlap.AddDynamic(this, &ADropBoxActor::OnEndOverlap);
}

void ADropBoxActor::CompleteRecipe()
{
	bHasRecipe = false;
	CompleteRecipe_BP();
	if (const ACookingGameMode* GM = GetWorld()->GetAuthGameMode<ACookingGameMode>())
	{
		GM->CompletedRecipe_Notify(RecipeData.RecipeName);
	}
}


void ADropBoxActor::OnBeginOverlap(AActor* ThisActor, AActor* OtherActor)
{
	if (const auto Ingredient = Cast<ARecipeItem>(OtherActor))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("ingredient Added"));
		CurrentIngredients.Add(Ingredient);
	}
}

void ADropBoxActor::OnEndOverlap(AActor* ThisActor, AActor* OtherActor)
{
	if (const auto Ingredient = Cast<ARecipeItem>(OtherActor))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("ingredient removed"));
		CurrentIngredients.Remove(Ingredient);
	}
}

void ADropBoxActor::SetRecipe(const FRecipeData& InRecipeData)
{
	bHasRecipe = true;
	RecipeData = InRecipeData;

	OnSetRecipe_BP(InRecipeData);
}
