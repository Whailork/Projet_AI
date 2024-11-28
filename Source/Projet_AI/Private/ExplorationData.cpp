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

ARecipeItem* UExplorationData::checkForItem(EIngredientType itemType)
{
	/*EIngredientType enumType;
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
	}*/
	
	if(itemsMap[itemType].IsEmpty())
	{
		return nullptr;
	}
	ARecipeItem* itemToReturn = itemsMap[itemType][0];
	RemoveItemFromList(itemToReturn);
	return itemToReturn;
}

bool UExplorationData::RemoveItemFromList(ARecipeItem* item)
{
	if(!itemsMap[item->IngredientType].IsEmpty())
	{
		if(itemsMap[item->IngredientType].Find(item))
		{
			itemsMap[item->IngredientType].Remove(item);
			return true;
		}
	}
	
	return false;
	
}


