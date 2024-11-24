#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/WorldSubsystem.h"
#include "RecipeFactorySubsystem.generated.h"


enum class EIngredientType;

USTRUCT(BlueprintType)
struct FIngredientData
{
	GENERATED_BODY()
	FIngredientData(): Type()
	{
	}

	FIngredientData(FGameplayTag InName, TSoftObjectPtr<UStaticMesh> InMesh, TSoftObjectPtr<UTexture2D> InIcon,
	                EIngredientType InType) : Name(InName), Mesh(InMesh), Icon(InIcon), Type(InType)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EIngredientType Type;
};

USTRUCT(BlueprintType)
struct FRecipeData
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag RecipeName;

	UPROPERTY(BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(BlueprintReadOnly)
	TArray<FIngredientData> IngredientsList;
};


UCLASS()
class COOKINGFACTORY_API URecipeFactorySubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

	UPROPERTY()
	TSoftObjectPtr<UDataTable> IngredientsContainer;

	UPROPERTY()
	TSoftObjectPtr<UDataTable> RecipesContainer;

public:
	UFUNCTION(BlueprintCallable)
	void Init(const UDataTable* InIngredientsTable, const UDataTable* InRecipesTable);

	UFUNCTION(BlueprintCallable)
	TArray<FRecipeData> GenerateRecipes(const int32 InNumOfRecipe);

	UFUNCTION(BlueprintCallable)
	FRecipeData AddRecipe() const;

	// Spawns an ingredient item matching the RecipeTag (returns nullptr if none is found)
	UFUNCTION(BlueprintCallable)
	class ARecipeItem* CreateIngredientItem(const FGameplayTag RecipeTag,const FVector InLocation,const FRotator InRotation = FRotator::ZeroRotator) ;

private:
	FRecipeData CreateRecipe() const;
};
