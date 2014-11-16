/*
	By Rama
*/
#include "VictoryBPLibraryPrivatePCH.h"

#include "StaticMeshResources.h"

//////////////////////////////////////////////////////////////////////////
// UVictoryBPFunctionLibrary

UVictoryBPFunctionLibrary::UVictoryBPFunctionLibrary(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	
}
 
FVictoryInput UVictoryBPFunctionLibrary::VictoryGetVictoryInput(const FKeyboardEvent& KeyEvent)
{
	FVictoryInput VInput;
	 
	VInput.Key 			= KeyEvent.GetKey();
	VInput.KeyAsString 	= VInput.Key.GetDisplayName().ToString();
	
	VInput.bAlt 		= KeyEvent.IsAltDown();
	VInput.bCtrl 		= KeyEvent.IsControlDown();
	VInput.bShift 	= KeyEvent.IsShiftDown();
	VInput.bCmd 		= KeyEvent.IsCommandDown();
	
	return VInput;
}

void UVictoryBPFunctionLibrary::VictoryGetAllKeyBindings(TArray<FVictoryInput>& Bindings)
{
	Bindings.Empty();
	 
	const UInputSettings* Settings = GetDefault<UInputSettings>();
	if(!Settings) return;
	
	const TArray<FInputActionKeyMapping>& Actions = Settings->ActionMappings;
	
	for(const FInputActionKeyMapping& Each : Actions)
	{
		Bindings.Add(FVictoryInput(Each));
	}
}
bool UVictoryBPFunctionLibrary::VictoryReBindKey(FVictoryInput Action)
{
	UInputSettings* Settings = const_cast<UInputSettings*>(GetDefault<UInputSettings>());
	if(!Settings) return false;

	TArray<FInputActionKeyMapping>& Actions = Settings->ActionMappings;
	
	//~~~
	
	bool Found = false;
	for(FInputActionKeyMapping& Each : Actions)
	{
		if(Each.ActionName.ToString() == Action.ActionName)
		{  
			UVictoryBPFunctionLibrary::UpdateActionMapping(Each,Action);
			Found = true;
			break;
		}  
	}
	
	if(Found) 
	{
		//SAVES TO DISK
		const_cast<UInputSettings*>(Settings)->SaveKeyMappings();
		   
		//REBUILDS INPUT, creates modified config in Saved/Config/Windows/Input.ini
		for (TObjectIterator<UPlayerInput> It; It; ++It)
		{
			It->ForceRebuildingKeyMaps(true);
		}
	}
	return Found;
}









void UVictoryBPFunctionLibrary::GetAllWidgetsOfClass(UObject* WorldContextObject, TSubclassOf<UUserWidget> WidgetClass, TArray<UUserWidget*>& FoundWidgets,bool TopLevelOnly)
{
	//Prevent possibility of an ever-growing array if user uses this in a loop
	FoundWidgets.Empty();
	//~~~~~~~~~~~~
	 
	if(!WidgetClass) return;
	if(!WorldContextObject) return;
	 
	UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject);
	if(!World) return;
	//~~~~~~~~~~~
	
	for(TObjectIterator<UUserWidget> Itr; Itr; ++Itr)
	{
		if(Itr->GetWorld() != World) continue;
		//~~~~~~~~~~~~~~~~~~~~~
		
		if( ! Itr->IsA(WidgetClass)) continue;
		//~~~~~~~~~~~~~~~~~~~
		 
		//Top Level?
		if(TopLevelOnly)
		{
			//only add top level widgets
			if(Itr->GetIsVisible())			//IsInViewport in 4.6
			{
				FoundWidgets.Add(*Itr);
			}
		}
		else
		{
			//add all internal widgets
			FoundWidgets.Add(*Itr);
		}
	}
} 
void UVictoryBPFunctionLibrary::RemoveAllWidgetsOfClass(UObject* WorldContextObject, TSubclassOf<UUserWidget> WidgetClass)
{
	if(!WidgetClass) return;
	if(!WorldContextObject) return;
	 
	UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject);
	if(!World) return;
	//~~~~~~~~~~~
	 
	for(TObjectIterator<UUserWidget> Itr; Itr; ++Itr)
	{
		if(Itr->GetWorld() != World) continue;
		//~~~~~~~~~~~~~~~~~~~~~
		
		if( ! Itr->IsA(WidgetClass)) continue;
		//~~~~~~~~~~~~~~~~~~~
		 
		//only add top level widgets
		if(Itr->GetIsVisible())			//IsInViewport in 4.6
		{
			Itr->RemoveFromViewport();
		}
	}
}


 

bool UVictoryBPFunctionLibrary::VictorySoundVolumeChange(USoundClass* SoundClassObject, float NewVolume)
 {
	FAudioDevice* Device = GEngine->GetAudioDevice();
	if (!Device || !SoundClassObject)
	{
		return false;
	}
	    
	bool bFound = Device->SoundClasses.Contains(SoundClassObject);
	if(bFound)
	{ 
		Device->SetClassVolume(SoundClassObject, NewVolume);
		return true;
	}
	return false;
 }
float UVictoryBPFunctionLibrary::VictoryGetSoundVolume(USoundClass* SoundClassObject)
{
	FAudioDevice* Device = GEngine->GetAudioDevice();
	if (!Device || !SoundClassObject)
	{
		return -1;
	}
	    
	FSoundClassProperties* Props = Device->GetSoundClassCurrentProperties(SoundClassObject);
	if(!Props) return -1;
	return Props->Volume;
}








//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

bool UVictoryBPFunctionLibrary::VictoryGetCustomConfigVar_Bool(FString SectionName,FString VariableName)
{
	if(!GConfig) return false;
	//~~~~~~~~~~~
 
	bool Value;
	GConfig->GetBool(
		*SectionName,
		*VariableName,
		Value,
		GGameIni
	);
	return Value;
}
int32 UVictoryBPFunctionLibrary::VictoryGetCustomConfigVar_Int(FString SectionName,FString VariableName)
{
	if(!GConfig) return 0;
	//~~~~~~~~~~~
 
	int32 Value;
	GConfig->GetInt(
		*SectionName,
		*VariableName,
		Value,
		GGameIni
	);
	return Value;
}
float UVictoryBPFunctionLibrary::VictoryGetCustomConfigVar_Float(FString SectionName,FString VariableName)
{
	if(!GConfig) return 0;
	//~~~~~~~~~~~
 
	float Value;
	GConfig->GetFloat(
		*SectionName,
		*VariableName,
		Value,
		GGameIni
	);
	return Value;
}
FVector UVictoryBPFunctionLibrary::VictoryGetCustomConfigVar_Vector(FString SectionName,FString VariableName)
{
	if(!GConfig) return FVector::ZeroVector;
	//~~~~~~~~~~~
 
	FVector Value;
	GConfig->GetVector(
		*SectionName,
		*VariableName,
		Value,
		GGameIni
	);
	return Value;
}
FRotator UVictoryBPFunctionLibrary::VictoryGetCustomConfigVar_Rotator(FString SectionName,FString VariableName)
{
	if(!GConfig) return FRotator::ZeroRotator;
	//~~~~~~~~~~~
 
	FRotator Value;
	GConfig->GetRotator(
		*SectionName,
		*VariableName,
		Value,
		GGameIni
	);
	return Value;
}
FLinearColor UVictoryBPFunctionLibrary::VictoryGetCustomConfigVar_Color(FString SectionName,FString VariableName)
{
	if(!GConfig) return FColor::Black;
	//~~~~~~~~~~~
  
	FColor Value;
	GConfig->GetColor(
		*SectionName,
		*VariableName,
		Value,
		GGameIni
	);
	return FLinearColor(Value);
}
FString UVictoryBPFunctionLibrary::VictoryGetCustomConfigVar_String(FString SectionName,FString VariableName)
{
	if(!GConfig) return "";
	//~~~~~~~~~~~
 
	FString Value;
	GConfig->GetString(
		*SectionName,
		*VariableName,
		Value,
		GGameIni
	);
	return Value;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void UVictoryBPFunctionLibrary::VictorySetCustomConfigVar_Bool(FString SectionName,FString VariableName, bool Value)
{
	if(!GConfig) return;
	//~~~~~~~~~~~
 
	GConfig->SetBool(
		*SectionName,
		*VariableName,
		Value,
		GGameIni
	);
}
void UVictoryBPFunctionLibrary::VictorySetCustomConfigVar_Int(FString SectionName,FString VariableName, int32 Value)
{
	if(!GConfig) return;
	//~~~~~~~~~~~
 
	GConfig->SetInt(
		*SectionName,
		*VariableName,
		Value,
		GGameIni
	);
}
void UVictoryBPFunctionLibrary::VictorySetCustomConfigVar_Float(FString SectionName,FString VariableName, float Value)
{
	if(!GConfig) return;
	//~~~~~~~~~~~
	
	GConfig->SetFloat(
		*SectionName,
		*VariableName,
		Value,
		GGameIni
	);
}
void UVictoryBPFunctionLibrary::VictorySetCustomConfigVar_Vector(FString SectionName,FString VariableName, FVector Value)
{
	if(!GConfig) return;
	//~~~~~~~~~~~
	
	GConfig->SetVector(
		*SectionName,
		*VariableName,
		Value,
		GGameIni
	);
}
void UVictoryBPFunctionLibrary::VictorySetCustomConfigVar_Rotator(FString SectionName,FString VariableName, FRotator Value)
{
	if(!GConfig) return;
	//~~~~~~~~~~~
	
	GConfig->SetRotator(
		*SectionName,
		*VariableName,
		Value,
		GGameIni
	);
}
void UVictoryBPFunctionLibrary::VictorySetCustomConfigVar_Color(FString SectionName,FString VariableName, FLinearColor Value)
{
	if(!GConfig) return;
	//~~~~~~~~~~~
	 
	GConfig->SetColor(
		*SectionName,
		*VariableName,
		FColor(Value),
		GGameIni
	);
}
void UVictoryBPFunctionLibrary::VictorySetCustomConfigVar_String(FString SectionName,FString VariableName, FString Value)
{
	if(!GConfig) return;
	//~~~~~~~~~~~
 
	GConfig->SetString(
		*SectionName,
		*VariableName,
		*Value,
		GGameIni
	);
}






//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~























UObject* UVictoryBPFunctionLibrary::LoadObjectFromAssetPath(TSubclassOf<UObject> ObjectClass,FName Path,bool& IsValid)
{
	IsValid = false;
	
	if(Path == NAME_None) return NULL;
	//~~~~~~~~~~~~~~~~~~~~~
	
	UObject* LoadedObj = StaticLoadObject( ObjectClass, NULL,*Path.ToString());
	 
	IsValid = LoadedObj != nullptr;
	
	return LoadedObj;
}
FName UVictoryBPFunctionLibrary::GetObjectPath(UObject* Obj)
{
	if(!Obj) return NAME_None;
	if(!Obj->IsValidLowLevel()) return NAME_None;
	//~~~~~~~~~~~~~~~~~~~~~~~~~
	
	FStringAssetReference ThePath = FStringAssetReference(Obj);
		
	if(!ThePath.IsValid()) return "";
	
	//The Class FString Name For This Object
	FString str=Obj->GetClass()->GetDescription();
	
	//Remove spaces in Material Instance Constant class description!
	str = str.Replace(TEXT(" "),TEXT(""));
	
	str += "'";
	str += ThePath.ToString();
	str += "'";
	
	return FName(*str);
}
int32 UVictoryBPFunctionLibrary::GetPlayerUniqueNetID()
{
	TObjectIterator<APlayerController> ThePC;
	if(!ThePC) 					return -1; 
	if(!ThePC->PlayerState) return -1;
	//~~~~~~~~~~~~~~~~~~~
	
	return ThePC->PlayerState->PlayerId;
}
UObject* UVictoryBPFunctionLibrary::CreateObject(UObject* WorldContextObject,UClass* TheObjectClass, FName Name)
{
	if(!TheObjectClass) return NULL;
	//~~~~~~~~~~~~~~~~~
	
	//using a context object to get the world!
    UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject);
	if(!World) return NULL;
	//~~~~~~~~~~~
	 
	return StaticConstructObject( TheObjectClass, World, Name);
}
UPrimitiveComponent* UVictoryBPFunctionLibrary::CreatePrimitiveComponent(
	UObject* WorldContextObject, 
	TSubclassOf<UPrimitiveComponent> CompClass, 
	FName Name,
	FVector Location, 
	FRotator Rotation
){
	if(!CompClass) return NULL;
	//~~~~~~~~~~~~~~~~~
	
	//using a context object to get the world!
    UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject);
	if(!World) return NULL;
	//~~~~~~~~~~~
	 
	UPrimitiveComponent* NewComp = ConstructObject<UPrimitiveComponent>( CompClass, World, Name);
	if(!NewComp) return NULL;
	//~~~~~~~~~~~~~
	 
	NewComp->SetWorldLocation(Location);
	NewComp->SetWorldRotation(Rotation);
	NewComp->RegisterComponentWithWorld(World);
	
	return NewComp;
}

AActor* UVictoryBPFunctionLibrary::SpawnActorIntoLevel(UObject* WorldContextObject, TSubclassOf<AActor> ActorClass, FName Level, FVector Location, FRotator Rotation,bool SpawnEvenIfColliding)
{
	if(!ActorClass) return NULL;
	//~~~~~~~~~~~~~~~~~
	
	//using a context object to get the world!
    UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject);
	if(!World) return NULL;
	//~~~~~~~~~~~
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.bNoCollisionFail 		= SpawnEvenIfColliding;
	SpawnParameters.bDeferConstruction = false;
	 
	 
	//Get Level from Name!
	ULevel* FoundLevel = NULL;
	
	for(const ULevelStreaming* EachLevel : World->StreamingLevels)
	{
		if( ! EachLevel) continue;
		//~~~~~~~~~~~~~~~~
	
		ULevel* LevelPtr = EachLevel->GetLoadedLevel();
		
		//Valid?
		if(!LevelPtr) continue;
		
		if(EachLevel->GetWorldAssetPackageFName() == Level)
		{
			FoundLevel = LevelPtr; 
			break;
		} 
	}
	//~~~~~~~~~~~~~~~~~~~~~
	if(FoundLevel)
	{
		SpawnParameters.OverrideLevel = FoundLevel;
	}
	//~~~~~~~~~~~~~~~~~~~~~
	
	return World->SpawnActor( ActorClass, &Location, &Rotation, SpawnParameters);
	
}
void UVictoryBPFunctionLibrary::GetNamesOfLoadedLevels(UObject* WorldContextObject, TArray<FString>& NamesOfLoadedLevels)
{
	
	//using a context object to get the world!
    UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject);
	if(!World) return;
	//~~~~~~~~~~~
	
	NamesOfLoadedLevels.Empty();
	
	//Get Level from Name!
	ULevel* FoundLevel = NULL;
	
	for(const ULevelStreaming* EachLevel : World->StreamingLevels)
	{
		if( ! EachLevel) continue;
		//~~~~~~~~~~~~~~~~
	
		ULevel* LevelPtr = EachLevel->GetLoadedLevel();
		
		//Valid?
		if(!LevelPtr) continue;
		
		//Is This Level Visible?
		if(!LevelPtr->bIsVisible) continue;
		//~~~~~~~~~~~~~~~~~~~
		 
		NamesOfLoadedLevels.Add(EachLevel->GetWorldAssetPackageFName().ToString());
	}
}
		
	
void UVictoryBPFunctionLibrary::Loops_ResetBPRunawayCounter()
{
	//Reset Runaway loop counter (use carefully)
	GInitRunaway();
}

void UVictoryBPFunctionLibrary::GraphicsSettings__SetFrameRateToBeUnbound()
{
	if(!GEngine) return;
	//~~~~~~~~~
	
	GEngine->bSmoothFrameRate = 0; 
}
void UVictoryBPFunctionLibrary::GraphicsSettings__SetFrameRateCap(float NewValue)
{
	if(!GEngine) return;
	//~~~~~~~~~
	   
	GEngine->bSmoothFrameRate = 1; 
	GEngine->SmoothedFrameRateRange = FFloatRange(10,NewValue);
}

FVector2D UVictoryBPFunctionLibrary::ProjectWorldToScreenPosition(const FVector& WorldLocation)
{
	TObjectIterator<APlayerController> ThePC;
	if(!ThePC) return FVector2D::ZeroVector;
	
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(ThePC->Player);
	if (LocalPlayer != NULL && LocalPlayer->ViewportClient != NULL && LocalPlayer->ViewportClient->Viewport != NULL)
	{
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		
		// Create a view family for the game viewport
		FSceneViewFamilyContext ViewFamily( FSceneViewFamily::ConstructionValues(
			LocalPlayer->ViewportClient->Viewport,
			ThePC->GetWorld()->Scene,
			LocalPlayer->ViewportClient->EngineShowFlags )
			.SetRealtimeUpdate(true) );

		// Calculate a view where the player is to update the streaming from the players start location
		FVector ViewLocation;
		FRotator ViewRotation;
		FSceneView* SceneView = LocalPlayer->CalcSceneView( &ViewFamily, /*out*/ ViewLocation, /*out*/ ViewRotation, LocalPlayer->ViewportClient->Viewport );

		//Valid Scene View?
		if (SceneView) 
		{
			//Return
			FVector2D ScreenLocation;
			SceneView->WorldToPixel(WorldLocation,ScreenLocation );
			return ScreenLocation;
		}
	} 
	
	return FVector2D::ZeroVector;
}



bool UVictoryBPFunctionLibrary::GetStaticMeshVertexLocations(UStaticMeshComponent* Comp, TArray<FVector>& VertexPositions)
{
	if(!Comp) return false;
	if(!Comp->IsValidLowLevel()) return false;
	
	//~~~~~~~~~~~~~~~~~~~~
	//				Vertex Buffer
	if(! Comp) 								return false;
	if(! Comp->StaticMesh) 					return false;
	if(! Comp->StaticMesh->RenderData) 	return false;
	if( Comp->StaticMesh->RenderData->LODResources.Num() < 1) return false;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
	//~~~~~~~~~~~~
	VertexPositions.Empty();
	//~~~~~~~~~~~~
	
	FPositionVertexBuffer* VertexBuffer = NULL;
	VertexBuffer = & Comp->StaticMesh->RenderData->LODResources[0].PositionVertexBuffer;
	if(!VertexBuffer) return false;
	//~~~~~~~~~~~~~~~~
	
	int32 VertexCount = VertexBuffer->GetNumVertices();
	 
	FTransform RV_Transform = Comp->GetComponentTransform(); 
	for(int32 Itr = 0; Itr < VertexCount; Itr++)
	{
		VertexPositions.Add(
			Comp->GetComponentLocation() + RV_Transform.TransformVector(VertexBuffer->VertexPosition(Itr))
		);
	}
	
	return true;
}


void UVictoryBPFunctionLibrary::AddToActorRotation(AActor* TheActor, FRotator AddRot)
{
	if (!TheActor) return;
	//~~~~~~~~~~~

	FTransform TheTransform = TheActor->GetTransform();
	TheTransform.ConcatenateRotation(AddRot.Quaternion());
	TheTransform.NormalizeRotation();
	TheActor->SetActorTransform(TheTransform);
}




void UVictoryBPFunctionLibrary::DrawCircle(
	UObject* WorldContextObject,
	FVector Center, 
	float Radius, 
	int32 NumPoints,
	float Thickness,
	FLinearColor LineColor,
	FVector YAxis,
	FVector ZAxis,
	float Duration,
	bool PersistentLines
){ 
	
	
	if(!WorldContextObject) return ;
	
	//using a context object to get the world!
    UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject);
	if(!World) return;
	//~~~~~~~~~~~
	 
	/* //FOR PULL REQUEST TO EPIC
	FMatrix TM;
	TM.SetOrigin(Center);
	TM.SetAxis(0, FVector(1,0,0));
	TM.SetAxis(1, YAxis);
	TM.SetAxis(2, ZAxis);
	
	DrawDebugCircle(
		World, 
		TM, 
		Radius, NumPoints, 
		FColor::Red, 
		false, 
		-1.f, 
		0
	);
*/
	 
	// Need at least 4 segments  
	NumPoints = FMath::Max(NumPoints, 4);
	const float AngleStep = 2.f * PI / float(NumPoints);

	float Angle = 0.f;
	for(int32 v = 0; v < NumPoints; v++)
	{  
		const FVector Vertex1 = Center + Radius * (YAxis * FMath::Cos(Angle) + ZAxis * FMath::Sin(Angle));
		Angle += AngleStep;
		const FVector Vertex2 = Center + Radius * (YAxis * FMath::Cos(Angle) + ZAxis * FMath::Sin(Angle));
		   
		DrawDebugLine(
			World, 
			Vertex1, 
			Vertex2,
			LineColor,  
			PersistentLines, 
			Duration,
			0, 				//depth  
			Thickness          
		);
	}
}


bool UVictoryBPFunctionLibrary::LoadStringArrayFromFile(TArray<FString>& StringArray, int32& ArraySize, FString FullFilePath, bool ExcludeEmptyLines)
{
	ArraySize = 0;
	
	if(FullFilePath == "" || FullFilePath == " ") return false;
	
	//Empty any previous contents!
	StringArray.Empty();
	
	TArray<FString> FileArray;
	
	if( ! FFileHelper::LoadANSITextFileToStrings(*FullFilePath, NULL, FileArray))
	{
		return false;
	}
	
	if(ExcludeEmptyLines)
	{
		for(const FString& Each : FileArray )
		{
			if(Each == "") continue;
			//~~~~~~~~~~~~~
			
			//check for any non whitespace
			bool FoundNonWhiteSpace = false;
			for(int32 v = 0; v < Each.Len(); v++)
			{
				if(Each[v] != ' ' && Each[v] != '\n')
				{
					FoundNonWhiteSpace = true;
					break;
				}
				//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			}
			
			if(FoundNonWhiteSpace)
			{
				StringArray.Add(Each);
			}
		}
	}
	else
	{
		StringArray.Append(FileArray);
	}
	
	ArraySize = StringArray.Num();
	return true; 
}


void UVictoryBPFunctionLibrary::Selection_SelectionBox(UObject* WorldContextObject,TArray<AActor*>& SelectedActors, FVector2D AnchorPoint,FVector2D DraggedPoint,TSubclassOf<AActor> ClassFilter)
{
	if(!WorldContextObject) return ;
	
	
	//using a context object to get the world!
    UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject);
	if(!World) return;
	//~~~~~~~~~~~
	
	SelectedActors.Empty();
	
	FBox2D Box;
	Box+=DraggedPoint;
	Box+=AnchorPoint;
	
	for(TActorIterator<AActor> Itr(World); Itr; ++Itr)
	{
		if(!Itr->IsA(ClassFilter)) continue;
		//~~~~~~~~~~~~~~~~~~
		
		if(Box.IsInside(UVictoryBPFunctionLibrary::ProjectWorldToScreenPosition(Itr->GetActorLocation())))
		{
			SelectedActors.Add(*Itr);
		}
	}
}

bool UVictoryBPFunctionLibrary::PlayerController_GetControllerID(APlayerController* ThePC, int32& ControllerID)
{
	if(!ThePC) return false;
	
	ULocalPlayer * LP = Cast<ULocalPlayer>(ThePC->Player);
    if(!LP) return false;
 
    ControllerID = LP->ControllerId;
	
	return true;
}
	
bool UVictoryBPFunctionLibrary::PlayerState_GetPlayerID(APlayerController* ThePC, int32& PlayerID)
{
	if(!ThePC) return false;
	
	if(!ThePC->PlayerState) return false;
	
	PlayerID = ThePC->PlayerState->PlayerId;
	
	return true;
}

void UVictoryBPFunctionLibrary::Open_URL_In_Web_Browser(FString TheURL)
{
	FPlatformProcess::LaunchURL( *TheURL, nullptr, nullptr );
}

void UVictoryBPFunctionLibrary::OperatingSystem__GetCurrentPlatform(
	bool& Windows_, 		//some weird bug of making it all caps engine-side
	bool& Mac,
	bool& Linux, 
	bool& iOS, 
	bool& Android,
	bool& PS4,
	bool& XBoxOne,
	bool& HTML5,
	bool& WinRT_Arm,
	bool& WinRT
){
	//#define's in UE4 source code
	Windows_ = 				PLATFORM_WINDOWS;
	Mac = 						PLATFORM_MAC;
	Linux = 					PLATFORM_LINUX;
	
	PS4 = 						PLATFORM_PS4;
	XBoxOne = 				PLATFORM_XBOXONE;
	
	iOS = 						PLATFORM_IOS;
	Android = 				PLATFORM_ANDROID;
	
	HTML5 = 					PLATFORM_HTML5;
	
	WinRT_Arm =	 			PLATFORM_WINRT_ARM;
	WinRT 	= 				PLATFORM_WINRT;
}

FString UVictoryBPFunctionLibrary::RealWorldTime__GetCurrentOSTime( 
	int32& MilliSeconds,
	int32& Seconds, 
	int32& Minutes, 
	int32& Hours12,
	int32& Hours24,
	int32& Day,
	int32& Month,
	int32& Year
){
	const FDateTime Now = FDateTime::Now();
	
	MilliSeconds = 		Now.GetMillisecond( );
	Seconds = 			Now.GetSecond( );
	Minutes = 				Now.GetMinute( );
	Hours12 = 			Now.GetHour12( );
	Hours24 = 			Now.GetHour( ); //24
	Day = 					Now.GetDay( );
	Month = 				Now.GetMonth( );
	Year = 				Now.GetYear( );
	
	return Now.ToString();
}

void UVictoryBPFunctionLibrary::RealWorldTime__GetTimePassedSincePreviousTime(
		const FString& PreviousTime,
		float& Milliseconds,
		float& Seconds,
		float& Minutes,
		float& Hours
){
	FDateTime ParsedDateTime;
	FDateTime::Parse(PreviousTime,ParsedDateTime);
	const FTimespan TimeDiff = FDateTime::Now() - ParsedDateTime;
	
	Milliseconds 	= TimeDiff.GetTotalMilliseconds( );
	Seconds 		= TimeDiff.GetTotalSeconds( );
	Minutes 		= TimeDiff.GetTotalMinutes( );
	Hours 			= TimeDiff.GetTotalHours( );
}
	
void UVictoryBPFunctionLibrary::RealWorldTime__GetDifferenceBetweenTimes(
		const FString& PreviousTime1,
		const FString& PreviousTime2,
		float& Milliseconds,
		float& Seconds,
		float& Minutes,
		float& Hours
){
	FDateTime ParsedDateTime1;
	FDateTime::Parse(PreviousTime1,ParsedDateTime1);
	
	FDateTime ParsedDateTime2;
	FDateTime::Parse(PreviousTime2,ParsedDateTime2);
	
	FTimespan TimeDiff; 
	
	if(PreviousTime1 < PreviousTime2)
	{
		TimeDiff = ParsedDateTime2 - ParsedDateTime1;
	}
	else
	{
		TimeDiff = ParsedDateTime1 - ParsedDateTime2;
	}
	
	Milliseconds 	= TimeDiff.GetTotalMilliseconds( );
	Seconds 		= TimeDiff.GetTotalSeconds( );
	Minutes 		= TimeDiff.GetTotalMinutes( );
	Hours 			= TimeDiff.GetTotalHours( );
}



void UVictoryBPFunctionLibrary::MaxOfFloatArray(const TArray<float>& FloatArray, int32& IndexOfMaxValue, float& MaxValue)
{
	MaxValue = UVictoryBPFunctionLibrary::Max(FloatArray,&IndexOfMaxValue);
}

void UVictoryBPFunctionLibrary::MaxOfIntArray(const TArray<int32>& IntArray, int32& IndexOfMaxValue, int32& MaxValue)
{
	MaxValue = UVictoryBPFunctionLibrary::Max(IntArray,&IndexOfMaxValue);
}

void UVictoryBPFunctionLibrary::MinOfFloatArray(const TArray<float>& FloatArray, int32& IndexOfMinValue, float& MinValue)
{
	MinValue = UVictoryBPFunctionLibrary::Min(FloatArray,&IndexOfMinValue);
}

void UVictoryBPFunctionLibrary::MinOfIntArray(const TArray<int32>& IntArray, int32& IndexOfMinValue, int32& MinValue)
{
	MinValue = UVictoryBPFunctionLibrary::Min(IntArray,&IndexOfMinValue);
}



bool UVictoryBPFunctionLibrary::CharacterMovement__SetMaxMoveSpeed(ACharacter* TheCharacter, float NewMaxMoveSpeed)
{
	if(!TheCharacter)
	{
		return false;
	}
	if(!TheCharacter->CharacterMovement)
	{
		return false;
	}
	
	TheCharacter->CharacterMovement->MaxWalkSpeed = NewMaxMoveSpeed;
	
	return true;
}
	


int32 UVictoryBPFunctionLibrary::Conversion__FloatToRoundedInteger(float IN_Float)
{
	return FGenericPlatformMath::RoundToInt(IN_Float);
}

FString UVictoryBPFunctionLibrary::String__CombineStrings(FString StringFirst, FString StringSecond, FString Separator, FString StringFirstLabel, FString StringSecondLabel)
{
	return StringFirstLabel + StringFirst + Separator + StringSecondLabel + StringSecond;
}

bool UVictoryBPFunctionLibrary::OptionsMenu__GetDisplayAdapterScreenResolutions(TArray<int32>& Widths, TArray<int32>& Heights, TArray<int32>& RefreshRates,bool IncludeRefreshRates)
{
	//Clear any Previous
	Widths.Empty();
	Heights.Empty();
	RefreshRates.Empty();
	
	TArray<FString> Unique;	
	
	FScreenResolutionArray Resolutions;
	if(RHIGetAvailableResolutions(Resolutions, false))
	{
		for (const FScreenResolutionRHI& EachResolution : Resolutions)
		{
			FString Str = "";
			Str += FString::FromInt(EachResolution.Width);
			Str += FString::FromInt(EachResolution.Height);
			
			//Include Refresh Rates?
			if(IncludeRefreshRates)
			{
				Str += FString::FromInt(EachResolution.RefreshRate);
			}		
			
			if(Unique.Contains(Str))
			{
				//Skip! This is duplicate!
				continue;
			}
			else
			{
				//Add to Unique List!
				Unique.AddUnique(Str);
			}
			
			//Add to Actual Data Output!
			Widths.Add(			EachResolution.Width);
			Heights.Add(			EachResolution.Height);
			RefreshRates.Add(	EachResolution.RefreshRate);
		}

		return true;
	}
	return false;
}

AStaticMeshActor* UVictoryBPFunctionLibrary::Clone__StaticMeshActor(UObject* WorldContextObject, bool&IsValid, AStaticMeshActor* ToClone, FVector LocationOffset,FRotator RotationOffset)
{
	IsValid = NULL;
	if(!ToClone) return NULL;
	if(!ToClone->IsValidLowLevel()) return NULL;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
	if(!WorldContextObject) return NULL;
	
	//using a context object to get the world!
    UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject);
	if(!World) return NULL;
	//~~~~~~~~~~~
	
	//For BPS
	UClass* SpawnClass = ToClone->GetClass();
	
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.bNoCollisionFail 		= true;
	SpawnInfo.Owner 				= ToClone;
	SpawnInfo.Instigator				= NULL;
	SpawnInfo.bDeferConstruction 	= NULL;
	
	AStaticMeshActor* NewSMA = World->SpawnActor<AStaticMeshActor>(SpawnClass, ToClone->GetActorLocation() + FVector(0,0,512) ,ToClone->GetActorRotation(), SpawnInfo );
	
	if(!NewSMA) return NULL;
	
	//Copy Transform
	NewSMA->SetActorTransform(ToClone->GetTransform());
	
	//Mobility
	NewSMA->StaticMeshComponent->SetMobility(EComponentMobility::Movable	);
	
	//copy static mesh
	NewSMA->StaticMeshComponent->SetStaticMesh(ToClone->StaticMeshComponent->StaticMesh);
	
	//~~~
	
	//copy materials
	TArray<UMaterialInterface*> Mats;
	ToClone->StaticMeshComponent->GetUsedMaterials(Mats);
	
	const int32 Total = Mats.Num();
	for(int32 v = 0; v < Total; v++ )
	{
		NewSMA->StaticMeshComponent->SetMaterial(v,Mats[v]);
	}
	
	//~~~
	
	//copy physics state
	if(ToClone->StaticMeshComponent->IsSimulatingPhysics())
	{
		NewSMA->StaticMeshComponent->SetSimulatePhysics(true);
	}
	
	//~~~
	
	//Add Location Offset
	const FVector SpawnLoc = ToClone->GetActorLocation() + LocationOffset;
	NewSMA->SetActorLocation(SpawnLoc);
	
	//Add Rotation offset
	FTransform TheTransform = NewSMA->GetTransform();
	TheTransform.ConcatenateRotation(RotationOffset.Quaternion());
	TheTransform.NormalizeRotation();
	
	//Set Transform
	NewSMA->SetActorTransform(TheTransform);
	
	IsValid = true;
	return NewSMA;
}

bool UVictoryBPFunctionLibrary::Actor__TeleportToActor(AActor* ActorToTeleport, AActor* DestinationActor)
{
	if(!ActorToTeleport) 							return false;
	if(!ActorToTeleport->IsValidLowLevel()) 	return false;
	if(!DestinationActor) 							return false;
	if(!DestinationActor->IsValidLowLevel()) 	return false;
	
	//Set Loc
	ActorToTeleport->SetActorLocation(DestinationActor->GetActorLocation());
	
	//Set Rot
	ActorToTeleport->SetActorRotation(DestinationActor->GetActorRotation());
	
	return true;
}

bool UVictoryBPFunctionLibrary::WorldType__InEditorWorld(UObject* WorldContextObject)
{
	if(!WorldContextObject) return false;
	
	//using a context object to get the world!
    UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject);
	if(!World) return false;
	//~~~~~~~~~~~
	
    return (World->WorldType == EWorldType::Editor );
}

bool UVictoryBPFunctionLibrary::WorldType__InPIEWorld(UObject* WorldContextObject)
{
	if(!WorldContextObject) return false;
	
	//using a context object to get the world!
    UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject);
	if(!World) return false;
	//~~~~~~~~~~~
	
    return (World->WorldType == EWorldType::PIE );
}
bool UVictoryBPFunctionLibrary::WorldType__InGameInstanceWorld(UObject* WorldContextObject)
{
	if(!WorldContextObject) return false;
	
	//using a context object to get the world!
    UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject);
	if(!World) return false;
	//~~~~~~~~~~~
	
    return (World->WorldType == EWorldType::Game );
}
	
FString UVictoryBPFunctionLibrary::Accessor__GetNameAsString(const UObject* TheObject)
{
	if (!TheObject) return "";
	return TheObject->GetName();
}

FRotator UVictoryBPFunctionLibrary::Conversions__VectorToRotator(const FVector& TheVector)
{
	return TheVector.Rotation();
}
FVector UVictoryBPFunctionLibrary::Conversions__RotatorToVector(const FRotator& TheRotator)
{
	return TheRotator.Vector();
}
FRotator UVictoryBPFunctionLibrary::Character__GetControllerRotation(AActor * TheCharacter)
{
	ACharacter * AsCharacter = Cast<ACharacter>(TheCharacter);

	if (!AsCharacter) return FRotator::ZeroRotator;
	
	return AsCharacter->GetControlRotation();
}

//Draw From Socket on Character's Mesh
void UVictoryBPFunctionLibrary::Draw__Thick3DLineFromCharacterSocket(AActor* TheCharacter,  const FVector& EndPoint, FName Socket, FLinearColor LineColor, float Thickness, float Duration)
{
	ACharacter * AsCharacter = Cast<ACharacter>(TheCharacter);
	if (!AsCharacter) return;
	if (!AsCharacter->Mesh) return;
	//~~~~~~~~~~~~~~~~~~~~
	
	//Get World
	UWorld* TheWorld = AsCharacter->GetWorld();
	if (!TheWorld) return;
	//~~~~~~~~~~~~~~~~~
	
	const FVector SocketLocation = AsCharacter->Mesh->GetSocketLocation(Socket);
	DrawDebugLine(
		TheWorld, 
		SocketLocation, 
		EndPoint, 
		FColor(LineColor), 
		false, 
		Duration, 
		0, 
		Thickness
	);
	
}
/** Draw 3D Line of Chosen Thickness From Mesh Socket to Destination */
void UVictoryBPFunctionLibrary::Draw__Thick3DLineFromSocket(USkeletalMeshComponent* Mesh, const FVector& EndPoint, FName Socket, FLinearColor LineColor, float Thickness, float Duration)
{
	if (!Mesh) return;
	//~~~~~~~~~~~~~~
	
	//Get an actor to GetWorld() from
	TObjectIterator<APlayerController> Itr;
	if (!Itr) return;
	//~~~~~~~~~~~~
	
	//Get World
	UWorld* TheWorld = Itr->GetWorld();
	if (!TheWorld) return;
	//~~~~~~~~~~~~~~~~~
	
	const FVector SocketLocation = Mesh->GetSocketLocation(Socket);
	
	DrawDebugLine(
		TheWorld, 
		SocketLocation, 
		EndPoint, 
		FColor(LineColor), 
		false, 
		Duration, 
		0, 
		Thickness
	);
}
/** Draw 3D Line of Chosen Thickness Between Two Actors */
void UVictoryBPFunctionLibrary::Draw__Thick3DLineBetweenActors(AActor * StartActor, AActor * EndActor, FLinearColor LineColor, float Thickness, float Duration)
{
	if (!StartActor) return;
	if (!EndActor) return;
	//~~~~~~~~~~~~~~~~
	
	DrawDebugLine(
		StartActor->GetWorld(), 
		StartActor->GetActorLocation(), 
		EndActor->GetActorLocation(), 
		FColor(LineColor), 
		false, 
		Duration, 
		0, 
		Thickness
	);
}
	
bool UVictoryBPFunctionLibrary::Animation__GetAimOffsets(AActor* AnimBPOwner, float& Pitch, float& Yaw)
{
	//Get Owning Character
	ACharacter * TheCharacter = Cast<ACharacter>(AnimBPOwner);
	
	if (!TheCharacter) return false;
	//~~~~~~~~~~~~~~~
	
	//Get Owning Controller Rotation
	const FRotator TheCtrlRotation = TheCharacter->GetControlRotation();
	
	const FVector RotationDir = TheCtrlRotation.Vector();
	
	//Inverse of ActorToWorld matrix is Actor to Local Space
		//so this takes the direction vector, the PC or AI rotation
		//and outputs what this dir is 
		//in local actor space &
		
		//local actor space is what we want for aiming offsets
		
	const FVector LocalDir = TheCharacter->ActorToWorld().InverseTransformVectorNoScale(RotationDir);
	const FRotator LocalRotation = LocalDir.Rotation();
		
	//Pass out Yaw and Pitch
	Yaw = LocalRotation.Yaw;
	Pitch = LocalRotation.Pitch;
	
	return true;
}
bool UVictoryBPFunctionLibrary::Animation__GetAimOffsetsFromRotation(AActor * AnimBPOwner, const FRotator & TheRotation, float & Pitch, float & Yaw)
{
	//Get Owning Character
	ACharacter * TheCharacter = Cast<ACharacter>(AnimBPOwner);
	
	if (!TheCharacter) return false;
	//~~~~~~~~~~~~~~~
	
	//using supplied rotation
	const FVector RotationDir = TheRotation.Vector();
	
	//Inverse of ActorToWorld matrix is Actor to Local Space
		//so this takes the direction vector, the PC or AI rotation
		//and outputs what this dir is 
		//in local actor space &
		
		//local actor space is what we want for aiming offsets
		
	const FVector LocalDir = TheCharacter->ActorToWorld().InverseTransformVectorNoScale(RotationDir);
	const FRotator LocalRotation = LocalDir.Rotation();
		
	//Pass out Yaw and Pitch
	Yaw = LocalRotation.Yaw;
	Pitch = LocalRotation.Pitch;
	
	return true;
}

void UVictoryBPFunctionLibrary::Visibility__GetRenderedActors(TArray<AActor*>& CurrentlyRenderedActors, float MinRecentTime)
{
	//Empty any previous entries
	CurrentlyRenderedActors.Empty();
	
	//Iterate Over Actors
	for ( TObjectIterator<AActor> Itr; Itr; ++Itr )
	{
		if (Itr->GetLastRenderTime() > MinRecentTime)
		{
			CurrentlyRenderedActors.Add( * Itr);
		}
	}
}
void UVictoryBPFunctionLibrary::Visibility__GetNotRenderedActors(TArray<AActor*>& CurrentlyNotRenderedActors, float MinRecentTime)
{
	//Empty any previous entries
	CurrentlyNotRenderedActors.Empty();
	
	//Iterate Over Actors
	for ( TObjectIterator<AActor> Itr; Itr; ++Itr )
	{
		if (Itr->GetLastRenderTime() <= MinRecentTime)
		{
			CurrentlyNotRenderedActors.Add( * Itr);
		}
	}
}

void UVictoryBPFunctionLibrary::Rendering__FreezeGameRendering()
{
	FViewport::SetGameRenderingEnabled(false);
}
void UVictoryBPFunctionLibrary::Rendering__UnFreezeGameRendering()
{
	FViewport::SetGameRenderingEnabled(true);
}
	
bool UVictoryBPFunctionLibrary::ClientWindow__GameWindowIsForeGroundInOS()
{
	//Iterate Over Actors
	UWorld* TheWorld = NULL;
	for ( TObjectIterator<AActor> Itr; Itr; ++Itr )
	{
		TheWorld = Itr->GetWorld();
		if (TheWorld) break;
		//~~~~~~~~~~~~~~~~~~~~~~~
	}
	//Get Player
	ULocalPlayer* VictoryPlayer = 
            TheWorld->GetFirstLocalPlayerFromController(); 

	if (!VictoryPlayer) return false;
	//~~~~~~~~~~~~~~~~~~~~
	
	//get view port ptr
	UGameViewportClient * VictoryViewportClient = 
		Cast < UGameViewportClient > (VictoryPlayer->ViewportClient);
		
	if (!VictoryViewportClient) return false;
	//~~~~~~~~~~~~~~~~~~~~
	 
	FViewport * VictoryViewport = VictoryViewportClient->Viewport;
	
	if (!VictoryViewport) return false;
	//~~~~~~~~~~~~~~~~~~~~
	
    return VictoryViewport->IsForegroundWindow();
}
bool UVictoryBPFunctionLibrary::FileIO__SaveStringTextToFile(
	FString SaveDirectory, 
	FString JoyfulFileName, 
	FString SaveText,
	bool AllowOverWriting
){
	//Dir Exists?
	if ( !FPlatformFileManager::Get().GetPlatformFile().DirectoryExists( *SaveDirectory))
	{
		//create directory if it not exist
		FPlatformFileManager::Get().GetPlatformFile().CreateDirectory( *SaveDirectory);
		
		//still could not make directory?
		if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists( *SaveDirectory))
		{
			//Could not make the specified directory
			return false;
			//~~~~~~~~~~~~~~~~~~~~~~
		}
	}
	
	//get complete file path
	SaveDirectory += "\\";
	SaveDirectory += JoyfulFileName;
	
	//No over-writing?
	if (!AllowOverWriting)
	{
		//Check if file exists already
		if (FPlatformFileManager::Get().GetPlatformFile().FileExists( * SaveDirectory))
		{
			//no overwriting
			return false;
		}
	}
	
	return FFileHelper::SaveStringToFile(SaveText, * SaveDirectory);
}

float UVictoryBPFunctionLibrary::Calcs__ClosestPointToSourcePoint(const FVector & Source, const TArray<FVector>& OtherPoints, FVector& ClosestPoint)
{
	float CurDist = 0;
	float ClosestDistance = -1;
	int32 ClosestVibe = 0;
	ClosestPoint = FVector::ZeroVector;
	
	if (OtherPoints.Num() <= 0) return ClosestDistance;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
	for (int32 Itr = 0; Itr < OtherPoints.Num(); Itr++)
	{
		if (Source == OtherPoints[Itr]) continue;
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		
		//Dist
		CurDist = FVector::Dist(Source, OtherPoints[Itr]);
		
		//Min
		if (ClosestDistance < 0 || ClosestDistance >= CurDist)
		{
			ClosestVibe = Itr;
			ClosestDistance = CurDist;
		}
	}
	
	//Out
	ClosestPoint = OtherPoints[ClosestVibe];
	return ClosestDistance;
}

bool UVictoryBPFunctionLibrary::Data__GetCharacterBoneLocations(AActor * TheCharacter, TArray<FVector>& BoneLocations)
{
	ACharacter * Source = Cast<ACharacter>(TheCharacter);
	if (!Source) return false;
	
	if (!Source-> Mesh) return false;
	//~~~~~~~~~~~~~~~~~~~~~~~~~
	TArray<FName> BoneNames;
	
	BoneLocations.Empty();
	
	
	//Get Bone Names
	Source-> Mesh->GetBoneNames(BoneNames);
	
	//Get Bone Locations
	for (int32 Itr = 0; Itr < BoneNames.Num(); Itr++ )
	{
		BoneLocations.Add(Source->Mesh->GetBoneLocation(BoneNames[Itr]));
	}
	
	return true;
}

USkeletalMeshComponent* UVictoryBPFunctionLibrary::Accessor__GetCharacterSkeletalMesh(AActor * TheCharacter, bool& IsValid)
{
	IsValid = false;
	
	ACharacter * AsCharacter = Cast<ACharacter>(TheCharacter);
	if (!AsCharacter) return NULL;
	//~~~~~~~~~~~~~~~~~
	
	//Is Valid?
	if (AsCharacter->Mesh)
		if (AsCharacter->Mesh->IsValidLowLevel() ) 
			IsValid = true;
			
	return AsCharacter->Mesh;
}

bool UVictoryBPFunctionLibrary::TraceData__GetTraceDataFromCharacterSocket(
	FVector & TraceStart, //out
	FVector & TraceEnd,	//out
	AActor * TheCharacter,
	const FRotator& TraceRotation, 
	float TraceLength,
	FName Socket, 
	bool DrawTraceData, 
	FLinearColor TraceDataColor, 
	float TraceDataThickness
) {
	ACharacter * AsCharacter = Cast<ACharacter>(TheCharacter);
	if (!AsCharacter) return false;
	
	//Mesh Exists?
	if (!AsCharacter->Mesh) return false;
	
	//Socket Exists?
	if (!AsCharacter->Mesh->DoesSocketExist(Socket)) return false;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
	TraceStart = AsCharacter->Mesh->GetSocketLocation(Socket);
	TraceEnd = TraceStart + TraceRotation.Vector() * TraceLength;
	
	if (DrawTraceData) 
	{
		//Get World
		UWorld* TheWorld = AsCharacter->GetWorld();
		if (!TheWorld) return false;
		//~~~~~~~~~~~~~~~~~
	
		DrawDebugLine(
			TheWorld, 
			TraceStart, 
			TraceEnd, 
			FColor(TraceDataColor), 
			false, 
			0.0333, 
			0, 
			TraceDataThickness
		);
	}
	
	return true;
}
bool UVictoryBPFunctionLibrary::TraceData__GetTraceDataFromSkeletalMeshSocket(
	FVector & TraceStart, //out
	FVector & TraceEnd,	//out
	USkeletalMeshComponent * Mesh,
	const FRotator & TraceRotation,	
	float TraceLength,
	FName Socket, 
	bool DrawTraceData, 
	FLinearColor TraceDataColor, 
	float TraceDataThickness
) {
	//Mesh Exists?
	if (!Mesh) return false;
	
	//Socket Exists?
	if (!Mesh->DoesSocketExist(Socket)) return false;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	TraceStart = Mesh->GetSocketLocation(Socket);
	TraceEnd = TraceStart + TraceRotation.Vector() * TraceLength;
	
	if (DrawTraceData) 
	{
		//Get a PC to GetWorld() from
		TObjectIterator<APlayerController> Itr;
		if (!Itr) return false;
		
		//~~~~~~~~~~~~
		
		//Get World
		UWorld* TheWorld = Itr->GetWorld();
		if (!TheWorld) return false;
		//~~~~~~~~~~~~~~~~~
	
		DrawDebugLine(
			TheWorld, 
			TraceStart, 
			TraceEnd, 
			FColor(TraceDataColor), 
			false, 
			0.0333, 
			0, 
			TraceDataThickness
		);
	}
	
	return true;
}
AActor*  UVictoryBPFunctionLibrary::Traces__CharacterMeshTrace___ClosestBone(
	AActor* TraceOwner,
	const FVector & TraceStart, 
	const FVector & TraceEnd, 
	FVector & OutImpactPoint, 
	FVector & OutImpactNormal, 
	FName & ClosestBoneName,
	FVector & ClosestBoneLocation,
	bool& IsValid
)
{
	IsValid = false;
	AActor * HitActor = NULL;
	//~~~~~~~~~~~~~~~~~~~~~~
	
	//Get a PC to GetWorld() from
	TObjectIterator<APlayerController> Itr;
	if (!Itr) return NULL;
	
	//~~~~~~~~~~~~
	
	//Get World
	UWorld* TheWorld = Itr->GetWorld();
	if (TheWorld == nullptr) return NULL;
	//~~~~~~~~~~~~~~~~~
	
	
	//Simple Trace First
	FCollisionQueryParams TraceParams(FName(TEXT("VictoryBPTrace::CharacterMeshTrace")), true, HitActor);
	TraceParams.bTraceComplex = true;
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.AddIgnoredActor(TraceOwner);
	
	//initialize hit info
	FHitResult RV_Hit(ForceInit);
	
	TheWorld->LineTraceSingle(
		RV_Hit,		//result
		TraceStart, 
		TraceEnd, 
		ECC_Pawn, //collision channel
		TraceParams
	);
		
	//Hit Something!
	if (!RV_Hit.bBlockingHit) return HitActor;
	
	
	//Character?
	HitActor = RV_Hit.GetActor();
	ACharacter * AsCharacter = Cast<ACharacter>(HitActor);
	if (!AsCharacter) return HitActor;
	
	//Mesh
	if (!AsCharacter->Mesh) return HitActor;
	
	//Component Trace
	IsValid = AsCharacter->Mesh->K2_LineTraceComponent(
		TraceStart, 
		TraceEnd, 
		true, 
		false, 
		OutImpactPoint, 
		OutImpactNormal,
		ClosestBoneName
	); 
	
	//Location
	ClosestBoneLocation = AsCharacter->Mesh->GetBoneLocation(ClosestBoneName);
	
	return HitActor;
}

AActor* UVictoryBPFunctionLibrary::Traces__CharacterMeshTrace___ClosestSocket(
	const AActor * TraceOwner, 
	const FVector & TraceStart, 
	const FVector & TraceEnd, 
	FVector & OutImpactPoint, 
	FVector & OutImpactNormal, 
	FName & ClosestSocketName, 
	FVector & SocketLocation, 
	bool & IsValid
)
{
	IsValid = false;
	AActor * HitActor = NULL;
	//~~~~~~~~~~~~~~~~~~~~~~
	 
	//There may not be a trace owner so dont rely on it
	
	//Get a PC to GetWorld() from
	TObjectIterator<APlayerController> Itr;
	if (!Itr) return NULL;
	
	//~~~~~~~~~~~~
	
	//Get World
	UWorld* TheWorld = Itr->GetWorld();
	if (TheWorld == nullptr) return NULL;
	//~~~~~~~~~~~~~~~~~
	
	
	//Simple Trace First
	FCollisionQueryParams TraceParams(FName(TEXT("VictoryBPTrace::CharacterMeshSocketTrace")), true, HitActor);
	TraceParams.bTraceComplex = true;
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.AddIgnoredActor(TraceOwner);
	
	//initialize hit info
	FHitResult RV_Hit(ForceInit);
	
	TheWorld->LineTraceSingle(
		RV_Hit,		//result
		TraceStart, 
		TraceEnd, 
		ECC_Pawn, //collision channel
		TraceParams
	);
		
	//Hit Something!
	if (!RV_Hit.bBlockingHit) return HitActor;
	
	
	//Character?
	HitActor = RV_Hit.GetActor();
	ACharacter * AsCharacter = Cast<ACharacter>(HitActor);
	if (!AsCharacter) return HitActor;
	
	//Mesh
	if (!AsCharacter->Mesh) return HitActor;
	
	//Component Trace
	FName BoneName;
	if (! AsCharacter->Mesh->K2_LineTraceComponent(
		TraceStart, 
		TraceEnd, 
		true, 
		false, 
		OutImpactPoint, 
		OutImpactNormal,
		BoneName
	)) return HitActor;
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//					Socket Names
	TArray<FComponentSocketDescription> SocketNames;
	
	//Get Bone Names
	AsCharacter->Mesh->QuerySupportedSockets(SocketNames);
	
	//						Min
	FVector CurLoc;
	float CurDist = 0;
	float ClosestDistance = -1;
	int32 ClosestVibe = 0;
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//Check All Bones Locations
	for (int32 Itr = 0; Itr < SocketNames.Num(); Itr++ )
	{
		//Is this a Bone not a socket?
		if(SocketNames[Itr].Type == EComponentSocketType::Bone) continue;
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		
		CurLoc = AsCharacter->Mesh->GetSocketLocation(SocketNames[Itr].Name);
		
		//Dist
		CurDist = FVector::Dist(OutImpactPoint, CurLoc);
		
		//Min
		if (ClosestDistance < 0 || ClosestDistance >= CurDist)
		{
			ClosestVibe = Itr;
			ClosestDistance = CurDist;
		}
	}
	
	//Name
	ClosestSocketName = SocketNames[ClosestVibe].Name;
	
	//Location
	SocketLocation = AsCharacter->Mesh->GetSocketLocation(ClosestSocketName);
	
	//Valid
	IsValid = true;
	
	//Actor
	return HitActor;
}
	
//Most HUD stuff is in floats so I do the conversion internally
bool UVictoryBPFunctionLibrary::Viewport__SetMousePosition(const APlayerController* ThePC, const float& PosX, const float& PosY)
{
	if (!ThePC) return false;
	//~~~~~~~~~~~~~
	
	//Get Player
	const ULocalPlayer * VictoryPlayer = Cast<ULocalPlayer>(ThePC->Player); 
											//PlayerController::Player is UPlayer
           
	if (!VictoryPlayer) return false;
	//~~~~~~~~~~~~~~~~~~~~
	
	//get view port ptr
	const UGameViewportClient * VictoryViewportClient = 
		Cast < UGameViewportClient > (VictoryPlayer->ViewportClient);
		
	if (!VictoryViewportClient) return false;
	//~~~~~~~~~~~~~~~~~~~~
	 
	FViewport * VictoryViewport = VictoryViewportClient->Viewport;
	
	if (!VictoryViewport) return false;
	//~~~~~~~~~~~~~~~~~~~~
	
	//Set Mouse
	VictoryViewport->SetMouse(int32(PosX), int32(PosY));
	
	return true;
}

APlayerController * UVictoryBPFunctionLibrary::Accessor__GetPlayerController(
	AActor * TheCharacter, 
	bool & IsValid
)
{
	IsValid = false;
	
	//Cast to Character
	ACharacter * AsCharacter = Cast<ACharacter>(TheCharacter);
	if (!AsCharacter) return NULL;
	
	//cast to PC
	APlayerController * ThePC = Cast < APlayerController > (AsCharacter->GetController());
	
	if (!ThePC ) return NULL;
	
	IsValid = true;
	return ThePC;
}
	
bool UVictoryBPFunctionLibrary::Viewport__GetCenterOfViewport(const APlayerController * ThePC, float & PosX, float & PosY)
{
	if (!ThePC) return false;
	//~~~~~~~~~~~~~
	
	//Get Player
	const ULocalPlayer * VictoryPlayer = Cast<ULocalPlayer>(ThePC->Player); 
											//PlayerController::Player is UPlayer
           
	if (!VictoryPlayer) return false;
	//~~~~~~~~~~~~~~~~~~~~
	
	//get view port ptr
	const UGameViewportClient * VictoryViewportClient = 
		Cast < UGameViewportClient > (VictoryPlayer->ViewportClient);
		
	if (!VictoryViewportClient) return false;
	//~~~~~~~~~~~~~~~~~~~~
	 
	FViewport * VictoryViewport = VictoryViewportClient->Viewport;
	
	if (!VictoryViewport) return false;
	//~~~~~~~~~~~~~~~~~~~~
	
	//Get Size
	FIntPoint Size = VictoryViewport->GetSizeXY();
	
	//Center
	PosX = Size.X / 2;
	PosY = Size.Y / 2;
	
	return true;
}

bool UVictoryBPFunctionLibrary::Viewport__GetMousePosition(const APlayerController * ThePC, float & PosX, float & PosY)
{
	if (!ThePC) return false;
	//~~~~~~~~~~~~~
	
	//Get Player
	const ULocalPlayer * VictoryPlayer = Cast<ULocalPlayer>(ThePC->Player); 
											//PlayerController::Player is UPlayer
           
	if (!VictoryPlayer) return false;
	//~~~~~~~~~~~~~~~~~~~~
	
	//get view port ptr
	const UGameViewportClient * VictoryViewportClient = 
		Cast < UGameViewportClient > (VictoryPlayer->ViewportClient);
		
	if (!VictoryViewportClient) return false;
	//~~~~~~~~~~~~~~~~~~~~
	 
	FViewport * VictoryViewport = VictoryViewportClient->Viewport;
	
	if (!VictoryViewport) return false;
	//~~~~~~~~~~~~~~~~~~~~
	
	PosX = float(VictoryViewport->GetMouseX());
	PosY = float(VictoryViewport->GetMouseY());
	
	return true;
}





bool UVictoryBPFunctionLibrary::Physics__EnterRagDoll(AActor * TheCharacter)
{
	ACharacter * AsCharacter = Cast<ACharacter>(TheCharacter);
	if (!AsCharacter) return false;
	
	//Mesh?
	if (!AsCharacter->Mesh) return false;
	
	//Physics Asset?
	if(!AsCharacter->Mesh->GetPhysicsAsset()) return false;
	
	//Victory Ragdoll
	AsCharacter->Mesh->SetPhysicsBlendWeight(1);
	AsCharacter->Mesh->bBlendPhysics = true;
	
	return true;
}


bool UVictoryBPFunctionLibrary::Physics__LeaveRagDoll(
	AActor* TheCharacter,
	float HeightAboveRBMesh,
	const FVector& InitLocation, 
	const FRotator& InitRotation
){
	ACharacter * AsCharacter = Cast<ACharacter>(TheCharacter);
	if (!AsCharacter) return false;
	
	//Mesh?
	if (!AsCharacter->Mesh) return false;
	
	//Set Actor Location to Be Near Ragdolled Mesh
	//Calc Ref Bone Relative Pos for use with IsRagdoll
	TArray<FName> BoneNames;
	AsCharacter->Mesh->GetBoneNames(BoneNames);
	if(BoneNames.Num() > 0)
	{
		AsCharacter->SetActorLocation(FVector(0,0,HeightAboveRBMesh) + AsCharacter->Mesh->GetBoneLocation(BoneNames[0]));
	}
	
	//Exit Ragdoll
	AsCharacter->Mesh->SetPhysicsBlendWeight(0); //1 for full physics

	//Restore Defaults
	AsCharacter->Mesh->SetRelativeRotation(InitRotation);
	AsCharacter->Mesh->SetRelativeLocation(InitLocation);
	
	//Set Falling After Final Capsule Relocation
	if(AsCharacter->CharacterMovement) AsCharacter->CharacterMovement->SetMovementMode(MOVE_Falling);	
	
	return true;
}	

bool UVictoryBPFunctionLibrary::Physics__IsRagDoll(AActor* TheCharacter)
{
	ACharacter * AsCharacter = Cast<ACharacter>(TheCharacter);
	if (!AsCharacter) return false;
	
	//Mesh?
	if (!AsCharacter->Mesh) return false;
	
	return AsCharacter->Mesh->IsAnySimulatingPhysics();
}	

bool UVictoryBPFunctionLibrary::Physics__GetLocationofRagDoll(AActor* TheCharacter, FVector& RagdollLocation)
{
	ACharacter * AsCharacter = Cast<ACharacter>(TheCharacter);
	if (!AsCharacter) return false;
	
	//Mesh?
	if (!AsCharacter->Mesh) return false;
	
	TArray<FName> BoneNames;
	AsCharacter->Mesh->GetBoneNames(BoneNames);
	if(BoneNames.Num() > 0)
	{
		RagdollLocation = AsCharacter->Mesh->GetBoneLocation(BoneNames[0]);
	}
	else return false;
	
	return true;
}

bool UVictoryBPFunctionLibrary::Physics__InitializeVictoryRagDoll(
	AActor* TheCharacter, 
	FVector& InitLocation, 
	FRotator& InitRotation
){
	ACharacter * AsCharacter = Cast<ACharacter>(TheCharacter);
	if (!AsCharacter) return false;
	
	//Mesh?
	if (!AsCharacter->Mesh) return false;
	
	InitLocation = AsCharacter->Mesh->GetRelativeTransform().GetLocation();
	InitRotation = AsCharacter->Mesh->GetRelativeTransform().Rotator();
	
	return true;
}

bool UVictoryBPFunctionLibrary::Physics__UpdateCharacterCameraToRagdollLocation(
	AActor* TheCharacter, 
	float HeightOffset,
	float InterpSpeed
){
	ACharacter * AsCharacter = Cast<ACharacter>(TheCharacter);
	if (!AsCharacter) return false;
	
	//Mesh?
	if (!AsCharacter->Mesh) return false;
	
	//Ragdoll?
	if(!AsCharacter->Mesh->IsAnySimulatingPhysics()) return false;
	
	FVector RagdollLocation = FVector(0,0,0);
	TArray<FName> BoneNames;
	AsCharacter->Mesh->GetBoneNames(BoneNames);
	if(BoneNames.Num() > 0)
	{
		RagdollLocation = AsCharacter->Mesh->GetBoneLocation(BoneNames[0]);
	}
	
	//Interp
	RagdollLocation = FMath::VInterpTo(AsCharacter->GetActorLocation(), RagdollLocation + FVector(0,0,HeightOffset), AsCharacter->GetWorld()->DeltaTimeSeconds, InterpSpeed);

	//Set Loc
	AsCharacter->SetActorLocation(RagdollLocation);
	
	return true;
}
/*
bool UVictoryBPFunctionLibrary::Accessor__GetSocketLocalTransform(const USkeletalMeshComponent* Mesh, FTransform& LocalTransform, FName SocketName)
{
	if(!Mesh) return false;
	
	LocalTransform =  Mesh->GetSocketLocalTransform(SocketName);
	
	return true;
}
*/

void UVictoryBPFunctionLibrary::StringConversion__GetFloatAsStringWithPrecision(float TheFloat, FString & FloatString, uint8 Precision)
{
	const TCHAR* TheDot = TEXT(".");
		
	FloatString = FString::SanitizeFloat(TheFloat);
	
	//No . ?
	if( ! FloatString.Contains(TheDot))
	{
		return;
	}
	
	//Split
	FString LeftS;
	FString RightS;
	
	FloatString.Split(TheDot,&LeftS,&RightS);
	
	//Add dot back to LeftS
	LeftS += TheDot;
	
	//Get the Single Number after the precision amount
	// so in .1273, get the 7
	FString RightSFirstTruncated = "";
	if(RightS.Len() - 1 >= Precision)
	{
		RightSFirstTruncated = RightS.Mid(Precision,1);
	}
	
	//Truncate the RightS
	// 	.1273 becomes .12 with precision 2
	RightS = RightS.Left(Precision);
	
	//Round Up if There was any truncated portion
	if(RightSFirstTruncated != "")
	{
		if(FCString::Atod(*RightSFirstTruncated) >= 5)
		{
			//.1273 becomes .13
			RightS = FString::FromInt(FCString::Atod(*RightS) + 1);
		}
	}
	
	FloatString = LeftS + RightS;
}

bool UVictoryBPFunctionLibrary::LensFlare__GetLensFlareOffsets(
	APlayerController* PlayerController,
	AActor* LightSource, 
	float& PitchOffset, 
	float& YawOffset, 
	float& RollOffset
){
	if(!PlayerController) return false;
	if(!LightSource) return false;
	//~~~~~~~~~~~~~~~~~~~
	
	//angle from player to light source
	const FRotator AngleToLightSource = (LightSource->GetActorLocation() - PlayerController->GetFocalLocation()).Rotation();
	
	const FRotator Offsets = AngleToLightSource - PlayerController->GetControlRotation();
	
	PitchOffset = Offsets.Pitch;
	YawOffset = Offsets.Yaw;
	RollOffset = Offsets.Roll;
	return true;
}

/*
bool UVictoryBPFunctionLibrary::AnimatedVertex__GetAnimatedVertexLocations(
	USkeletalMeshComponent* Mesh, 
	TArray<FVector>& Locations
)
{
	if(!Mesh) return false;
	if(!Mesh->SkeletalMesh) return false;
	//~~~~~~~~~
	
	Locations.Empty(); 
	//~~~~~~~~~~~~~~~~~~~
	
	//	Get the Verticies For Each Bone, Most Influenced by That Bone!
	//					Vertices are in Bone space.
	TArray<FBoneVertInfo> BoneVertexInfos;
	FSkeletalMeshTools::CalcBoneVertInfos(Mesh->SkeletalMesh,BoneVertexInfos,true); //true = only dominant influence
	
	//~~~~~~~~~~~~~~~~~~~~~
	int32 VertItr = 0;
	FBoneVertInfo* EachBoneVertInfo;
	FVector BoneWorldPos;
	int32 NumOfVerticies;
	FTransform RV_Transform;
	FVector RV_Vect;
	for(int32 Itr=0; Itr < BoneVertexInfos.Num() ; Itr++)
	{
		EachBoneVertInfo = &BoneVertexInfos[Itr];
		//~~~~~~~~~~~~~~~~~~~~~~~~
		
		//Bone Transform To World Space, and Location
		RV_Transform = Mesh->GetBoneTransform(Itr);
		BoneWorldPos = RV_Transform.GetLocation();
		
		//How many verts is this bone influencing?
		NumOfVerticies = EachBoneVertInfo->Positions.Num();
		for(VertItr=0; VertItr < NumOfVerticies ; VertItr++)
		{
			//Animated Vertex Location!
			Locations.Add(  BoneWorldPos + RV_Transform.TransformVector(EachBoneVertInfo->Positions[VertItr])  );
		}
	}
	
	//~~~ Cleanup ~~~
	BoneVertexInfos.Empty();
	
	return true;
}
	
bool UVictoryBPFunctionLibrary::AnimatedVertex__GetAnimatedVertexLocationsAndNormals(
	USkeletalMeshComponent* Mesh, 
	TArray<FVector>& Locations, 
	TArray<FVector>& Normals 
)
{
	if(!Mesh) return false;
	if(!Mesh->SkeletalMesh) return false;
	//~~~~~~~~~
	
	Locations.Empty(); 
	Normals.Empty();
	//~~~~~~~~~~~~~~~~~~~
	
	//	Get the Verticies For Each Bone, Most Influenced by That Bone!
	//					Vertices are in Bone space.
	TArray<FBoneVertInfo> BoneVertexInfos;
	FSkeletalMeshTools::CalcBoneVertInfos(Mesh->SkeletalMesh,BoneVertexInfos,true); //true = only dominant influence
	
	//~~~~~~~~~~~~~~~~~~~~~
	int32 VertItr = 0;
	FBoneVertInfo* EachBoneVertInfo;
	FVector BoneWorldPos;
	int32 NumOfVerticies;
	FTransform RV_Transform;
	FVector RV_Vect;
	for(int32 Itr=0; Itr < BoneVertexInfos.Num() ; Itr++)
	{
		EachBoneVertInfo = &BoneVertexInfos[Itr];
		//~~~~~~~~~~~~~~~~~~~~~~~~
		
		//Bone Transform To World Space, and Location
		RV_Transform = Mesh->GetBoneTransform(Itr);
		BoneWorldPos = RV_Transform.GetLocation();
		
		//How many verts is this bone influencing?
		NumOfVerticies = EachBoneVertInfo->Positions.Num();
		for(VertItr=0; VertItr < NumOfVerticies ; VertItr++)
		{
			//Animated Vertex Location!
			Locations.Add(  BoneWorldPos + RV_Transform.TransformVector(EachBoneVertInfo->Positions[VertItr])  );
		
			//Animated Vertex Normal for rotating the emitter!!!!!
			Normals.Add(  RV_Transform.TransformVector(EachBoneVertInfo->Normals[VertItr])  );
		}
	}
	
	//~~~ Cleanup ~~~
	BoneVertexInfos.Empty();
	
	return true;
}
	
bool UVictoryBPFunctionLibrary::AnimatedVertex__DrawAnimatedVertexLocations(
	UObject* WorldContextObject,
	USkeletalMeshComponent* Mesh, 
	float ChanceToSkipAVertex, 
	bool DrawNormals
)
{
	UWorld* const TheWorld = GEngine->GetWorldFromContextObject(WorldContextObject);
	
	if(!TheWorld) return false;
	if(!Mesh) return false;
	if(!Mesh->SkeletalMesh) return false;
	//~~~~~~~~~
	
	//	Get the Verticies For Each Bone, Most Influenced by That Bone!
	//					Vertices are in Bone space.
	TArray<FBoneVertInfo> BoneVertexInfos;
	FSkeletalMeshTools::CalcBoneVertInfos(Mesh->SkeletalMesh,BoneVertexInfos,true); //true = only dominant influence
	
	//~~~~~~~~~~~~~~~~~~~~~
	int32 VertItr = 0;
	FBoneVertInfo* EachBoneVertInfo;
	FVector BoneWorldPos;
	int32 NumOfVerticies;
	FTransform RV_Transform;
	FVector RV_Vect;
	
	const FColor HappyRed = FColor(255,0,0);
	const FColor HappyBlue = FColor(0,0,255);
	for(int32 Itr=0; Itr < BoneVertexInfos.Num() ; Itr++)
	{
		EachBoneVertInfo = &BoneVertexInfos[Itr];
		//~~~~~~~~~~~~~~~~~~~~~~~~
		
		//Bone Transform To World Space, and Location
		RV_Transform = Mesh->GetBoneTransform(Itr);
		BoneWorldPos = RV_Transform.GetLocation();
		
		//How many verts is this bone influencing?
		NumOfVerticies = EachBoneVertInfo->Positions.Num();
		for(VertItr=0; VertItr < NumOfVerticies ; VertItr++)
		{
			if(FMath::FRandRange(0, 1) < ChanceToSkipAVertex) continue;
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			
			RV_Vect = BoneWorldPos + RV_Transform.TransformVector(EachBoneVertInfo->Positions[VertItr]);
			
			DrawDebugPoint(
				TheWorld, 
				RV_Vect,
				12, 
				HappyRed, 
				false, 
				0.03
			);
			
			if(DrawNormals)
			{
			DrawDebugLine(
				TheWorld, 
				RV_Vect, 
				RV_Vect + RV_Transform.TransformVector(EachBoneVertInfo->Normals[VertItr] * 64),  
				HappyBlue, 
				false, 
				0.03, 
				0, 
				1
			);
			}
		}
	}
	
	//~~~ Cleanup ~~~
	BoneVertexInfos.Empty();
	
	return true;
}
	
bool UVictoryBPFunctionLibrary::AnimatedVertex__GetCharacterAnimatedVertexLocations(
	AActor* TheCharacter, 
	TArray<FVector>& Locations
)
{
	ACharacter * AsCharacter = Cast<ACharacter>(TheCharacter);
	if (!AsCharacter) return false;
	
	USkeletalMeshComponent* Mesh = AsCharacter->Mesh;
	if (!Mesh) return false;
	//~~~~~~~~~~~~~~~~~~~~
	
	AnimatedVertex__GetAnimatedVertexLocations(Mesh,Locations);
	
	return true;
}
	
bool UVictoryBPFunctionLibrary::AnimatedVertex__GetCharacterAnimatedVertexLocationsAndNormals(
	AActor* TheCharacter, 
	TArray<FVector>& Locations, 
	TArray<FVector>& Normals 
)
{
	ACharacter * AsCharacter = Cast<ACharacter>(TheCharacter);
	if (!AsCharacter) return false;
	
	USkeletalMeshComponent* Mesh = AsCharacter->Mesh;
	if (!Mesh) return false;
	//~~~~~~~~~~~~~~~~~~~~
	
	AnimatedVertex__GetAnimatedVertexLocationsAndNormals(Mesh,Locations,Normals);
	
	return true;
}
	
bool UVictoryBPFunctionLibrary::AnimatedVertex__DrawCharacterAnimatedVertexLocations(
	AActor* TheCharacter, 
	float ChanceToSkipAVertex, 
	bool DrawNormals
)
{	
	ACharacter * AsCharacter = Cast<ACharacter>(TheCharacter);
	if (!AsCharacter) return false;
	
	USkeletalMeshComponent* Mesh = AsCharacter->Mesh;
	if (!Mesh) return false;
	//~~~~~~~~~~~~~~~~~~~~
	
	AnimatedVertex__DrawAnimatedVertexLocations(TheCharacter,Mesh,ChanceToSkipAVertex,DrawNormals);
	
	return true;
}
*/

//SMA Version
float UVictoryBPFunctionLibrary::DistanceToSurface__DistaceOfPointToMeshSurface(AStaticMeshActor* TheSMA, const FVector& TestPoint, FVector& ClosestSurfacePoint)
{
	if(!TheSMA) return -1;
	if(!TheSMA->StaticMeshComponent) return -1;
	//~~~~~~~~~~
	
	//Dist of pt to Surface, retrieve closest Surface Point to Actor
	return TheSMA->StaticMeshComponent->GetDistanceToCollision(TestPoint, ClosestSurfacePoint);
}

bool UVictoryBPFunctionLibrary::Mobility__SetSceneCompMobility(
	USceneComponent* SceneComp,
	EComponentMobility::Type NewMobility
)
{
	if(!SceneComp) return false;
	//~~~~~~~~~~~
	
	SceneComp->SetMobility(NewMobility);
	
	return true;
}



















//~~~~~~~~~~~~~~~~~~
//		FullScreen
//~~~~~~~~~~~~~~~~~~	
TEnumAsByte<EJoyGraphicsFullScreen::Type> UVictoryBPFunctionLibrary::JoyGraphicsSettings__FullScreen_Get()
{
	return TEnumAsByte<EJoyGraphicsFullScreen::Type>(JoyGraphics_FullScreen_GetFullScreenType());
}
	  
void UVictoryBPFunctionLibrary::JoyGraphicsSettings__FullScreen_Set(TEnumAsByte<EJoyGraphicsFullScreen::Type> NewSetting)
{  
	JoyGraphics_FullScreen_SetFullScreenType(NewSetting.GetValue()); 
}














//~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~
//			  Contributed by Others


void UVictoryBPFunctionLibrary::String__ExplodeString(TArray<FString>& OutputStrings, FString InputString, FString Separator, int32 limit, bool bTrimElements)
{
	OutputStrings.Empty();
	//~~~~~~~~~~~
	
	if (InputString.Len() > 0 && Separator.Len() > 0) {
		int32 StringIndex = 0;
		int32 SeparatorIndex = 0;

		FString Section = "";
		FString Extra = "";

		int32 PartialMatchStart = -1;

		while (StringIndex < InputString.Len()) {

			if (InputString[StringIndex] == Separator[SeparatorIndex]) {
				if (SeparatorIndex == 0) {
					//A new partial match has started.
					PartialMatchStart = StringIndex;
				}
				Extra.AppendChar(InputString[StringIndex]);
				if (SeparatorIndex == (Separator.Len() - 1)) {
					//We have matched the entire separator.
					SeparatorIndex = 0;
					PartialMatchStart = -1;
					if (bTrimElements == true) {
						OutputStrings.Add(FString(Section).Trim().TrimTrailing());
					}
					else {
						OutputStrings.Add(FString(Section));
					}

					//if we have reached the limit, stop.
					if (limit > 0 && OutputStrings.Num() >= limit) 
					{
						return;
						//~~~~
					}

					Extra.Empty();
					Section.Empty();
				}
				else {
					++SeparatorIndex;
				}
			}
			else {
				//Not matched.
				//We should revert back to PartialMatchStart+1 (if there was a partial match) and clear away extra.
				if (PartialMatchStart >= 0) {
					StringIndex = PartialMatchStart;
					PartialMatchStart = -1;
					Extra.Empty();
					SeparatorIndex = 0;
				}
				Section.AppendChar(InputString[StringIndex]);
			}

			++StringIndex;
		}

		//If there is anything left in Section or Extra. They should be added as a new entry.
		if (bTrimElements == true) {
			OutputStrings.Add(FString(Section + Extra).Trim().TrimTrailing());
		}
		else {
			OutputStrings.Add(FString(Section + Extra));
		}

		Section.Empty();
		Extra.Empty();
	}
}

UTexture2D* UVictoryBPFunctionLibrary::GetTexture2DFromFile(const FString& FilePath)
{
	UTexture2D* Texture = NULL;

	FString TexturePath = FilePath;//FPaths::GameContentDir( ) + TEXT( "../Data/" ) + TextureFilename;
	TArray<uint8> FileData;

	/* Load DDS texture */
	if( FFileHelper::LoadFileToArray( FileData, *TexturePath, 0 ) )
	{
		FDDSLoadHelper DDSLoadHelper( &FileData[ 0 ], FileData.Num( ) );
		if( DDSLoadHelper.IsValid2DTexture( ) )
		{
			int32 NumMips = DDSLoadHelper.ComputeMipMapCount( );
			EPixelFormat Format = DDSLoadHelper.ComputePixelFormat( );
			int32 BlockSize = 16;

			if( NumMips == 0 )
			{
				NumMips = 1;
			}

			if( Format == PF_DXT1 )
			{
				BlockSize = 8;
			}

			/* Create transient texture */
			Texture = UTexture2D::CreateTransient( DDSLoadHelper.DDSHeader->dwWidth, DDSLoadHelper.DDSHeader->dwHeight, Format );
			
			#if WITH_EDITOR
			Texture->MipGenSettings = TMGS_LeaveExistingMips;
			#endif  //WITH_EDITOR
			
			Texture->PlatformData->NumSlices = 1;
			Texture->NeverStream = true;
			//Texture->Rename(  );

			/* Get pointer to actual data */
			uint8* DataPtr = (uint8*) DDSLoadHelper.GetDDSDataPointer( );

			uint32 CurrentWidth = DDSLoadHelper.DDSHeader->dwWidth;
			uint32 CurrentHeight = DDSLoadHelper.DDSHeader->dwHeight;

			/* Iterate through mips */
			for( int32 i = 0; i < NumMips; i++ )
			{
				/* Lock to 1x1 as smallest size */
				CurrentWidth = ( CurrentWidth < 1 ) ? 1 : CurrentWidth;
				CurrentHeight = ( CurrentHeight < 1 ) ? 1 : CurrentHeight;

				/* Get number of bytes to read */
				int32 NumBytes = CurrentWidth * CurrentHeight * 4;
				if( Format == PF_DXT1 || Format == PF_DXT3 || Format == PF_DXT5 )
				{
					/* Compressed formats */
					NumBytes = ( ( CurrentWidth + 3 ) / 4 ) * ( ( CurrentHeight + 3 ) / 4 ) * BlockSize;
				}

				/* Write to existing mip */
				if( i < Texture->PlatformData->Mips.Num( ) )
				{
					FTexture2DMipMap& Mip = Texture->PlatformData->Mips[ i ];

					void* Data = Mip.BulkData.Lock( LOCK_READ_WRITE );
					FMemory::Memcpy( Data, DataPtr, NumBytes );
					Mip.BulkData.Unlock( );
				}

				/* Add new mip */
				else
				{
					FTexture2DMipMap* Mip = new( Texture->PlatformData->Mips ) FTexture2DMipMap( );
					Mip->SizeX = CurrentWidth;
					Mip->SizeY = CurrentHeight;

					Mip->BulkData.Lock( LOCK_READ_WRITE );
					Mip->BulkData.Realloc( NumBytes );
					Mip->BulkData.Unlock( );

					void* Data = Mip->BulkData.Lock( LOCK_READ_WRITE );
					FMemory::Memcpy( Data, DataPtr, NumBytes );
					Mip->BulkData.Unlock( );
				}

				/* Set next mip level */
				CurrentWidth /= 2;
				CurrentHeight /= 2;

				DataPtr += NumBytes;
			}

			Texture->UpdateResource( );
		}
	}

	return Texture;
}

class UAudioComponent* UVictoryBPFunctionLibrary::PlaySoundAttachedFromFile(const FString& FilePath, class USceneComponent* AttachToComponent, FName AttachPointName, FVector Location, EAttachLocation::Type LocationType, bool bStopWhenAttachedToDestroyed, float VolumeMultiplier, float PitchMultiplier, float StartTime, class USoundAttenuation* AttenuationSettings)
{	
	USoundWave* sw = GetSoundWaveFromFile(FilePath);

	if (!sw)
		return NULL;

	return UGameplayStatics::PlaySoundAttached(sw, AttachToComponent, AttachPointName, Location, LocationType, bStopWhenAttachedToDestroyed, VolumeMultiplier, PitchMultiplier, StartTime, AttenuationSettings);
}

void UVictoryBPFunctionLibrary::PlaySoundAtLocationFromFile(UObject* WorldContextObject, const FString& FilePath, FVector Location, float VolumeMultiplier, float PitchMultiplier, float StartTime, class USoundAttenuation* AttenuationSettings)
{
	USoundWave* sw = GetSoundWaveFromFile(FilePath);

	if (!sw)
		return;

	UGameplayStatics::PlaySoundAtLocation(WorldContextObject, sw, Location, VolumeMultiplier, PitchMultiplier, StartTime, AttenuationSettings);
}

class USoundWave* UVictoryBPFunctionLibrary::GetSoundWaveFromFile(const FString& FilePath)
{
	USoundWave* sw = (USoundWave*)StaticConstructObject(USoundWave::StaticClass());

	if (!sw)
		return NULL;

	//* If true the song was successfully loaded
	bool loaded = false;

	//* loaded song file (binary, encoded)
	TArray < uint8 > rawFile;

	loaded = FFileHelper::LoadFileToArray(rawFile, FilePath.GetCharArray().GetTypedData());

	if (loaded)
	{
		FByteBulkData* bulkData = &sw->CompressedFormatData.GetFormat(TEXT("OGG"));

		bulkData->Lock(LOCK_READ_WRITE);
		FMemory::Memcpy(bulkData->Realloc(rawFile.Num()), rawFile.GetTypedData(), rawFile.Num());
		bulkData->Unlock();

		loaded = fillSoundWaveInfo(sw, &rawFile) == 0 ? true : false;
	}

	if (!loaded)
		return NULL;

	return sw;
}

int32 UVictoryBPFunctionLibrary::fillSoundWaveInfo(class USoundWave* sw, TArray<uint8>* rawFile)
{
    FSoundQualityInfo info;
    FVorbisAudioInfo vorbis_obj = FVorbisAudioInfo();
    if (!vorbis_obj.ReadCompressedInfo(rawFile->GetTypedData(), rawFile->Num(), &info))
    {
        //Debug("Can't load header");
        return 1;
    }

	if(!sw) return 1;
	sw->SoundGroup = ESoundGroup::SOUNDGROUP_Default;
    sw->NumChannels = info.NumChannels;
    sw->Duration = info.Duration;
    sw->RawPCMDataSize = info.SampleDataSize;
    sw->SampleRate = info.SampleRate;

    return 0;
}

      
int32 UVictoryBPFunctionLibrary::findSource(class USoundWave* sw, class FSoundSource* out_audioSource)
{
	FAudioDevice* device = GEngine ? GEngine->GetAudioDevice() : NULL; //gently ask for the audio device

	FActiveSound* activeSound;
	FSoundSource* audioSource;
	FWaveInstance* sw_instance;
	if (!device)
	{
		activeSound = NULL;
		audioSource = NULL;
		out_audioSource = audioSource;
		return -1;
	}

	TArray<FActiveSound*> tmpActualSounds = device->GetActiveSounds();
	if (tmpActualSounds.Num())
	{
		for (auto activeSoundIt(tmpActualSounds.CreateIterator()); activeSoundIt; ++activeSoundIt)
		{
			activeSound = *activeSoundIt;
			for (auto WaveInstanceIt(activeSound->WaveInstances.CreateIterator()); WaveInstanceIt; ++WaveInstanceIt)
			{
				sw_instance = WaveInstanceIt.Value();
				if (sw_instance->WaveData->CompressedDataGuid == sw->CompressedDataGuid)
				{
					audioSource = device->WaveInstanceSourceMap.FindRef(sw_instance);
					out_audioSource = audioSource;
					return 0;
				}
			}
		}
	}

	audioSource = NULL;
	activeSound = NULL;
	out_audioSource = audioSource;
	return -2;
}



//TESTING
static void TESTINGInternalDrawDebugCircle(const UWorld* InWorld, const FMatrix& TransformMatrix, float Radius, int32 Segments, const FColor& Color, bool bPersistentLines, float LifeTime, uint8 DepthPriority, float Thickness=0)
{
	//this is how you can make cpp only internal functions!
	
}
