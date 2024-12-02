// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerManagerController.h"

#include "SpawnManagerPawn.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameState/CookingGameState.h"
#include "GameplayTagContainer.h"
#include "Subsystem/RecipeFactorySubsystem.h"

ASpawnerManagerController::ASpawnerManagerController(const FObjectInitializer& ObjectInitializer)
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree Component Manager"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Component"));
}

void ASpawnerManagerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Cast your Character/Pawn to get access to the attributes
	if (auto SpawnManager = Cast<ASpawnManagerPawn>(InPawn))
	{
		// Check if the assets has been selected in the editor
		if (SpawnManager->TreeAsset) {
			
			// Run the behavior tree
			BlackboardComponent->InitializeBlackboard(*SpawnManager->TreeAsset->BlackboardAsset);
			BehaviorTreeComponent->StartTree(*SpawnManager->TreeAsset);

		}
		
		
	}

	//on load les recipes déjà existantes s'il y en a
	ACookingGameState* gameState = Cast<ACookingGameState>(GetWorld()->GetGameState());
	if(gameState)
	{
		TArray<FRecipeData> recipes = gameState->GetActiveRecipes();
		for (FRecipeData Recipe : recipes)
		{
			for (FIngredientData Ingredient : Recipe.IngredientsList)
			{
				toSpawnList.Add(Ingredient.Name);
			}
		}
		// on s'abonne à la delegate pour quand on va avoir des nouvelles recettes
		gameState->OnEnableRecipe_Event.AddDynamic(this,&ASpawnerManagerController::OnNewRecipe);
	}
	
}

void ASpawnerManagerController::OnNewRecipe(FRecipeData data)
{
	for(int i = 0; i < data.IngredientsList.Num();i++)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("lol"));
		toSpawnList.Add(data.IngredientsList[i].Name);
	}
	
}

bool ASpawnerManagerController::GetNextIngredient(FGameplayTag& outTag)
{
	if(toSpawnList.IsEmpty())
	{
		return false;
	}
	FGameplayTag toReturnTag = toSpawnList[0];
	toSpawnList.RemoveAt(0);
	outTag = toReturnTag;
	return true;
}



