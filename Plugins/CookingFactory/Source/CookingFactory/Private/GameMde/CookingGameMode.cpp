#include "GameMde/CookingGameMode.h"

#include "Data/RecipeTable.h"
#include "GameState/CookingGameState.h"


ACookingGameMode::ACookingGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	InitialNumOfRecipe = 10;
	TotalActiveRecipe = 3;
}

void ACookingGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ACookingGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (ACookingGameState* InCookingGameState = GetGameState<ACookingGameState>())
	{
		//Cacher our Custom Game State
		CookingGameState = InCookingGameState;
		GenerateRecipe();
	}
}

void ACookingGameMode::CompletedRecipe_Notify(const FGameplayTag RecipeTag) const
{
	//remove completed recipe
	CancelRecipe_Notify(RecipeTag);

	if (CookingGameState->ForceActiveRecipe())
	{
		//sucessfull activation recipe
		return;
	}
	//Mean we have no recipe and need to generate new one for activation behaviour
	if (URecipeFactorySubsystem* RecipeFactory = GetWorld()->GetSubsystem<URecipeFactorySubsystem>())
	{
		CancelRecipe_Notify(RecipeTag);
		const TArray<FRecipeData> RecipeData = RecipeFactory->GenerateRecipes(10);
		CookingGameState->SetRecipes(RecipeData);
		CookingGameState->ForceActiveRecipe();
	}
}

void ACookingGameMode::CancelRecipe_Notify(const FGameplayTag RecipeTag) const
{
	CookingGameState->ForceDisableRecipe(RecipeTag);
}

void ACookingGameMode::GenerateRecipe() const
{
	if (URecipeFactorySubsystem* RecipeFactory = GetWorld()->GetSubsystem<URecipeFactorySubsystem>())
	{
		RecipeFactory->Init(IngredientsContainer.LoadSynchronous(), RecipesContainer.LoadSynchronous());

		const TArray<FRecipeData> RecipeData = RecipeFactory->GenerateRecipes(GetNumOfRecipe());
		CookingGameState->SetRecipes(RecipeData);
		for (int i = 0; i < GetTotalActiveRecipe(); i++) CookingGameState->ForceActiveRecipe();
	}
}
