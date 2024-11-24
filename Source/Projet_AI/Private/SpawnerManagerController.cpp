// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerManagerController.h"

#include "SpawnManagerPawn.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

ASpawnerManagerController::ASpawnerManagerController(const FObjectInitializer& ObjectInitializer)
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree Component Manager"));
	//BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Component"));
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

		/*
		if (SpawnManager->TreeAsset->BlackboardAsset)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("SpawnManager->TreeAsset->BlackboardAsset"));
			BlackboardComponent = BehaviorTreeComponent->GetBlackboardComponent();
			//UseBlackboard(SpawnManager->TreeAsset->BlackboardAsset, BlackboardComponent);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("!(SpawnManager->TreeAsset->BlackboardAsset)"));
		}
		*/
	}

	//BehaviorTreeComponent->GetBlackboardComponent()
}

bool ASpawnerManagerController::Spawn()
{
	/*
	if (!BlackboardComponent)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("BlackboardComponent is not initialized"));
		return false;
	}
*/
	// Vérifier ici////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (!BehaviorTreeComponent->GetBlackboardComponent())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("BehaviorTreeComponent->BlackboardComponent is not initialized"));
		return false;
	}

	SpawnLocation = BehaviorTreeComponent->GetBlackboardComponent()->GetValueAsVector(FName("SpawnLocation"));
	FRotator SpawnRotation = FRotator::ZeroRotator;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("X : ") + FString::SanitizeFloat(SpawnLocation.X) + TEXT("Y : ") + FString::SanitizeFloat(SpawnLocation.Y) + TEXT("Z : ") + FString::SanitizeFloat(SpawnLocation.Z));
	
	/*
	
	//SpawnLocation = GetBlackboardComponent()->GetValueAsVector(FName("SpawnLocation"));
	
	
	*/

	/*
	// TODO :	Use CreateIngredientItem(const FGameplayTag RecipeTag, const FVector InLocation, const FRotator InRotation)
	//			#include "CookingFactory/Public/Subsystem/RecipeFactorySubsystem.h"
	AActor* SpawnedObject = GetWorld()->SpawnActor<AActor>(SpawnedIngredient, SpawnLocation, SpawnRotation);

	if (SpawnedObject)
	{
		UStaticMeshComponent* ObjectMesh = SpawnedObject->FindComponentByClass<UStaticMeshComponent>();
		return true;
	}
	return false;
	*/


	return true;
}