// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumFoodTypeClass.h"
#include "Data/IngredientTable.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "AICharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS(config = Game)
class PROJET_AI_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

	// Camera boom positioning the camera behind the character 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	// Follow camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;



public:
	// Sets default values for this character's properties
	AAICharacter();

	//UPROPERTY(EditAnywhere, Category = "TypeAlimentaire")
	//class UBehaviorTree* TreeAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TypeAlimentaire")
	EIngredientType FoodType;

	// Behavior Tree
	UPROPERTY(EditAnywhere, Category = "AI")
	class UBehaviorTree* TreeAsset;

	UPROPERTY()
	class ARecipeItem* currentIngredient;
	//les trucs pour les anims
	UPROPERTY(EditAnywhere)
	bool triggerGrab;
	UPROPERTY(EditAnywhere)
	bool triggerShrug;
	UPROPERTY(EditAnywhere)
	bool triggerNotify;
	UFUNCTION(BlueprintCallable)
	bool GetTriggerGrab();
	UFUNCTION(BlueprintCallable)
	bool GetTriggerShrug();
	UFUNCTION(BlueprintCallable)
	bool GetTriggerNotify();
	UFUNCTION(BlueprintCallable)
	void SetTriggerGrab(bool value);
	UFUNCTION(BlueprintCallable)
	void SetTriggerShrug(bool value);
	UFUNCTION(BlueprintCallable)
	void SetTriggerNotify(bool value);

	UFUNCTION(BlueprintCallable)
	bool Grab(ARecipeItem* targetIngredient);
	UFUNCTION(BlueprintCallable)
	void attatchIngredient(ARecipeItem* targetIngredient);
	UFUNCTION(BlueprintCallable)
	void Drop();

	UFUNCTION(BlueprintCallable)
	EIngredientType getFoodType();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
