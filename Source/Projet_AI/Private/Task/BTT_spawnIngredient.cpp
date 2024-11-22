// Fill out your copyright notice in the Description page of Project Settings.


#include "Task/BTT_spawnIngredient.h"

#include "Ingredient.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

EBTNodeResult::Type UBTT_spawnIngredient::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//return Super::ExecuteTask(OwnerComp, NodeMemory);

	SpawnLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(FName("SpawnLocation"));
	FRotator SpawnRotation = FRotator::ZeroRotator;

	const AIngredient* SpawnedObject = GetWorld()->SpawnActor<AIngredient>(SpawnedIngredient, SpawnLocation, SpawnRotation);

	if (SpawnedObject)
	{
		UStaticMeshComponent* ObjectMesh = SpawnedObject->FindComponentByClass<UStaticMeshComponent>();
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}