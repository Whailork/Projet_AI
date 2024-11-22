#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CookingHUD.generated.h"

struct FRecipeData;

UCLASS()
class COOKINGFACTORY_API ACookingHUD : public AHUD
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCookingWidgetHUD> CookingHUDClass;

	UPROPERTY()
	TObjectPtr<class UCookingWidgetHUD> CookingHUDPtr;

public:
	ACookingHUD();

protected:
	virtual void BeginPlay() override;

	void SetRecipe(const FRecipeData RecipeData);
	void RemoveRecipe(const FRecipeData RecipeData);
};
