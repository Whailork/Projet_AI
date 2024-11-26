// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControllerBase.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AICharacter.h"
#include "Actor/RecipeItem.h"
#include "GameState/CookingGameState.h"
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

        possessedAi = AICharactere;
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
    
    if (Actor)
    {
        //FString ActorClass = Actor->GetClass()->GetName(); 
        //GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Orange, FString::Printf(TEXT("Detected Actor Class: %s"), *ActorClass));
        
         //GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("IntoOnTargetPerceptionUpdated"));
   if (auto ingredient = Cast<ARecipeItem>(Actor)) 
        {
            if (BlackboardComponent)
            {
                //si l'ingredient est utile
                if(auto gameState = Cast<ACookingGameState>(GetWorld()->GetGameState()))
                {
                    if(gameState->isIngredientInActiveRecipe(ingredient->IngredientTag))
                    {
                        //si l'Ai n'a rien dans ses mains
                        if(possessedAi->currentIngredient == nullptr)
                        {
                            //si l'Ai peu le pickup
                            if(possessedAi->FoodType == ingredient->IngredientType)
                            {
                                //pickup ingredient
                                // Def l'acteur comme TargetIngredient
                                BlackboardComponent->SetValueAsObject(TEXT("TargetIngredient"), Actor);
                                GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Green, FString::Printf(TEXT("TargetIngredient set to: %s"), *Actor->GetName()));

                                // Def la position de l'acteur comme TargetLocation 
                                FVector TargetLocation = Actor->GetActorLocation();
                                BlackboardComponent->SetValueAsVector(TEXT("IngredientLocation"), TargetLocation);
                                GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Blue, FString::Printf(TEXT("IngredientLocation set to: %s"), *TargetLocation.ToString()));
                                //possessedAi->Grab(ingredient);
                            }
                            else
                            {
                                //l'Ai averti les autres
                                possessedAi->triggerNotify = true;
                            }
                        }
                        else
                        {
                            //on fait apparaitre un point d'exclamation au dessus de sa tête pour dire qu'il l'a vu
                        }
                        
                    }
                    else
                    {
                        //l'Ai shrug et continue l'exploration
                        possessedAi->triggerShrug = true;
                    }
                }
                
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

