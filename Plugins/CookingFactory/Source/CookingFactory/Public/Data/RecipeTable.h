#pragma once

#include "CoreMinimal.h"
#include"GameplayTagContainer.h"
#include "RecipeTable.generated.h"


USTRUCT(BlueprintType)
struct FRecipeTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FGameplayTag> Ingredients;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MinIngredient;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxIngredient;
	
};
