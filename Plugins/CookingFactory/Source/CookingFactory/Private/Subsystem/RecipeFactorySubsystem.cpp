// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/RecipeFactorySubsystem.h"

#include "../../../../../../Source/Projet_AI/Public/Ingredient.h"
#include "Actor/RecipeItem.h"
#include "Data/IngredientTable.h"
#include "Data/RecipeTable.h"

void URecipeFactorySubsystem::Init(const UDataTable* InIngredientsTable, const UDataTable* InRecipesTable)
{
	if (!InIngredientsTable->IsValidLowLevel() || !InRecipesTable->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Warning, TEXT("Initializing RecipeFactorySubsystem"));
	}
	IngredientsContainer = InIngredientsTable;
	RecipesContainer = InRecipesTable;
}

TArray<FRecipeData> URecipeFactorySubsystem::GenerateRecipes(const int32 InNumOfRecipe)
{
	TArray<FRecipeData> RecipeInfos = TArray<FRecipeData>();
	int Index = 0;
	while (Index < InNumOfRecipe)
	{
		RecipeInfos.Add(AddRecipe());
		Index++;
	}
	return RecipeInfos;
}


FRecipeData URecipeFactorySubsystem::AddRecipe() const
{
	const FRecipeData NewRecipe = CreateRecipe();
	return NewRecipe;
}

ARecipeItem* URecipeFactorySubsystem::CreateIngredientItem(const FGameplayTag RecipeTag, const FVector InLocation,
                                                           const FRotator InRotation) 
{
	TArray<FIngredientTable*> OutIngredientRows;
	//if (IngredientsContainer->GetRowMap().Num() > 0){}
	IngredientsContainer.LoadSynchronous()->GetAllRows<FIngredientTable>("", OutIngredientRows);

	for (const FIngredientTable* Ingredient : OutIngredientRows)
	{
		if (Ingredient->Name == RecipeTag)
		{
			ARecipeItem* RecipeItem = GetWorld()->SpawnActor<ARecipeItem>(InLocation, InRotation);
			RecipeItem->SetMesh(Ingredient->Mesh.LoadSynchronous());
			RecipeItem->IngredientTag = Ingredient->Name;
			RecipeItem->IngredientType = Ingredient->Type;
			return RecipeItem;
		}
	}
	return nullptr;
}

FRecipeData URecipeFactorySubsystem::CreateRecipe() const
{
	TArray<FIngredientTable*> OutIngredientRows;
	IngredientsContainer.LoadSynchronous()->GetAllRows<FIngredientTable>("", OutIngredientRows);

	TArray<FRecipeTable*> OutRecipeRows;
	RecipesContainer.LoadSynchronous()->GetAllRows<FRecipeTable>("", OutRecipeRows);

	const int32 RecipeIndex = FMath::RandRange(0, OutRecipeRows.Num() - 1);
	if (OutRecipeRows.IsValidIndex(RecipeIndex))
	{
		const FRecipeTable* RandomRecipeRow = OutRecipeRows[RecipeIndex];
		const int32 NumOfItem = FMath::RandRange(RandomRecipeRow->MinIngredient, RandomRecipeRow->MaxIngredient);

		FRecipeData NewRecipe = FRecipeData();
		NewRecipe.RecipeName = RandomRecipeRow->Name;
		NewRecipe.Icon = RandomRecipeRow->Icon;

		for (int i = 0; i < NumOfItem; i++)
		{
			const int32 IngredientChosen = FMath::RandRange(0, RandomRecipeRow->Ingredients.Num() - 1);

			FGameplayTag IngredientTag = RandomRecipeRow->Ingredients[IngredientChosen];
			auto Predicate = [&IngredientTag](const FIngredientTable* IngredientData)
			{
				return IngredientData && IngredientData->Name == IngredientTag;
			};
			FIngredientTable** Ingredient = OutIngredientRows.FindByPredicate(Predicate);

			const FIngredientData* NewIngredient = new FIngredientData((*Ingredient)->Name, (*Ingredient)->Mesh,
			                                                           (*Ingredient)->Icon, (*Ingredient)->Type);

			NewRecipe.IngredientsList.Add(*NewIngredient);
		}
		return NewRecipe;
	}


	return FRecipeData();
}
