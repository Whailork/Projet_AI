// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControllerBase.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AICharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AAIControllerBase::AAIControllerBase(const FObjectInitializer& ObjectInitializer)
{
    // Create BlackBoard
    BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));

    // Create BehaviorTree Component
    BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree Component"));

    //initialise perception
    PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));

    SetPerceptionComponent(*PerceptionComponent);

    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

    //settings par defaut pour la sight
    SightConfig->PeripheralVisionAngleDegrees = 45.0f;
    SightConfig->SightRadius = 10.0f;
    SightConfig->SetMaxAge(6.0f);
    SightConfig->AutoSuccessRangeFromLastSeenLocation = 2.0f;
    SightConfig->LoseSightRadius = 12.0f;
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

    PerceptionComponent->ConfigureSense(*SightConfig);
    PerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());
}

void AAIControllerBase::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    // Cast your Character/Pawn to get access to the attributes
    if (auto AICharactere = Cast<AAICharacter>(InPawn))
    {
        
        if (PerceptionComponent)
        {
            PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAIControllerBase::OnTargetPerceptionUpdated);
            PerceptionComponent->OnTargetPerceptionForgotten.AddDynamic(this, &AAIControllerBase::OnTargetPerceptionForgotten);
        }

        // Check if the assets has been selected in the editor
        if (AICharactere->TreeAsset && AICharactere->TreeAsset->BlackboardAsset) {

            // Initialize the blackboard with the blackboard asset
            BlackboardComponent->InitializeBlackboard(*AICharactere->TreeAsset->BlackboardAsset);

            // Run the behavior tree
            BehaviorTreeComponent->StartTree(*AICharactere->TreeAsset);
        }
    }
}

void AAIControllerBase::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("perception Updated"));	
}

void AAIControllerBase::OnTargetPerceptionForgotten(AActor* Actor)
{
}
