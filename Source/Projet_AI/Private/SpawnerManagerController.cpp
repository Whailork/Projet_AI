// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerManagerController.h"

#include "SpawnManagerPawn.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

ASpawnerManagerController::ASpawnerManagerController(const FObjectInitializer& ObjectInitializer)
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree Component Manager"));
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
			BehaviorTreeComponent->StartTree(*SpawnManager->TreeAsset);
		}
	}

	//BehaviorTreeComponent->GetBlackboardComponent()
}