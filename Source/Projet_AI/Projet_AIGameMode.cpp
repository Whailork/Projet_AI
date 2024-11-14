// Copyright Epic Games, Inc. All Rights Reserved.

#include "Projet_AIGameMode.h"
#include "Projet_AICharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjet_AIGameMode::AProjet_AIGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
