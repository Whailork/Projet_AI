// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControllerBase.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AICharacter.h"

AAIControllerBase::AAIControllerBase(const FObjectInitializer& ObjectInitializer)
{
    // Create BlackBoard
    BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));

    // Create BehaviorTree Component
    BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree Component"));
}

void AAIControllerBase::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    // Cast your Character/Pawn to get access to the attributes
    if (auto AICharactere = Cast<AAICharacter>(InPawn))
    {
        // Check if the assets has been selected in the editor
        if (AICharactere->TreeAsset && AICharactere->TreeAsset->BlackboardAsset) {

            // Initialize the blackboard with the blackboard asset
            BlackboardComponent->InitializeBlackboard(*AICharactere->TreeAsset->BlackboardAsset);

            // Run the behavior tree
            BehaviorTreeComponent->StartTree(*AICharactere->TreeAsset);
        }
    }
}