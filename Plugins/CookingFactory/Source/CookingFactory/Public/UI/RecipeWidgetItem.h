#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/WrapBox.h"
#include "Components/Image.h"
#include "RecipeWidgetItem.generated.h"

struct FRecipeData;

UCLASS()
class COOKINGFACTORY_API URecipeWidgetItem : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta=(BindWidget))
	UWrapBox* IngredientContainer;

	UPROPERTY(meta=(BindWidget))
	UImage* RecipeImage;

public:
	void SetRecipe(const FRecipeData& RecipeData);
};
