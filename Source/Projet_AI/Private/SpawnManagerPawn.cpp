// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnManagerPawn.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"

// Sets default values
ASpawnManagerPawn::ASpawnManagerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add(FName("Enemy"));
}

// Called when the game starts or when spawned
void ASpawnManagerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnManagerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASpawnManagerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

