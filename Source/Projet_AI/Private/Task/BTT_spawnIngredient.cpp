// Fill out your copyright notice in the Description page of Project Settings.


#include "Task/BTT_spawnIngredient.h"
#include "GameplayTagContainer.h"
#include"CookingFactory/Public/Subsystem/RecipeFactorySubsystem.h"
#include "SpawnerManagerController.h"
#include "Ingredient.h"
#include "SpawnerManagerController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

EBTNodeResult::Type UBTT_spawnIngredient::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	/* Algorithme permettant de décider quelle ingrédient
	
	FGameplayTag tag;
	FVector location = OwnerComp.GetBlackboardComponent()->GetValueAsVector(FName("SpawnLocation"));
	FRotator rotation = FRotator::ZeroRotator;

	if (recipeFactorySubsystem->CreateIngredientItem(tag, location, rotation))
	{
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
	*/

	return EBTNodeResult::Succeeded;
}