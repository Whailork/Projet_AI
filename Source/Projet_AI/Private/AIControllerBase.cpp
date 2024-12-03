// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControllerBase.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AICharacter.h"
#include "DropBoxActor.h"
#include "ExplorationData.h"
#include "NavigationSystem.h"
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
    SightConfig->SightRadius = 1000.0f;
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
            //on met la data d'exploration s'il y en a pas déjà  dans le blackboard
            if(!BlackboardComponent->GetValueAsObject(TEXT("ExplorationData")))
            {
                UExplorationData* data = NewObject<UExplorationData>();
                Cast<ACookingGameState>(GetWorld()->GetGameState())->ExplorationData = data;
                BlackboardComponent->SetValueAsObject(TEXT("ExplorationData"),data);
            }
            

            // Run the behavior tree
            BehaviorTreeComponent->StartTree(*AICharactere->TreeAsset);
        }
        //on ramasse la position d'une dropbox et on la set comme position de retour dans le blackboard
        /*for (auto Element : GetLevel()->Actors)
        {
            if(Element.IsA(ADropBoxActor::StaticClass()))
            {
                BlackboardComponent->SetValueAsVector(TEXT("CookingZoneLocation"),Element->GetActorLocation());
                break;
            }
        }*/
        
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
                        //si l'ingredient n'est pas dans la dropping zone
                        if (!ingredient->InDroppingZone)
                        {
                            //si l'Ai n'a rien dans ses mains
                            if (possessedAi->currentIngredient == nullptr)
                            {
                                //si l'Ai peu le pickup
                                if (possessedAi->FoodType == ingredient->IngredientType)
                                {
                                    //pickup ingredient
                                    // Def l'acteur comme TargetIngredient
                                    BlackboardComponent->SetValueAsObject(TEXT("TargetIngredient"), Actor);
                                    GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Green,
                                                                     FString::Printf(
                                                                         TEXT("TargetIngredient set to: %s"),
                                                                         *Actor->GetName()));

                                    // Def la position de l'acteur comme TargetLocation 
                                    FVector TargetLocation = Actor->GetActorLocation();
                                    BlackboardComponent->SetValueAsVector(TEXT("IngredientLocation"), TargetLocation);
                                }
                                else
                                {
                                    UExplorationData* data = Cast<UExplorationData>(BlackboardComponent->GetValueAsObject(TEXT("ExplorationData")));
                                    if (data)
                                    {
                                        if (!data->itemsMap[ingredient->IngredientType].Contains(ingredient))
                                        {
                                            //l'Ai averti les autres
                                            possessedAi->triggerNotify = true;
                                            data->itemsMap[ingredient->IngredientType].Add(ingredient);
                                            GEngine->AddOnScreenDebugMessage(
                                                -1, 20.0f, FColor::Blue, TEXT("Add Ingredient to array"));
                                        }
                                    }
                                }
                            }
                            else
                            {
                                //on fait apparaitre un point d'exclamation au dessus de sa tête pour dire qu'il l'a vu
                            }
                        }
                    }
                    else
                    {
                        //l'Ai shrug et continue l'exploration
                        GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, TEXT("Useless Ingredient"));
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

void AAIControllerBase::BackToNavMesh()
{
    FVector ownerLocation = GetPawn()->GetActorLocation();
    UNavigationSystemV1* navSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    FVector result;
    float randomRadius = 0;
    bool bSucess = false;
    do
    {
        randomRadius+=50;
       
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::SanitizeFloat(randomRadius));

        bSucess = navSystem->K2_GetRandomLocationInNavigableRadius(GetWorld(),ownerLocation,result,randomRadius);

        if(randomRadius > 10000000000)
        {
            break;
        }
    }while(!bSucess || ownerLocation.Equals(result) || !GetPawn()->TeleportTo(FVector(result.X,result.Y,ownerLocation.Z),GetPawn()->GetActorRotation()));
   
}

