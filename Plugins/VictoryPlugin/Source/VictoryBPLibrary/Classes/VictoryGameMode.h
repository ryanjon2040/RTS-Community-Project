/*

	By Rama

*/
#pragma once

#include "VictoryGameMode.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(VictoryGameModeLog, Log, All);

UCLASS()
class VICTORYBPLIBRARY_API AVictoryGameMode : public AGameMode
{
	GENERATED_UCLASS_BODY()

	/** This var is used to indicate when you are overriding a Game Mode function that returns a simple data type like bool. Ex: This is required for CPP Override Can Player Restart */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Victory Game Mode")
	UObject* CPPOverrideVar_SelfReference;
	
//Find Player Start
public:
	/** Use this var inside the body of CPP Override ~ Find Player Start event to override the C++ functionality! */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Victory Game Mode")
	AActor* CPPOverrideVar_FindPlayerStart;
	
	/** 
	 * Use CPPOverrideVar_FindPlayerStart inside of this event to implement FindPlayerStart in BP ! <3 Rama
	 *
	 * Return the 'best' player start for this player to start from.
	 * @param Player - is the AController for whom we are choosing a playerstart
	 * @param IncomingName - specifies the tag of a Playerstart to use
	 * @returns Actor chosen as player start (usually a PlayerStart)
	 */
	UFUNCTION(BlueprintImplementableEvent, meta=(FriendlyName = "CPP Override ~ Find Player Start"))
	virtual void CPPOverride_FindPlayerStart();

	virtual class AActor* FindPlayerStart( AController* Player, const FString& IncomingName = TEXT("") ) override;

//Choose Player Start
public:
	/** Use this var inside the body of CPP Override ~ Find Player Start event to override the C++ functionality! */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Victory Game Mode")
	AActor* CPPOverrideVar_ChoosePlayerStart;
	
	/** 
	* Use CPPOverrideVar_ChoosePlayerStart inside of this event to implement in BP ! <3 Rama
	*
	* @param Player is the controller for whom we are choosing a playerstart
 	* @returns AActor chosen as player start (usually a PlayerStart)
 	 */
	UFUNCTION(BlueprintImplementableEvent, meta=(FriendlyName = "CPP Override ~ Choose Player Start"))
	virtual void CPPOverride_ChoosePlayerStart();
	
 	virtual class AActor* ChoosePlayerStart( AController* Player ) override;
	
//Can Player Restart
public:
	/** Use this var inside the body of CPP Override ~ Player Can Restart event to override the C++ functionality! */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Victory Game Mode")
	bool CPPOverrideVar_PlayerCanRestart;
	
	/** 
	* Use CPPOverrideVar_PlayerCanRestart inside of this event to implement in BP ! <3 Rama
	*  Set the SelfReference to Get Reference to Self to indicate you are implementing this function in BP!
	*
	* @return true if it's valid to call RestartPlayer. Will call Player->CanRestartPlayer */
 	UFUNCTION(BlueprintImplementableEvent, meta=(FriendlyName = "CPP Override ~ Player Can Restart"))
	virtual void CPPOverride_PlayerCanRestart();
	
	virtual bool PlayerCanRestart( APlayerController* Player ) override;
	
};