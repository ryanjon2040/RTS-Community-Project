/*
	
	By Rama

*/
#pragma once

//~~~~~~~~~~~~ UMG ~~~~~~~~~~~~~~~
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//									Includes for other's Nodes
//Audio
#include "Components/AudioComponent.h"
#include "AudioDecompress.h"
#include "AudioDevice.h"
#include "ActiveSound.h"
#include "Audio.h"
#include "Developer/TargetPlatform/Public/Interfaces/IAudioFormat.h"
#include "VorbisAudioInfo.h"
     

//Texture2D
//#include "Engine/Texture2D.h"
#include "DDSLoader.h"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#include "VictoryBPFunctionLibrary.generated.h"

// BP Library for You
//
// Written by Rama

//note about UBlueprintFunctionLibrary
// This class is a base class for any function libraries exposed to blueprints.
// Methods in subclasses are expected to be static, and no methods should be added to the base class.


USTRUCT(BlueprintType)
struct FVictoryInput
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input Song")
	FString ActionName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input Song")
	FKey Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input Song")
	FString KeyAsString;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input Song")
	uint32 bShift:1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input Song")
	uint32 bCtrl:1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input Song")
	uint32 bAlt:1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input Song")
	uint32 bCmd:1;
	
	   
	FVictoryInput(){}
	FVictoryInput(const FString InActionName, const FKey InKey, const bool bInShift, const bool bInCtrl, const bool bInAlt, const bool bInCmd)
		: Key(InKey)
		, KeyAsString(Key.GetDisplayName().ToString())
		, bShift(bInShift)
		, bCtrl(bInCtrl)
		, bAlt(bInAlt)
		, bCmd(bInCmd)
	{ 
		ActionName = InActionName;
	}
	
	FVictoryInput(const FInputActionKeyMapping& Action)
		: Key(Action.Key)
		, KeyAsString(Action.Key.GetDisplayName().ToString())
		, bShift(Action.bShift)
		, bCtrl(Action.bCtrl)
		, bAlt(Action.bAlt)
		, bCmd(Action.bCmd)
	{  
		ActionName = Action.ActionName.ToString();
	}
}; 
 
UENUM(BlueprintType)
namespace EJoyGraphicsFullScreen
{
	//256 entries max
	enum Type
	{
		FullScreen								UMETA(DisplayName="Regular Full Screen"),
		WindowedFullScreen					UMETA(DisplayName="Windowed Full Screen High Quality"),
		WindowedFullScreenPerformance		UMETA(DisplayName="Windowed Full Screen (Default)"),
		//~~~
		
		//256th entry
		EJoyGraphicsFullScreen_Max		UMETA(Hidden),
	};
}


UCLASS()
class VICTORYBPLIBRARY_API UVictoryBPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
	
	//~~~~ Key Re Binding ! ~~~~
	  
	UFUNCTION(BlueprintPure, Category = "VictoryBPLibrary|Key Rebinding")
	static FVictoryInput VictoryGetVictoryInput(const FKeyEvent& KeyEvent);
	
	
	static FORCEINLINE void UpdateActionMapping(FInputActionKeyMapping& Destination, const FVictoryInput& VictoryInputBind)
	{
		Destination.Key		= VictoryInputBind.Key;
		Destination.bShift	= VictoryInputBind.bShift;
		Destination.bCtrl		= VictoryInputBind.bCtrl;	
		Destination.bAlt			= VictoryInputBind.bAlt;
		Destination.bCmd 		= VictoryInputBind.bCmd;
	}

	UFUNCTION(BlueprintPure, Category = "VictoryBPLibrary|Key Rebinding")
	static void VictoryGetAllKeyBindings(TArray<FVictoryInput>& Bindings);
	
	/** You can leave the AsString field blank :) Returns false if the key could not be found as an existing mapping!  Enjoy! <3  Rama */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary|Key Rebinding")
	static bool VictoryReBindKey(FVictoryInput Action);
	
	/*
		To do = axis key bindings, check out same place where  FInputActionKeyMapping is defined
	*/
	 
	//~~~~~~~~~~~~~~~~~~~~
	
	/** Change volume of Sound class of your choosing, sets the volume instantly! Returns false if the sound class was not found and volume was not set. */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary|Sound")
	static bool VictorySoundVolumeChange(USoundClass* SoundClassObject, float NewVolume);
	
	/** Get Current Sound Volume! Returns -1 if sound class was not found*/
	UFUNCTION(BlueprintPure, Category = "VictoryBPLibrary|Sound")
	static float VictoryGetSoundVolume(USoundClass* SoundClassObject);
	
	//~~~~~~~~~~~~~~~~~~~~
	
	
	
	
	
	
	
	
	/** Get Custom Config Var! These are stored in Saved/Config/Windows/Game.ini */
	UFUNCTION(BlueprintPure, Category = "VictoryBPLibrary|Custom Config Vars!")
	static bool VictoryGetCustomConfigVar_Bool(FString SectionName,FString VariableName);
	
	/** Get Custom Config Var! These are stored in Saved/Config/Windows/Game.ini */
	UFUNCTION(BlueprintPure, Category = "VictoryBPLibrary|Custom Config Vars!")
	static int32 VictoryGetCustomConfigVar_Int(FString SectionName,FString VariableName);
	
	/** Get Custom Config Var! These are stored in Saved/Config/Windows/Game.ini */
	UFUNCTION(BlueprintPure, Category = "VictoryBPLibrary|Custom Config Vars!")
	static float VictoryGetCustomConfigVar_Float(FString SectionName,FString VariableName);
	
	/** Get Custom Config Var! These are stored in Saved/Config/Windows/Game.ini */
	UFUNCTION(BlueprintPure, Category = "VictoryBPLibrary|Custom Config Vars!")
	static FVector VictoryGetCustomConfigVar_Vector(FString SectionName,FString VariableName);
	
	/** Get Custom Config Var! These are stored in Saved/Config/Windows/Game.ini */
	UFUNCTION(BlueprintPure, Category = "VictoryBPLibrary|Custom Config Vars!")
	static FRotator VictoryGetCustomConfigVar_Rotator(FString SectionName,FString VariableName);
	
	/** Get Custom Config Var! These are stored in Saved/Config/Windows/Game.ini */
	UFUNCTION(BlueprintPure, Category = "VictoryBPLibrary|Custom Config Vars!")
	static FLinearColor VictoryGetCustomConfigVar_Color(FString SectionName,FString VariableName);
	
	/** Get Custom Config Var! These are stored in Saved/Config/Windows/Game.ini */
	UFUNCTION(BlueprintPure, Category = "VictoryBPLibrary|Custom Config Vars!")
	static FString VictoryGetCustomConfigVar_String(FString SectionName,FString VariableName);
	
	//~~~~~~~~~~~~~~~~~~~~
	
	/** Set Custom Config Var! These are stored in Saved/Config/Windows/Game.ini */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary|Custom Config Vars!")
	static void VictorySetCustomConfigVar_Bool(FString SectionName,FString VariableName, bool Value);
	
	/** Set Custom Config Var! These are stored in Saved/Config/Windows/Game.ini */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary|Custom Config Vars!")
	static void VictorySetCustomConfigVar_Int(FString SectionName,FString VariableName, int32 Value);
	
	/** Set Custom Config Var! These are stored in Saved/Config/Windows/Game.ini */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary|Custom Config Vars!")
	static void VictorySetCustomConfigVar_Float(FString SectionName,FString VariableName, float Value);
	
	/** Set Custom Config Var! These are stored in Saved/Config/Windows/Game.ini */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary|Custom Config Vars!")
	static void VictorySetCustomConfigVar_Vector(FString SectionName,FString VariableName, FVector Value);
	
	/** Set Custom Config Var! These are stored in Saved/Config/Windows/Game.ini */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary|Custom Config Vars!")
	static void VictorySetCustomConfigVar_Rotator(FString SectionName,FString VariableName, FRotator Value);
	
	/** Set Custom Config Var! These are stored in Saved/Config/Windows/Game.ini */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary|Custom Config Vars!")
	static void VictorySetCustomConfigVar_Color(FString SectionName,FString VariableName, FLinearColor Value);
	
	
	/** Set Custom Config Var! These are stored in Saved/Config/Windows/Game.ini */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary|Custom Config Vars!")
	static void VictorySetCustomConfigVar_String(FString SectionName,FString VariableName, FString Value);
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	/** The FName that is expected is the exact same format as when you right click on asset -> Copy Reference! You can directly paste copied references into this node! IsValid lets you know if the path was correct or not and I was able to load the object. MAKE SURE TO SAVE THE RETURNED OBJECT AS A VARIABLE. Otherwise your shiny new asset will get garbage collected. I recommend you cast the return value to the appropriate object and then promote it to a variable :)  -Rama */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
	static UObject* LoadObjectFromAssetPath(TSubclassOf<UObject> ObjectClass, FName Path,bool& IsValid);
	
	/** Uses the same format as I use for LoadObjectFromAssetPath! Use this node to get the asset path of objects in the world! -Rama */
	UFUNCTION(BlueprintPure, Category = "VictoryBPLibrary")
	static FName GetObjectPath(UObject* Obj);
	
	
	/** Find all widgets of a certain class! Top level only means only widgets that are directly added to the viewport will be found */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary|UMG",meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject" ))
	static void GetAllWidgetsOfClass(UObject* WorldContextObject, TSubclassOf<UUserWidget> WidgetClass, TArray<UUserWidget*>& FoundWidgets, bool TopLevelOnly=true);
	
	/** Remove all widgets of a certain class from viewport! */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary|UMG",meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject" ))
	static void RemoveAllWidgetsOfClass(UObject* WorldContextObject, TSubclassOf<UUserWidget> WidgetClass);
	
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary|UMG",meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject" ))
	static bool IsWidgetOfClassInViewport(UObject* WorldContextObject, TSubclassOf<UUserWidget> WidgetClass);
	
	
	/** Retrieves the unique net ID for the local player as a number! The number itself will vary based on what Online Subsystem is being used, but you are guaranteed that this number is unique per player! */
	UFUNCTION(BlueprintPure, Category = "VictoryBPLibrary")
	static int32 GetPlayerUniqueNetID();
	
	/** Call this periodically in a huge loop you are intentionally using to reset the BP runaway loop system. Caution, if you have an infinite loop this will permanently hang your system until you turn your computer off. Use very very carefully! When constructing a new loop and you are not sure if it is totally stable, do NOT use this node! Always test new loops normally to ensure you dont truly have a runaway loop that would hang your computer forever. */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
	static void Loops_ResetBPRunawayCounter();
	  
	/** Set the Max Frame Rate. Min value is 10. */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary|Graphics Settings")
	static void GraphicsSettings__SetFrameRateCap(float NewValue);
	
	/** Only hardware dependent, no smoothing */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary|Graphics Settings")
	static void GraphicsSettings__SetFrameRateToBeUnbound();
	
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
	static FVector2D ProjectWorldToScreenPosition(const FVector& WorldLocation);
	
	/** Make sure to save off the return value as a global variable in one of your BPs or else it will get garbage collected! */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary",meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject" ))
	static UObject* CreateObject(UObject* WorldContextObject, UClass* TheObjectClass, FName Name);
	
	/** Make sure to save off the return value as a global variable in one of your BPs or else it will get garbage collected! */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary",meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject" ))
	static UPrimitiveComponent* CreatePrimitiveComponent(UObject* WorldContextObject, TSubclassOf<UPrimitiveComponent> CompClass, FName Name, FVector Location, FRotator Rotation);
	
	 
	
	/** Spawn an Actor and choose which level you want them to spawn into! */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary",meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject" ))
	static AActor* SpawnActorIntoLevel(UObject* WorldContextObject, TSubclassOf<AActor> ActorClass, FName Level=NAME_None,FVector Location=FVector::ZeroVector, FRotator Rotation = FRotator::ZeroRotator, bool SpawnEvenIfColliding=true);
	
	/** Get the names of all currently loaded and visible levels! */
	UFUNCTION(BlueprintPure, Category = "VictoryBPLibrary",meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject" ))
	static void GetNamesOfLoadedLevels(UObject* WorldContextObject, TArray<FString>& NamesOfLoadedLevels);
	
	
	
	
	/** Obtain the scaled,rotated, and translated vertex positions for any static mesh! Returns false if operation could not occur because the comp or static mesh asset was invalid. <3 Rama */
	UFUNCTION(BlueprintPure, Category = "VictoryBPLibrary")
	static bool GetStaticMeshVertexLocations(UStaticMeshComponent* Comp, TArray<FVector>& VertexPositions);
	
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
	static void AddToActorRotation(AActor* TheActor, FRotator AddRot);
	
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary",meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject" ))
	static void DrawCircle(
		UObject* WorldContextObject, 
		FVector Center, 
		float Radius, 
		int32 NumPoints=32, 
		float Thickness=7, 
		FLinearColor LineColor=FLinearColor::Red, 
		FVector YAxis= FVector(0,1,0),
		FVector ZAxis= FVector(0,0,1),
		float Duration=0, 
		bool PersistentLines=false
	);
	
	
	
	/** Implementation of a Selection Marquee / Selection Box as a BP Node. AnchorPoint is the first clicked point, which user then drags from to make the box. Class filter is optional way to narrow the scope of actors that can be selected by the selection box! -Rama*/
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary", meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject" ))
	static void Selection_SelectionBox(UObject* WorldContextObject,TArray<AActor*>& SelectedActors, FVector2D AnchorPoint,FVector2D DraggedPoint,TSubclassOf<AActor> ClassFilter);
	
	
	/** Get the Controller ID for a supplied Player Controller <3 Rama. Returns false if operation could not occur.  */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
	static bool PlayerController_GetControllerID(APlayerController* ThePC, int32& ControllerID);
	
	/** Get the Unique PlayerID from the PlayerState for a supplied Player Controller <3 Rama. Returns false if operation could not occur. */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
	static bool PlayerState_GetPlayerID(APlayerController* ThePC, int32& PlayerID);
	
	
	/** Launches the specified URL in the OS default web browser :) <3 Rama */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
	static void Open_URL_In_Web_Browser(FString TheURL);
	
	
	
	/** Returns which platform the game code is running in.*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VictoryBPLibrary")
	static void OperatingSystem__GetCurrentPlatform(
		bool& Windows_, 
		bool& Mac,
		bool& Linux, 
		bool& iOS, 
		bool& Android,
		bool& PS4,
		bool& XBoxOne,
		bool& HTML5,
		bool& WinRT_Arm,
		bool& WinRT
	);
	
	//~~~
	
	/** Retrieves the OS system Date and Time as a string at the instant this BP node runs. Use my other RealWorldTime node to get the time passed since the return value of this node! You can use this to record milliseconds/seconds/minutes/hours between events in game logic! */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VictoryBPLibrary")
	static FString RealWorldTime__GetCurrentOSTime( 
		int32& MilliSeconds,
		int32& Seconds, 
		int32& Minutes, 
		int32& Hours12,
		int32& Hours24,
		int32& Day,
		int32& Month,
		int32& Year
	);
	
	/** Get the amount of seconds/minutes/hours since the the supplied DateTime string! You can use this to record milliseconds/seconds/minutes/hours between events in game logic! */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VictoryBPLibrary")
	static void RealWorldTime__GetTimePassedSincePreviousTime(
		const FString& PreviousTime,
		float& Milliseconds,
		float& Seconds,
		float& Minutes,
		float& Hours
	);
	
	/** Get the difference between two recorded times! You can use this to record milliseconds/seconds/minutes/hours between events in game logic! */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VictoryBPLibrary")
	static void RealWorldTime__GetDifferenceBetweenTimes(
		const FString& PreviousTime1,
		const FString& PreviousTime2,
		float& Milliseconds,
		float& Seconds,
		float& Minutes,
		float& Hours
	);
	
	//~~~
	
	/** Loads a text file from hard disk and parses it into a String array, where each entry in the string array is 1 line from the text file. Option to exclude lines that are only whitespace characters or '\n'. Returns the size of the final String Array that was created. Returns false if the file could be loaded from hard disk. */
	UFUNCTION(BlueprintPure, Category = "VictoryBPLibrary")
	static bool LoadStringArrayFromFile(TArray<FString>& StringArray, int32& ArraySize, FString FullFilePath="Enter Full File Path", bool ExcludeEmptyLines=false);
	
	//~~~
	
	/** Max of all array entries. Returns -1 if the supplied array is empty. Returns the index of the max value as well as the value itself. */
	UFUNCTION(BlueprintPure, Category = "VictoryBPLibrary")
	static void MaxOfFloatArray(const TArray<float>& FloatArray, int32& IndexOfMaxValue, float& MaxValue);
	
	/** Max of all array entries. Returns -1 if the supplied array is empty. Returns the index of the max value as well as the value itself. */
	UFUNCTION(BlueprintPure, Category = "VictoryBPLibrary")
	static void MaxOfIntArray(const TArray<int32>& IntArray, int32& IndexOfMaxValue, int32& MaxValue);
	
	/** Min of all array entries. Returns -1 if the supplied array is empty. Returns the index of the min value as well as the value itself. */
	UFUNCTION(BlueprintPure, Category = "VictoryBPLibrary")
	static void MinOfFloatArray(const TArray<float>& FloatArray, int32& IndexOfMinValue, float& MinValue);
	
	/** Min of all array entries. Returns -1 if the supplied array is empty. Returns the index of the min value as well as the value itself. */
	UFUNCTION(BlueprintPure, Category = "VictoryBPLibrary")
	static void MinOfIntArray(const TArray<int32>& IntArray, int32& IndexOfMinValue, int32& MinValue);
	
	//~~~
	
	/** Set Max Move Speed. Supply the Character whose Character Movement to change! Returns false if operation could not occur due to invalid Character or MovementComponent could not be obtained.*/
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary", meta=(DefaultToSelf="TheCharacter"))
	static bool CharacterMovement__SetMaxMoveSpeed(ACharacter* TheCharacter, float NewMaxMoveSpeed);
	
	//~~~
	
	/** Converts a float to a rounded Integer, examples: 1.4 becomes 1,   1.6 becomes 2 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VictoryBPLibrary")
	static int32 Conversion__FloatToRoundedInteger(float IN_Float);
	
	/** Combines two strings together! The Separator and the Labels are optional*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VictoryBPLibrary")
	static FString String__CombineStrings(FString StringFirst, FString StringSecond, FString Separator="", FString StringFirstLabel="", FString StringSecondLabel="");
	
	
	/** Returns three arrays containing all of the resolutions and refresh rates for the current computer's current display adapter. You can loop over just 1 of the arrays and use the current index for the other two arrays, as all 3 arrays will always have the same length. Returns false if operation could not occur. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VictoryBPLibrary", meta=(Keywords="screen resolutions display adapater"))
	static bool OptionsMenu__GetDisplayAdapterScreenResolutions(TArray<int32>& Widths, TArray<int32>& Heights, TArray<int32>& RefreshRates, bool IncludeRefreshRates=false);
	
	/** Clones an actor by obtaining its class and creating a copy. Returns the created Actor. The cloned actor is set to have the rotation and location of the initial actor. You can optionally specify location / rotation offsets for the new clone from original actor. Use IsValid to know if the actor was able to be cloned. */
	UFUNCTION(BlueprintCallable, Category="VictoryBPLibrary", meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject" ) )
	static AStaticMeshActor* Clone__StaticMeshActor(UObject* WorldContextObject, bool&IsValid, AStaticMeshActor* ToClone, FVector LocationOffset=FVector(0,0,0), FRotator RotationOffset=FRotator(0,0,0));
	
	
	/** Teleport Actor To Actor. Moves an actor instantly to the position and rotation of another actor. Useful for player starts, notes, triggers, and any other destination actors who dont have collision. Returns false if the operation could not occur. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="VictoryBPLibrary")
	static bool Actor__TeleportToActor(AActor* ActorToTeleport, AActor* DestinationActor);
	
	/** Is this game logic running in the Editor world? */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="VictoryBPLibrary", meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject" ) )
	static bool WorldType__InEditorWorld(UObject* WorldContextObject);
	
	/** Is this game logic running in the PIE world? */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="VictoryBPLibrary", meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject" ) )
	static bool WorldType__InPIEWorld(UObject* WorldContextObject);
	
	/** Is this game logic running in an actual independent game instance? */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="VictoryBPLibrary", meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject" ) )
	static bool WorldType__InGameInstanceWorld(UObject* WorldContextObject);
	
	/** Cause a supplied Character (casted from Actor internally) to enter Ragdoll physics. A check will be done to see if the character has a physics asset. */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
	static bool Physics__EnterRagDoll(AActor* TheCharacter);
	
	/** Cause a supplied Character (casted from Actor internally) to exit Ragdoll physics. HeightAboveRBMesh is how far above the RB Mesh the Actor Capsule should be moved to upon exiting. Pass in the InitLocation and InitRotation from InitializeVictoryRagdoll */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
	static bool Physics__LeaveRagDoll(AActor* TheCharacter,float HeightAboveRBMesh=64,const FVector& InitLocation=FVector(0,0,0), const FRotator& InitRotation=FRotator(0,0,0));

	/** Returns whether or not a supplied Character is in Ragdoll Physics. Cast from Actor done internally for your convenience. */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
	static bool Physics__IsRagDoll(AActor* TheCharacter);
	
	/** Get the Ragdoll Position of the supplied actor, casted to Character internally. Returns false if operation could not occur. */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
	static bool Physics__GetLocationofRagDoll(AActor* TheCharacter,FVector& RagdollLocation);
	
	/** Initialize Victory Ragdoll Mode, by Obtaining the Default Relative Rotation and Location for this Character's Mesh. The Output Location and Rotation must be saved for use with LeaveRagdoll. Returns false if operation could not occur */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
	static bool Physics__InitializeVictoryRagDoll(AActor* TheCharacter, FVector&InitLocation, FRotator&InitRotation);
	
	/** Update the TheCharacter's Capsule Location and Camera to the Current Location of the Ragdoll. InterpSpeed is how fast the camera keeps up with the moving ragdoll! HeightOffset is the height above the ragdoll that the camera should stay. Returns false if operation could not occur or if Mesh was not in ragdoll */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
	static bool Physics__UpdateCharacterCameraToRagdollLocation(AActor* TheCharacter, float HeightOffset=128,float InterpSpeed=3);
	
	/** Get Name as String*/
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
	static FString Accessor__GetNameAsString(const UObject* TheObject);
	
	/** Get Socket Local Transform. Returns false if operation could not occur.*/
	//UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
	//static bool Accessor__GetSocketLocalTransform(const USkeletalMeshComponent* Mesh, FTransform& LocalTransform, FName SocketName=FName("SocketName"));
	
	/** Get Player Character's Player Controller. Requires: The Passed in Actor must be a character and it must be a player controlled character. IsValid will tell you if the return value is valid, make sure to do a Branch to confirm this! */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
	static APlayerController* Accessor__GetPlayerController(AActor* TheCharacter, bool&IsValid);
	
	/** SET the Mouse Position! Returns false if the operation could not occur */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
	static bool Viewport__SetMousePosition(const APlayerController* ThePC, const float& PosX, const float& PosY);
	
	/** Get the Cursor Position within the Player's Viewport. This will return a result consistent with SET Mouse Position Returns false if the operation could not occur */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
	static bool Viewport__GetMousePosition(const APlayerController* ThePC, float& PosX, float& PosY);
	
	/** Get the coordinates of the center of the player's screen / viewport. Returns false if the operation could not occur */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
	static bool Viewport__GetCenterOfViewport(const APlayerController* ThePC, float& PosX, float& PosY);
	
	
	
	/** Convert Rotator to Vector */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
	static FRotator Conversions__VectorToRotator(const FVector& TheVector);
	
	/** Convert Vector to Rotator*/
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
	static FVector Conversions__RotatorToVector(const FRotator& TheRotator);
	
	/** Input Actor is expected to be a ACharacter, conversion done internally for your convenience */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
	static FRotator Character__GetControllerRotation(AActor * TheCharacter);
	
	/** Draw 3D Line of Chosen Thickness From Socket on Character's Mesh to Destination, conversion of AActor to ACharacter done internally for your convenience. Duration is in Seconds */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
	static void Draw__Thick3DLineFromCharacterSocket(AActor* TheCharacter, const FVector& EndPoint, FName Socket=FName("SocketName"), FLinearColor LineColor=FLinearColor(1,0,0,1),  float Thickness=7, float Duration=-1.f);
	/** Draw 3D Line of Chosen Thickness From Mesh Socket to Destination. Duration is in Seconds */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
	static void Draw__Thick3DLineFromSocket(USkeletalMeshComponent* Mesh, const FVector& EndPoint, FName Socket=FName("SocketName"), FLinearColor LineColor=FLinearColor(0,1,0,1), float Thickness=7, float Duration=-1.f);
	/** Draw 3D Line of Chosen Thickness Between Two Actors. Duration is in Seconds */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
	static void Draw__Thick3DLineBetweenActors(AActor* StartActor, AActor* EndActor, FLinearColor LineColor=FLinearColor(0,0,1,1), float Thickness=7, float Duration=-1.f);
	
	/** AnimBPOwner - Must be a Character, Conversion Internally For Convenience.\n\nRetrieves the Aim Offsets Pitch & Yaw Based On the Rotation of the Controller of The Character Owning The Anim Instance.\n\nThe Pitch and Yaw are meant to be used with a Blend Space going from -90,-90 to 90,90.\n   Returns true if function filled the pitch and yaw vars successfully */
	UFUNCTION(BlueprintCallable, Category="VictoryBPLibrary")
	static bool Animation__GetAimOffsets(AActor* AnimBPOwner, float& Pitch, float& Yaw);
	
	/** AnimBPOwner - Must be a Character, Conversion Internally For Convenience.\n\nRetrieves the Aim Offsets Pitch & Yaw for the AnimBPOwner Based On the supplied Rotation.\n\nThe Pitch and Yaw are meant to be used with a Blend Space going from -90,-90 to 90,90.\n    Returns true if function filled the pitch and yaw vars successfully */
	UFUNCTION(BlueprintCallable, Category="VictoryBPLibrary")
	static bool Animation__GetAimOffsetsFromRotation(AActor * AnimBPOwner, const FRotator & TheRotation, float & Pitch, float & Yaw);
	
	/** Saves text to filename of your choosing, make sure include whichever file extension you want in the filename, ex: SelfNotes.txt . Make sure to include the entire file path in the save directory, ex: C:\MyGameDir\BPSavedTextFiles */
	UFUNCTION(BlueprintCallable, Category="VictoryBPLibrary")
	static bool FileIO__SaveStringTextToFile(FString SaveDirectory, FString JoyfulFileName, FString SaveText, bool AllowOverWriting = false);
	
	/** Obtain an Array of Actors Rendered Recently. You can specifiy what qualifies as "Recent" in seconds. */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
	static void Visibility__GetRenderedActors(TArray<AActor*>& CurrentlyRenderedActors, float MinRecentTime=0.01);
	
	/** Obtain an Array of Actors NOT Rendered Recently! You can specifiy what qualifies as "Recent" in seconds. */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
	static void Visibility__GetNotRenderedActors(TArray<AActor*>& CurrentlyNotRenderedActors, float MinRecentTime=0.01);
	
	/** Is the Current Game Window the Foreground window in the OS, or in the Editor? This will be accurate in standalone running of the game as well as in the editor PIE */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
	static bool ClientWindow__GameWindowIsForeGroundInOS();
	
	/** Freeze Game Render, Does Not Stop Game Logic, Just Rendering. This is not like Pausing. Mainly useful for freezing render when window is not in the foreground */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
	static void Rendering__FreezeGameRendering();
	
	/** Unfreeze Game Render. This is not an unpause function, it's just for actual screen rendering */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
	static void Rendering__UnFreezeGameRendering();
	
	/** Compare Source Vector against Array of Vectors. Returns: Returns the Closest Vector to Source and what that closest Distance is, or -1 if there was an error such as array being empty. Ignores: Ignores Source if source is in the array */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
	static float Calcs__ClosestPointToSourcePoint(const FVector & Source, const TArray<FVector>& OtherPoints, FVector& ClosestPoint);
	
	/** Takes in an actor (for convenience) and tries to cast it to Character and return an array of Vectors of all of the current bone locations of the character's Mesh. Locations are in World Space. Returns: false if the operation could not occur. Requires: Character Mesh Component must be valid */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
	static bool Data__GetCharacterBoneLocations(AActor * TheCharacter, TArray<FVector>& BoneLocations);
	
	/** Retrieves the "Mesh" component of a Character. IsValid lets you know if the data is valid, make sure to check if it is! Requires: the passed in Actor must be a Character */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
	static USkeletalMeshComponent* Accessor__GetCharacterSkeletalMesh(AActor* TheCharacter, bool& IsValid);
	
	/** Does Not Do A Trace, But Obtains the Start and End for doing a Trace:\n\nTakes in an actor (for convenience) and tries to cast it to Character. Takes in a socket name to find on the Character's Mesh component, the socket location will be the start of the trace.\n\nAlso takes in the Angle / Rotator and the length of the trace you want to do. Option to draw the trace with variable thickness as it occurs.\n\nReturns what the Trace Start and End should be so you can plug these into any existing trace node you want.\n\nRequires: Character Mesh Component must be valid. Returns False if trace could not be done */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary|Traces")
	static bool TraceData__GetTraceDataFromCharacterSocket( FVector& TraceStart, FVector& TraceEnd, AActor * TheCharacter, const FRotator& TraceRotation, float TraceLength=10240, FName Socket="SocketName", bool DrawTraceData=true, FLinearColor TraceDataColor=FLinearColor(1,0,0,1), float TraceDataThickness=7);
	
	/** Does Not Do A Trace, But Obtains the Start and End for doing a Trace:\n\nTakes in a Skeletal Mesh Component and a socket name to trace from. Also takes in the Angle / Rotator and the length of the trace you want to do.\n\nOption to draw the trace as a variable thickness line\n\nReturns what the Trace Start and End should be so you can plug these into any existing trace node you want.\n\n Requires: Mesh must be valid. Returns False if trace could not be done */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary|Traces")
	static bool TraceData__GetTraceDataFromSkeletalMeshSocket(FVector& TraceStart, FVector& TraceEnd, USkeletalMeshComponent* Mesh, const FRotator& TraceRotation, float TraceLength=10240, FName Socket="SocketName", bool DrawTraceData=true, FLinearColor TraceDataColor=FLinearColor(1,0,0,1), float TraceDataThickness=7);
	
	/** Does a simple line trace given Trace Start and End, and if a Character is hit by the trace, then a component trace is performed on that character's mesh. Trace Owner is ignored when doing the trace.\n\nReturns the Character that was hit, as an Actor, as well as the name of the bone that was closest to the impact point of the trace. Also returns the impact point itself as well as the impact normal.\n\nUsing component trace ensures accuracy for testing against bones and sockets.\n\nIsValid: Will be true only if the component trace also hit someting. But the Returned Actor will contain an actor if any actor at all was hit by the simple trace. */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary|Traces")
	static AActor* Traces__CharacterMeshTrace___ClosestBone(AActor* TraceOwner, const FVector& TraceStart, const FVector& TraceEnd, FVector& OutImpactPoint, FVector& OutImpactNormal, FName& ClosestBoneName,FVector & ClosestBoneLocation, bool&IsValid);
	
	/** Does a simple line trace given Trace Start and End, and if a Character is hit by the trace, then a component trace is performed on that character's mesh. Returns the name of the socket that was closest to the impact point of the trace. Also returns the impact point itself as well as the impact normal. Also returns the Socket Location. Using component trace ensures accuracy for testing against bones and sockets.*/
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary|Traces")
	static AActor* Traces__CharacterMeshTrace___ClosestSocket(const AActor * TraceOwner, const FVector& TraceStart, const FVector& TraceEnd, FVector& OutImpactPoint, FVector& OutImpactNormal, FName& ClosestSocketName,FVector & SocketLocation,bool&IsValid);
	
	/** Returns the float as a String with Precision, Precision 0 = no decimal value. Precison 1 = 1 decimal place. The re-precisioned result is rounded appropriately. */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
	static void StringConversion__GetFloatAsStringWithPrecision(float TheFloat, FString & TheString, uint8 Precision=2);
	
	/** Rotator out value is the degrees of difference between the player camera and the direction of player to light source. Returns false if the operation could not occur. */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
	static bool LensFlare__GetLensFlareOffsets(APlayerController* PlayerController,AActor* LightSource, float& PitchOffset, float& YawOffset, float& RollOffset);
	
	//~~~~~~~~~~~~~
	
	/** Returns false if the operation could not occur. */
	//UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
	//static bool AnimatedVertex__GetAnimatedVertexLocations(USkeletalMeshComponent* Mesh, TArray<FVector>& Locations );
	
	/** Returns false if the operation could not occur. */
	//UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
	//static bool AnimatedVertex__GetAnimatedVertexLocationsAndNormals(USkeletalMeshComponent* Mesh, TArray<FVector>& Locations, TArray<FVector>& Normals );
	
	/** 0 never skip, 0.5 = 50% chance to skip, 1 = skip all. Returns false if the operation could not occur. */
	//UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary", meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject" ))
	//static bool AnimatedVertex__DrawAnimatedVertexLocations(UObject* WorldContextObject, USkeletalMeshComponent* Mesh, float ChanceToSkipAVertex=0.777, bool DrawNormals=false);
	
	//~~~~~~~~~~~~~
	
	/** Returns false if the operation could not occur. */
	//UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary", meta=(DefaultToSelf="TheCharacter"))
	//static bool AnimatedVertex__GetCharacterAnimatedVertexLocations(AActor* TheCharacter, TArray<FVector>& Locations );
	
	/** Returns false if the operation could not occur. */
	//UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary", meta=(DefaultToSelf="TheCharacter"))
	//static bool AnimatedVertex__GetCharacterAnimatedVertexLocationsAndNormals(AActor* TheCharacter, TArray<FVector>& Locations, TArray<FVector>& Normals );
	
	/** 0 never skip, 0.5 = 50% chance to skip, 1 = skip all.. Returns false if the operation could not occur. */
	//UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
	//static bool AnimatedVertex__DrawCharacterAnimatedVertexLocations(AActor* TheCharacter, float ChanceToSkipAVertex=0.777, bool DrawNormals=false);
	
	/** Retrieve Distance of given point to any Surface point on a Static Mesh Actor. Returns the distance as well as the exact closest point on the mesh surface to the given point. Returns -1 if an error occurred*/
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
	static float DistanceToSurface__DistaceOfPointToMeshSurface(AStaticMeshActor* TheSMA, const FVector& TestPoint, FVector& ClosestSurfacePoint);
	
	/** Change the Mobility of a Static Mesh Component, can be used in Constructor Script or in Event Graph! Returns false if operation could not occur.*/
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
	static bool Mobility__SetSceneCompMobility(USceneComponent* SceneComp, EComponentMobility::Type NewMobility);
	
	
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//			  Graphics Settings Sample
	
	//~~~~~~~~~~~~~~~~~~
	//		FullScreen
	//~~~~~~~~~~~~~~~~~~
	
	/** Get Full Screen Type */
	UFUNCTION(BlueprintPure, Category = "VictoryBPLibrary|Graphics Settings")
	static TEnumAsByte<EJoyGraphicsFullScreen::Type> JoyGraphicsSettings__FullScreen_Get();
	
	/** Set Full Screen Type */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary|Graphics Settings")
	static void JoyGraphicsSettings__FullScreen_Set(TEnumAsByte<EJoyGraphicsFullScreen::Type> NewSetting);
	
	
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//			  CPP FUNCTION LIBRARY
	
	static FORCEINLINE void JoyCC(const FString& Str, const int32 Value)
	{
		TObjectIterator<APlayerController> PC;
		if(!PC) return;
		//~~~~~~
		
		PC->ConsoleCommand(Str + " " + FString::FromInt(Value));
	}
	static FORCEINLINE void JoyGraphics_FullScreen_SetFullScreenType(int32 Value)
	{
		JoyCC("r.FullScreenMode",Value);
	}
	static FORCEINLINE int32 JoyGraphics_FullScreen_GetFullScreenType()
	{
		static const auto CVar = IConsoleManager::Get().FindTConsoleVariableDataInt(TEXT("r.FullScreenMode")); 
		return CVar->GetValueOnGameThread();
	}
	static FORCEINLINE void JoyGraphics_SetFullScreen_NonWindowed()
	{
		/*"r.FullScreenMode"),
			2,
			TEXT("Defines how we do full screen when requested (e.g. command line option -fullscreen or in ini [SystemSettings] fullscreen=true)\n")
		*/
		JoyCC("r.FullScreenMode",0);
	}
	static FORCEINLINE void JoyGraphics_SetFullScreen_Windowed()
	{
		//TEXT(" 2: windowed full screen, specified resolution (like 1 but no unintuitive performance cliff, can be blurry, default)\n")
		JoyCC("r.FullScreenMode",2);
	}
	static FORCEINLINE void JoyGraphics_SetFullScreen_WindowedHighestQuality()
	{
		//TEXT(" 1: windowed full screen, desktop resolution (quick switch between applications and window mode, full quality)\n")
		JoyCC("r.FullScreenMode",1);
	}
	
	
	
	//Min and Max of Array
	static FORCEINLINE float Min( const TArray<float>& Values, int32* MinIndex = NULL )
	{
		if(MinIndex)
		{
			*MinIndex = 0;
		}
		
		if(Values.Num() <= 0 )
		{
			return -1;
		}
		
		float CurMin = Values[0];
		for(const float EachValue : Values )
		{
			CurMin = FMath::Min(CurMin,EachValue);
		}
		
		if(MinIndex)
		{
			*MinIndex = Values.Find(CurMin);
		}
		return CurMin;
	}
	static FORCEINLINE float Max( const TArray<float>& Values, int32* MaxIndex = NULL )
	{
		if(MaxIndex)
		{
			*MaxIndex = 0;
		}
		
		if(Values.Num() <= 0 )
		{
			return -1;
		}
		
		float CurMax = Values[0];
		for(const float EachValue : Values )
		{
			CurMax = FMath::Max(CurMax,EachValue);
		}
		
		if(MaxIndex)
		{
			*MaxIndex = Values.Find(CurMax);
		}
		return CurMax; 
	}
	
	static FORCEINLINE int32 Min( const TArray<int32>& Values,int32* MinIndex = NULL )
	{
		if(MinIndex)
		{
			*MinIndex = 0;
		}
		
		if(Values.Num() <= 0 )
		{
			return -1;
		}
		
		int32 CurMin = Values[0];
		for(const int32 EachValue : Values )
		{
			CurMin = FMath::Min(CurMin,EachValue);
		}
		
		if(MinIndex)
		{
			*MinIndex = Values.Find(CurMin);
		}
		return CurMin;
	}
	static FORCEINLINE int32 Max( const TArray<int32>& Values, int32* MaxIndex = NULL )
	{
		if(MaxIndex)
		{
			*MaxIndex = 0;
		}
		
		if(Values.Num() <= 0 )
		{
			return -1;
		}
		
		int32 CurMax = Values[0];
		for(const int32 EachValue : Values )
		{
			CurMax = FMath::Max(CurMax,EachValue);
		}
		
		if(MaxIndex)
		{
			*MaxIndex = Values.Find(CurMax);
		}
		return CurMax; 
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//			  Contributed by Others
	
	/**
	*
	* Contributed by: Mindfane
	*
	* Split a string into an array of substrings based on the given delimitter.
	* Unlike ParseIntoArray() function which expects single character delimitters, this function can accept a delimitter that is also a string.
	*
	* @param InputString - The string that is to be exploded.
	* @param Separator - The delimitter that is used for splitting (multi character strings are allowed)
	* @param limit - If greater than zero, returns only the first x strings. Otherwsie returns all the substrings
	* @param bTrimElelements - If True, then each subsctring is processed and any leading or trailing whitespcaes are trimmed.
	*/
	UFUNCTION(BlueprintPure, meta = (FriendlyName = "Explode string", Keywords = "split explode string"), Category = String)
	static void String__ExplodeString(TArray<FString>& OutputStrings, FString InputString, FString Separator = ",", int32 limit = 0, bool bTrimElements = false);
	

	/** Load a texture 2D from a file path! Contributed by UE4 forum member n00854180t! */
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
	static UTexture2D* GetTexture2DFromFile(const FString& FilePath);
	
	
	
	/** Contributed by UE4 forum member n00854180t! Plays a sound from file, attached to and following the specified component. This is a fire and forget sound. Replication is also not handled at this point.
	* @param FilePath - Path to sound file to play
	* @param AttachComponent - Component to attach to.
	* @param AttachPointName - Optional named point within the AttachComponent to play the sound at
	* @param Location - Depending on the value of Location Type this is either a relative offset from the attach component/point or an absolute world position that will be translated to a relative offset
	* @param LocationType - Specifies whether Location is a relative offset or an absolute world position
	* @param bStopWhenAttachedToDestroyed - Specifies whether the sound should stop playing when the owner of the attach to component is destroyed.
	* @param VolumeMultiplier - Volume multiplier
	* @param PitchMultiplier - PitchMultiplier
	* @param AttenuationSettings - Override attenuation settings package to play sound with
	*/ 
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary", meta = (VolumeMultiplier = "1.0", PitchMultiplier = "1.0", AdvancedDisplay = "2", UnsafeDuringActorConstruction = "true"))
	static class UAudioComponent* PlaySoundAttachedFromFile(const FString& FilePath, class USceneComponent* AttachToComponent, FName AttachPointName = NAME_None, FVector Location = FVector(ForceInit), EAttachLocation::Type LocationType = EAttachLocation::SnapToTarget, bool bStopWhenAttachedToDestroyed = false, float VolumeMultiplier = 1.f, float PitchMultiplier = 1.f, float StartTime = 0.f, class USoundAttenuation* AttenuationSettings = NULL);
	
	/** Contributed by UE4 forum member n00854180t! Plays a sound at the given location. This is a fire and forget sound and does not travel with any actor. Replication is also not handled at this point.
	* @param FilePath - Path to sound file to play
	* @param Location - World position to play sound at
	* @param World - The World in which the sound is to be played
	* @param VolumeMultiplier - Volume multiplier
	* @param PitchMultiplier - PitchMultiplier
	* @param AttenuationSettings - Override attenuation settings package to play sound with
	*/
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", VolumeMultiplier = "1.0", PitchMultiplier = "1.0", AdvancedDisplay = "3", UnsafeDuringActorConstruction = "true"))
	static void PlaySoundAtLocationFromFile(UObject* WorldContextObject, const FString& FilePath, FVector Location, float VolumeMultiplier = 1.f, float PitchMultiplier = 1.f, float StartTime = 0.f, class USoundAttenuation* AttenuationSettings = NULL);
	
	/** Contributed by UE4 forum member n00854180t! Creates a USoundWave* from file path.
	* Read .ogg header file and refresh USoundWave metadata.
	* @param FilePath		path to file to create sound wave from
	*/
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
	static class USoundWave* GetSoundWaveFromFile(const FString& FilePath);

private:
	// Thanks to @keru for the base code for loading an Ogg into a USoundWave: 
	// https://forums.unrealengine.com/showthread.php?7936-Custom-Music-Player&p=97659&viewfull=1#post97659

	     /**
        * Read .ogg header file and refresh USoundWave metadata.
        * @param sw             wave to put metadata
        * @param rawFile        pointer to src file in memory
        * @return 0     if everything is ok
        *                 1 if couldn't read metadata.
        */
        static int32 fillSoundWaveInfo(USoundWave* sw, TArray<uint8>* rawFile);



        /**
        * Tries to find out FSoundSource object associated to the USoundWave.
        * @param sw     wave, search key
        * @return 0 if wave found and correctly set
        *        -1 if error: sound device not set
        *        -2 if error: sound wave not found
        */
		static int32 findSource(class USoundWave* sw, class FSoundSource* out_audioSource);

};

