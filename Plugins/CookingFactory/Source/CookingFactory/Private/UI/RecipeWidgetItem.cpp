// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RecipeWidgetItem.h"

#include "Subsystem/RecipeFactorySubsystem.h"
#include "Blueprint/WidgetTree.h"

void URecipeWidgetItem::SetRecipe(const FRecipeData& RecipeData)
{
	//Setting Recipe Image
	FSlateBrush Brush = FSlateBrush();
	const UE::Slate::FDeprecateVector2DParameter RecipeParamSize(128.F, 128.F);
	Brush.SetResourceObject(RecipeData.Icon.LoadSynchronous());
	Brush.SetImageSize(RecipeParamSize);
	RecipeImage->SetBrush(Brush);
	IngredientContainer->ClearChildren();

	for (auto Ingredient : RecipeData.IngredientsList)
	{
		FSlateBrush IngredientBrush = FSlateBrush();
		const UE::Slate::FDeprecateVector2DParameter IngredientParamSize(32.F, 32.F);

		IngredientBrush.SetResourceObject(Ingredient.Icon.LoadSynchronous());
		IngredientBrush.SetImageSize(IngredientParamSize);

		UImage* IngredientImage = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
		IngredientImage->SetBrush(IngredientBrush);

		IngredientContainer->AddChild(IngredientImage);
	}
}
