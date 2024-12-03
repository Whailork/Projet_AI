#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Subsystem/RecipeFactorySubsystem.h"
#include "CookingGameState.generated.h"


//UI Should bind on this 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnableRecipe, const FRecipeData, RecipeData);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDisableRecipe, const FRecipeData, RecipeData);

UCLASS()
class COOKINGFACTORY_API ACookingGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnEnableRecipe OnEnableRecipe_Event;

	UPROPERTY(BlueprintAssignable)
	FOnDisableRecipe OnDisableRecipe_Event;

public:
	// il faut que je stocke l'exploration data quelque part sinon elle se fait erase par le garbage collector
	UPROPERTY()
	UObject* ExplorationData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite , meta = (AllowPrivateAccess = "true"))
	int Score = 0;
	UFUNCTION(BlueprintCallable)
	void UpdateScore(int plusValue);
	
	bool ForceActiveRecipe();
	bool ForceDisableRecipe(const FGameplayTag RecipeTag);

	void SetRecipes(const TArray<FRecipeData>& InActiveRecipe);
	bool GetActiveRecipe(const FGameplayTag RecipeTag, FRecipeData& OutRecipe) const;
	
	UFUNCTION(BlueprintCallable)
	TArray<FRecipeData> GetActiveRecipes() const { return ActiveRecipe; }

	UFUNCTION(BlueprintCallable)
	bool isIngredientInActiveRecipe(FGameplayTag ingredientTag);

private:
	UPROPERTY()
	TArray<FRecipeData> ActiveRecipe;
	UPROPERTY()
	TArray<FRecipeData> InactiveRecipe;
};
