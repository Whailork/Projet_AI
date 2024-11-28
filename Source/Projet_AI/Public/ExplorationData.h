// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/RecipeItem.h"
#include "ExplorationData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class PROJET_AI_API UExplorationData : public UObject
{
	GENERATED_BODY()
public:

	UExplorationData();
	
	TMap<EIngredientType,TArray<ARecipeItem*>> itemsMap;

	UFUNCTION(BlueprintCallable)
	ARecipeItem* checkForItem(EIngredientType itemType);
	UFUNCTION(BlueprintCallable)
	bool RemoveItemFromList(ARecipeItem* item);
};
