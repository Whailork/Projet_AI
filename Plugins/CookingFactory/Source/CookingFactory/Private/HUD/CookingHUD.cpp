#include "HUD/CookingHUD.h"

#include "Blueprint/UserWidget.h"
#include "GameMde/CookingGameMode.h"
#include "GameState/CookingGameState.h"
#include "UI/CookingWidgetHUD.h"


ACookingHUD::ACookingHUD()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACookingHUD::BeginPlay()
{
	Super::BeginPlay();

	if (CookingHUDClass)
	{
		CookingHUDPtr = CreateWidget<UCookingWidgetHUD>(GetOwningPlayerController(), CookingHUDClass.Get(),TEXT("CookingHUD"));

		if (ACookingGameState* GS = Cast<ACookingGameState>(GetOwningPlayerController()->GetWorld()->GetGameState()))
		{
			GS->OnEnableRecipe_Event.AddUniqueDynamic(this, &ThisClass::SetRecipe);
			if (!GS->GetActiveRecipes().IsEmpty())
			{
				for (const auto ActiveRecipe : GS->GetActiveRecipes())
				{
					SetRecipe(ActiveRecipe);
				}
			}
		}

		if (ACookingGameMode* GM = Cast<ACookingGameMode>(GetOwningPlayerController()->GetWorld()->GetAuthGameMode()))
		{
			GM->OnCompletedRecipe_Event.AddUniqueDynamic(this, &ThisClass::RemoveRecipe);
		}
		CookingHUDPtr->AddToViewport();
	}
}

void ACookingHUD::SetRecipe(const FRecipeData RecipeData)
{
	if (CookingHUDPtr)
	{
		CookingHUDPtr->SetRecipe(RecipeData);
	}
}

void ACookingHUD::RemoveRecipe(const FRecipeData RecipeData)
{
	if (CookingHUDPtr)
	{
		CookingHUDPtr->RemoveRecipe(RecipeData);
	}
}
