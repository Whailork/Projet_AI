#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/HorizontalBox.h"
#include"GameplayTagContainer.h"
#include "CookingWidgetHUD.generated.h"

struct FRecipeData;

USTRUCT()
struct FWidgetRecipe
{
	GENERATED_BODY()
	
	UPROPERTY()
	FGameplayTag ID;
	UPROPERTY()
	class URecipeWidgetItem* Widget;
	
	FWidgetRecipe(URecipeWidgetItem* InWidget, const FGameplayTag ID): ID(ID), Widget(InWidget)
	{
	}
	FWidgetRecipe(): ID(), Widget(nullptr)
	{
	}


};

UCLASS()
class COOKINGFACTORY_API UCookingWidgetHUD : public UUserWidget
{
	GENERATED_BODY()



	UPROPERTY()
	TArray<FWidgetRecipe> CreatedRecipeWidget;

protected:
	UPROPERTY(meta=(BindWidget))
	UHorizontalBox* RecipeContainer;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class URecipeWidgetItem> RecipeItemClass;
public:
	void SetRecipe(const FRecipeData RecipeData);
	void RemoveRecipe(const FRecipeData RecipeData);
};
