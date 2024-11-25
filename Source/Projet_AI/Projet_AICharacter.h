// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actor/RecipeItem.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Projet_AICharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config = Game)
class AProjet_AICharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UAnimSequence* crouchAnim;
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	//grab ingredient Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* GrabAction;
	//drop ingredient Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DropAction;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionStimuliSourceComponent* StimuliSource;

	UPROPERTY()
	class ARecipeItem* currentIngredient;
public:
	AProjet_AICharacter();

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
protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	
	void Grab();
	UFUNCTION(BlueprintCallable)
	void attatchIngredient();
	void Drop();


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

