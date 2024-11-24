// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CookingWidgetHUD.h"

#include "Subsystem/RecipeFactorySubsystem.h"
#include "UI/RecipeWidgetItem.h"

void UCookingWidgetHUD::SetRecipe(const FRecipeData RecipeData)
{
	if (RecipeData.IngredientsList.IsEmpty())return;

	URecipeWidgetItem* RecipeItem = CreateWidget<URecipeWidgetItem>(GetWorld(), RecipeItemClass);
	RecipeItem->SetRecipe(RecipeData);
	RecipeContainer->AddChildToVerticalBox(RecipeItem);

	const FWidgetRecipe Item = FWidgetRecipe(RecipeItem, RecipeData.RecipeName);
	CreatedRecipeWidget.Add(Item);
}

void UCookingWidgetHUD::RemoveRecipe(const FRecipeData RecipeData)
{
	for (int i = 0; i < CreatedRecipeWidget.Num(); i++)
	{
		auto& Recipe = CreatedRecipeWidget[i];

		if (Recipe.ID == RecipeData.RecipeName)
		{
			const bool bRemoved = RecipeContainer->RemoveChild(Recipe.Widget);
			if (bRemoved)
			{
				RecipeContainer->Modify();
				CreatedRecipeWidget.RemoveAt(i);
				
				RecipeContainer->InvalidateLayoutAndVolatility();
			}
			break;

		}
	}
}
