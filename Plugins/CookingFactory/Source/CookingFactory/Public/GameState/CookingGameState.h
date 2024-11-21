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
	bool ForceActiveRecipe();
	bool ForceDisableRecipe(const FGameplayTag RecipeTag);

	void SetRecipes(const TArray<FRecipeData>& InActiveRecipe);
	bool GetActiveRecipe(const FGameplayTag RecipeTag, FRecipeData& OutRecipe) const;
	
	UFUNCTION(BlueprintCallable)
	TArray<FRecipeData> GetActiveRecipes() const { return ActiveRecipe; }

private:
	UPROPERTY()
	TArray<FRecipeData> ActiveRecipe;
	UPROPERTY()
	TArray<FRecipeData> InactiveRecipe;
};
