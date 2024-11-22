#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Subsystem/RecipeFactorySubsystem.h"
#include "CookingGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCompletedRecipe, const FRecipeData, RecipeData);

UCLASS()
class COOKINGFACTORY_API ACookingGameMode : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Recipe|DataTable")
	TSoftObjectPtr<UDataTable> IngredientsContainer;

	UPROPERTY(EditAnywhere, Category = "Recipe|DataTable")
	TSoftObjectPtr<UDataTable> RecipesContainer;
	
	UPROPERTY(EditAnywhere, Category = "Recipe|Value")
	int32 InitialNumOfRecipe;

	UPROPERTY(EditAnywhere, Category = "Recipe|Value")
	int32 TotalActiveRecipe;


public:
	UPROPERTY(BlueprintAssignable)
	FOnCompletedRecipe OnCompletedRecipe_Event;

public:
	ACookingGameMode();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetNumOfRecipe() const { return InitialNumOfRecipe; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetTotalActiveRecipe() const { return TotalActiveRecipe; }

	/**
	 * This should be call when the player completed a recipe or when one isn't valid anymore to generate a new one
	 */
	UFUNCTION(BlueprintCallable)
	void CompletedRecipe_Notify(const FGameplayTag RecipeTag) const;

	UFUNCTION(BlueprintCallable)
	void CancelRecipe_Notify(const FGameplayTag RecipeTag) const;

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	void GenerateRecipe() const;

private:
	UPROPERTY()
	TObjectPtr<class ACookingGameState> CookingGameState;
};
