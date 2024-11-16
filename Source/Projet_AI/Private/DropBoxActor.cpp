// Fill out your copyright notice in the Description page of Project Settings.


#include "DropBoxActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

// Sets default values
ADropBoxActor::ADropBoxActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	RootComponent = BoxCollision;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(BoxCollision);
}

// Called when the game starts or when spawned
void ADropBoxActor::BeginPlay()
{
	Super::BeginPlay();
	
	this->OnActorBeginOverlap.AddDynamic(this, &ADropBoxActor::OnBeginOverlap);
	this->OnActorEndOverlap.AddDynamic(this, &ADropBoxActor::OnEndOverlap);
}

// Called every frame
void ADropBoxActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADropBoxActor::OnBeginOverlap(AActor* thisActor, AActor* OtherActor)
{
	if(auto ingredient = Cast<AIngredient>(OtherActor))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("ingredient Added"));	
		currentIngredients.Add(ingredient);
	}
}

void ADropBoxActor::OnEndOverlap(AActor* thisActor, AActor* OtherActor)
{
	if(auto ingredient = Cast<AIngredient>(OtherActor))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("ingredient removed"));	
		currentIngredients.Remove(ingredient);
	}
}


