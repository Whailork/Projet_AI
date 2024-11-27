// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplorationData.h"

#include "Data/IngredientTable.h"

UExplorationData::UExplorationData()
{
	itemsMap.Add(EIngredientType::Cereal);
	itemsMap.Add(EIngredientType::Dairy);
	itemsMap.Add(EIngredientType::Meet);
	itemsMap.Add(EIngredientType::Fruit_Vegetable);
}

ARecipeItem* UExplorationData::checkForItem(FString itemType)
{
	EIngredientType enumType;
	if(itemType == "Cereal")
	{
		enumType = EIngredientType::Cereal;
	}
	else
	{
		if(itemType == "Meet")
		{
			enumType = EIngredientType::Meet;
		}
		else
		{
			if(itemType == "Dairy")
			{
				enumType = EIngredientType::Dairy;
			}
			else
			{
				if(itemType == "Fruit_Vegetable")
				{
					enumType = EIngredientType::Fruit_Vegetable;
				}
				else
				{
					return nullptr;
				}
			}
		}
	}
	
	if(itemsMap[enumType].IsEmpty())
	{
		return nullptr;
	}
	ARecipeItem* itemToReturn = itemsMap[enumType][0];
	itemsMap[enumType].RemoveAt(0);
	return itemToReturn;
}
