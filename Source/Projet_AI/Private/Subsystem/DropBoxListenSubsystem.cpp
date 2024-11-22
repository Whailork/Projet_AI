#include "Subsystem/DropBoxListenSubsystem.h"
#include "EngineUtils.h"
#include "GameState/CookingGameState.h"

void UDropBoxListenSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	for (TActorIterator<ADropBoxActor> It(&InWorld); It; ++It)
	{
		ADropBoxActor* SpawnPointActor = *It;
		ActiveDropBox.Add(SpawnPointActor);
	}

	if (ActiveDropBox.IsEmpty())return;

	if (ACookingGameState* InCookingGameState = InWorld.GetGameState<ACookingGameState>())
	{
		InCookingGameState->OnEnableRecipe_Event.AddUniqueDynamic(this, &ThisClass::SetNewRecipe);

		//feeding recipe to dropbox so ui can easily access it
		for (int i = 0; i < InCookingGameState->GetActiveRecipes().Num(); i++)
		{
			FRecipeData Recipe = InCookingGameState->GetActiveRecipes()[i];
			if (ActiveDropBox.IsValidIndex(i))
			{
				ActiveDropBox[i]->SetRecipe(Recipe);
			}
		}
	}
}

void UDropBoxListenSubsystem::SubscribeDropBox(ADropBoxActor* DropBox)
{
	if (DropBox)
	{
		if (!ActiveDropBox.Contains(DropBox))
		{
			ActiveDropBox.Add(DropBox);
		}
	}
}

void UDropBoxListenSubsystem::UnSubscribeDropBox(ADropBoxActor* DropBox)
{
	if (DropBox)
	{
		if (ActiveDropBox.Contains(DropBox))
		{
			ActiveDropBox.Remove(DropBox);
		}
	}
}

void UDropBoxListenSubsystem::SetNewRecipe(const FRecipeData RecipeData)
{
	for (ADropBoxActor* DropBox : ActiveDropBox)
	{
		if (!DropBox->HasRecipe())
		{
			DropBox->SetRecipe(RecipeData);
			//we only set one recipe at the time
			break;
		}
	}
}
