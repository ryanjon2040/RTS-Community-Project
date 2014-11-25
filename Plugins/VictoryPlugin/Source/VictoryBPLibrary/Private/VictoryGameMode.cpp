/*

	By Rama

*/

#include "VictoryBPLibraryPrivatePCH.h"
#include "VictoryGameMode.h"
 
DEFINE_LOG_CATEGORY(VictoryGameModeLog)

//////////////////////////////////////////////////////////////////////////
// AVictoryGameMode

AVictoryGameMode::AVictoryGameMode(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

}
 
//Find
AActor* AVictoryGameMode::FindPlayerStart( AController* Player, const FString& IncomingName )
{
	//Clear any previous to indicate whether BP did an override
	CPPOverrideVar_FindPlayerStart = NULL;
	
	//=======================
	//BP Hook by Rama
	this->CPPOverride_FindPlayerStart();
	//=======================
	
	if(CPPOverrideVar_FindPlayerStart) //Did BP user set the var?
	{ 
		//Indicate that the BP override was found and is being used.
		UE_LOG(VictoryGameModeLog, Log, TEXT("\n\n"));
		UE_LOG(VictoryGameModeLog, Log, TEXT("FindPlayerStart: CPP override found in %s and used instead of C++ implementation"), *GetName());
		UE_LOG(VictoryGameModeLog, Log, TEXT("\n\n"));
		return CPPOverrideVar_FindPlayerStart;
	} 
	 
	//Default C++ Implementation
	return Super::FindPlayerStart(Player,IncomingName);
}

//Choose
AActor* AVictoryGameMode::ChoosePlayerStart( AController* Player)
{
	//Clear any previous to indicate whether BP did an override
	CPPOverrideVar_ChoosePlayerStart = NULL;
	
	//=======================
	//BP Hook by Rama
	this->CPPOverride_ChoosePlayerStart();
	//=======================
	
	if(CPPOverrideVar_ChoosePlayerStart) //Did BP user set the var?
	{ 
		//Indicate that the BP override was found and is being used.
		UE_LOG(VictoryGameModeLog, Log, TEXT("\n\n"));
		UE_LOG(VictoryGameModeLog, Log, TEXT("ChoosePlayerStart: CPP override found in %s and used instead of C++ implementation"), *GetName());
		UE_LOG(VictoryGameModeLog, Log, TEXT("\n\n"));
		return CPPOverrideVar_ChoosePlayerStart;
	} 
	 
	//Default C++ Implementation
	return Super::ChoosePlayerStart(Player);
} 

bool AVictoryGameMode::PlayerCanRestart( APlayerController* Player )
{
	//Clear any previous to indicate whether BP did an override
	CPPOverrideVar_SelfReference = NULL;
	
	//=======================
	//BP Hook by Rama
	this->CPPOverride_PlayerCanRestart(); 
	//=======================
	
	if(CPPOverrideVar_SelfReference) //Did BP user set the var?
	{ 
		//Indicate that the BP override was found and is being used.
		UE_LOG(VictoryGameModeLog, Log, TEXT("\n\n"));
		UE_LOG(VictoryGameModeLog, Log, TEXT("PlayerCanRestart: CPP override found in %s and used instead of C++ implementation"), *GetName());
		UE_LOG(VictoryGameModeLog, Log, TEXT("\n\n"));
		return CPPOverrideVar_PlayerCanRestart;
	} 
	 
	//Default C++ Implementation
	return Super::PlayerCanRestart(Player);
} 