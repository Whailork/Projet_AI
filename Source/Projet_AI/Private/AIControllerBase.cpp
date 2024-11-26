// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControllerBase.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AICharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

void AAIControllerBase::BeginPlay()
{
    Super::BeginPlay();
}


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
    SightConfig->PeripheralVisionAngleDegrees = 50.0f;
    SightConfig->SightRadius = 5000.0f;
    SightConfig->SetMaxAge(5.0f);
    SightConfig->AutoSuccessRangeFromLastSeenLocation = 2.0f;
    SightConfig->LoseSightRadius = 2500.0f;
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

    PerceptionComponent->ConfigureSense(*SightConfig);
    PerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());
    
}

void AAIControllerBase::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    
    if (auto AICharactere = Cast<AAICharacter>(InPawn))
    {

        
        if (PerceptionComponent)
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("Linking perception events"));
            PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAIControllerBase::OnTargetPerceptionUpdated);
            PerceptionComponent->OnTargetPerceptionForgotten.AddDynamic(this, &AAIControllerBase::OnTargetPerceptionForgotten);
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("PerceptionComponent is null"));
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
    
    GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("IntoOnTargetPerceptionUpdated"));
    if (Actor)
    {
        FString ActorClass = Actor->GetClass()->GetName(); 
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Orange, FString::Printf(TEXT("Detected Actor Class: %s"), *ActorClass));
        
        if (Actor->IsA(AIngredient::StaticClass())) 
        {
            if (BlackboardComponent)
            {
                // Def l'acteur comme TargetIngredient
                BlackboardComponent->SetValueAsObject(TEXT("TargetIngredient"), Actor);
                GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Green, FString::Printf(TEXT("TargetIngredient set to: %s"), *Actor->GetName()));

                // Def la position de l'acteur comme TargetLocation 
                FVector TargetLocation = Actor->GetActorLocation();
                BlackboardComponent->SetValueAsVector(TEXT("IngredientLocation"), TargetLocation);
                GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Blue, FString::Printf(TEXT("IngredientLocation set to: %s"), *TargetLocation.ToString()));
            }
        }
    }
}


void AAIControllerBase::OnTargetPerceptionForgotten(AActor* Actor)
{/*
    if (Actor)
    {
        
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("Target Forgotten: %s"), *Actor->GetName()));
        
        if (BlackboardComponent)
        {
            BlackboardComponent->SetValueAsObject(TEXT("TargetIngredient"), nullptr);
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("TargetIngredient cleared"));
            
            BlackboardComponent->SetValueAsVector(TEXT("TargetLocation"), FVector::ZeroVector);
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("TargetLocation cleared"));
        }
    }
    */
}

