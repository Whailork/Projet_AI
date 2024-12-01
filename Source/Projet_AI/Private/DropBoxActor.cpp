#include "DropBoxActor.h"

#include "Actor/RecipeItem.h"
#include "Components/BoxComponent.h"
#include "GameMde/CookingGameMode.h"

ADropBoxActor::ADropBoxActor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent->Mobility = EComponentMobility::Type::Movable;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->Mobility = EComponentMobility::Type::Movable;
	StaticMesh->SetupAttachment(RootComponent);
	
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);
	BoxCollision->Mobility = EComponentMobility::Type::Movable;
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
	RequiredTags.Empty();
	for (int i = 0; i < CurrentIngredients.Num(); i++)
	{
		CurrentIngredients[i]->Destroy();
	}
	CurrentIngredients.Empty();
	CurrentTags.Empty();
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
		CurrentTags.Add(Ingredient->IngredientTag);
		//on check si la recette est complète
		bool recipeIncomplete = false;
		if (CurrentTags.Num() == RequiredTags.Num())
		{
			for (auto tag : RequiredTags)
			{
				if (!CurrentTags.Contains(tag))
				{
					recipeIncomplete = true;
					break;
				}
			}
			if (!recipeIncomplete)
			{
				ingredientscore = 0;
				for (auto* Item : CurrentIngredients)
				{
					ingredientscore += 3;
				}

				score += ingredientscore;
				FString ScoreMessage = FString::Printf(TEXT("Score total ajouté : %d"), ingredientscore);
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, ScoreMessage);
				
				CompleteRecipe();
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("recipeComplete"));
			}
		}
	}
}

void ADropBoxActor::OnEndOverlap(AActor* ThisActor, AActor* OtherActor)
{
	if (const auto Ingredient = Cast<ARecipeItem>(OtherActor))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("ingredient removed"));
		CurrentIngredients.Remove(Ingredient);
		CurrentTags.RemoveSingle(Ingredient->IngredientTag);
		//on check si la recette est complète
		bool recipeIncomplete = false;
		if (CurrentTags.Num() == RequiredTags.Num())
		{
			for (auto tag : RequiredTags)
			{
				if (!CurrentTags.Contains(tag))
				{
					recipeIncomplete = true;
					break;
				}
			}
			if (!recipeIncomplete)
			{
				CompleteRecipe();
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("recipeComplete"));
				//todo : on ajoute du score ici
			}
		}
	}
}

void ADropBoxActor::SetRecipe(const FRecipeData& InRecipeData)
{
	bHasRecipe = true;
	RecipeData = InRecipeData;
	//on load les ingredients dans la liste de tags requis  pour la comparaison
	for (auto ingredient : InRecipeData.IngredientsList)
	{
		RequiredTags.Add(ingredient.Name);
	}
	OnSetRecipe_BP(InRecipeData);
}
