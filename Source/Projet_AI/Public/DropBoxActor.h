// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ingredient.h"
#include "GameFramework/Actor.h"
#include "DropBoxActor.generated.h"

UCLASS()
class PROJET_AI_API ADropBoxActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxCollision;
	UPROPERTY()
	TArray<AIngredient*> requiredIngredients;
	UPROPERTY()
	TArray<AIngredient*> currentIngredients;
	
public:	
	// Sets default values for this actor's properties
	ADropBoxActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Fonction appelée lors d'un overlap
	UFUNCTION()
	void OnBeginOverlap(AActor* thisActor,AActor* OtherActor);
	UFUNCTION()
	void OnEndOverlap(AActor* thisActor, AActor* OtherActor);
};
