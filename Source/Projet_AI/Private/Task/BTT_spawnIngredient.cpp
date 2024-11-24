// Fill out your copyright notice in the Description page of Project Settings.


#include "Task/BTT_spawnIngredient.h"
#include"CookingFactory/Public/Subsystem/RecipeFactorySubsystem.h"
#include "SpawnerManagerController.h"
#include "Ingredient.h"
#include "SpawnerManagerController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

EBTNodeResult::Type UBTT_spawnIngredient::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	/*
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("In spawnIngredient task"));
	
	ASpawnerManagerController* SpawnManager = nullptr;
	TArray<AActor*> FoundActors;
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnerManagerController::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		SpawnManager = Cast<ASpawnerManagerController>(Actor);
	}
	
	if (SpawnManager && SpawnManager->Spawn())
	{
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
	*/

	

	// Algorithme permettant de décider quelle ingrédient
	FGameplayTag tag = FGameplayTag::Empty;
	FVector location = OwnerComp.GetBlackboardComponent()->GetValueAsVector(FName("SpawnLocation"));
	FRotator rotation = FRotator::ZeroRotator;
	
	recipeFactorySubsystem->CreateIngredientItem(tag, location, rotation);

	//ARecipeItem* URecipeFactorySubsystem::CreateIngredientItem(const FGameplayTag RecipeTag, const FVector InLocation, const FRotator InRotation);
	
  
}

/*
// Get the navigation Area in the current level
	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

if (NavArea)
{
	// Get the current player position
	FVector PlayerPosition = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	// Return true if found a random point in the navigation and around your player position
	if (NavArea->K2_GetRandomPointInNavigableRadius(GetWorld(), PlayerPosition, TargetLocation, 100.0f))
	{
		// Set value in the blackboard
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName("TargetLocation"), TargetLocation);

		return EBTNodeResult::Succeeded;
	}
}
return EBTNodeResult::Failed;
 */