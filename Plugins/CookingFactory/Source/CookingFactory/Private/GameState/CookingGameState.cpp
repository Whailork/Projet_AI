#include "GameState/CookingGameState.h"


void ACookingGameState::SetRecipes(const TArray<FRecipeData>& InActiveRecipe)
{
	InactiveRecipe = InActiveRecipe;
}

bool ACookingGameState::GetActiveRecipe(const FGameplayTag RecipeTag, FRecipeData& OutRecipe) const
{
	for (const FRecipeData& Recipe : ActiveRecipe)
	{
		if (Recipe.RecipeName == RecipeTag)
		{
			OutRecipe = Recipe;
			return true;
		}
	}
	return false;
}

bool ACookingGameState::ForceActiveRecipe()
{
	if (InactiveRecipe.IsEmpty())return false;
	
	const int32 RandIndex = FMath::RandRange(0, InactiveRecipe.Num() - 1);
	const FRecipeData NewRecipe = InactiveRecipe[RandIndex];
	
	InactiveRecipe.RemoveAt(RandIndex);
	
	if (OnEnableRecipe_Event.IsBound())
	{
		OnEnableRecipe_Event.Broadcast(NewRecipe);
	}
	ActiveRecipe.Add(NewRecipe);
	return true;
}

bool ACookingGameState::ForceDisableRecipe(const FGameplayTag RecipeTag)
{
	if (ActiveRecipe.IsEmpty()) return false;
	for (int32 i = 0; i < ActiveRecipe.Num(); ++i)
	{
		if (ActiveRecipe[i].RecipeName == RecipeTag)
		{
			InactiveRecipe.Add(ActiveRecipe[i]);
			ActiveRecipe.RemoveAt(i);
			OnDisableRecipe_Event.Broadcast(ActiveRecipe[i]);

			return true;
		}
	}

	return false;
}
