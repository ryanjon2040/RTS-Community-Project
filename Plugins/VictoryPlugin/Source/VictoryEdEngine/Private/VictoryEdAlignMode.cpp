// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

//Victory Alignment Mode

//#include "VictoryGame.h"
#include "VictoryEdEnginePCH.h"
#include "ScopedTransaction.h"

#define LOCTEXT_NAMESPACE "VictoryVertexSnapEditor"

//~~~~~~~~~~~~~~~~

#define CHECK_VSELECTED if(!VictoryEngine) return; if(!VictoryEngine->VSelectedActor) return;

//~~~ Display Choices ~~~
#define VERTEX_DISPLAY_STARS 		0
#define VERTEX_DISPLAY_3DBOX 		1
#define VERTEX_DISPLAY_RECT 			2
#define VERTEX_DISPLAY_DIAMOND3D 	3
#define VERTEX_DISPLAY_SPHERE 		4
#define VERTEX_SELECTED_MULT		1.333
#define VERTEX_SHAPE_MULT			0.8
//~~~ Defines ~~~

//MAX
#define MAX_VERTEX_COUNT_FOR_DRAWING 10000
#define MAX_VERTEX_COUNT_FOR_DRAWING_SPHERES 5000

//Speeds
#define XYZSPEEDMULT 20

//3D
#define DEFAULT_INSTANT_MOVE_DISTANCE 2048
#define CURSOR_DELTA_DISTANCE_CALC 2048
//HUD
#define VICTORY_TITLE_HEIGHT 38
#define VICTORY_TEXT_HEIGHT 24

//Color
#define RED 		0
#define BLUE		1
#define YELLOW 3

//TIME
#define VICTORY_TITLE_VISIBLE_DURATION 2

//Button Vibes
#define BUTTON_VIBE_VICTORYHOTKEYS 		0

//~~~ Const ~~~
const FLinearColor FVictoryEdAlignMode::RV_Red = FLinearColor(1,0,0,1);
const FLinearColor FVictoryEdAlignMode::RV_Yellow = FLinearColor(1,1,0,1);
const FLinearColor FVictoryEdAlignMode::RV_Blue= FLinearColor(0,0,1,1);
const FColor FVictoryEdAlignMode::RV_VRed = FColor(255,0,0,255);
const FColor FVictoryEdAlignMode::RV_VBlue = FColor(0,0,255,255);
const FColor FVictoryEdAlignMode::RV_VYellow = FColor(255,255,0,255);
	

FVictoryEdAlignMode::FVictoryEdAlignMode()
{
	//FEdMode::FEdMode(); //call super?
}
FVictoryEdAlignMode::~FVictoryEdAlignMode()
{	
}

//INIT
void FVictoryEdAlignMode::JoyInit(UVictoryEdEngine* EnginePtr)
{
	//ID = FName("VictoryEditorMode");
	
	
	//Always Initialize Your Pointers!
	SelectedVertexBuffer = nullptr;
	HighlightedVertexBuffer = nullptr;

	  
	//VictoryEngine
	VictoryEngine = EnginePtr;
	
	//Victory Buttons
	RefreshVictoryButtons();

	//Enable Realtime
	ReEntering = true;
	
	//Verticies
	DrawVerticiesMode = 2;
	VertexDisplayChoice = VERTEX_DISPLAY_STARS;
	
	//~~~
	
	UsingMouseInstantMove = false;
	CurrentVerticiesScale = 12;
	
	//~~~
	
	//Get an actor, not sure if this matters
	//TObjectIterator< AActor> ActorItr;
	
	//Traces
	RV_TraceParams = FCollisionQueryParams(FName(TEXT("HUDRMBDown")), true, NULL);
	RV_TraceParams.bTraceComplex = true;
	//RV_TraceParams.bTraceAsyncScene = true;
	RV_TraceParams.bReturnPhysicalMaterial = false;
}

void FVictoryEdAlignMode::RefreshVictoryButtons()
{
	VictoryButtons.Empty();
	//~~~~~~~~~~~~
	
	FVButton NewButton;
	NewButton.Vibe 	= BUTTON_VIBE_VICTORYHOTKEYS;
	NewButton.minX	=	10;
	NewButton.maxX	=	200;
	NewButton.minY	=	VICTORY_TITLE_HEIGHT;
	NewButton.maxY	=	VICTORY_TITLE_HEIGHT + VICTORY_TEXT_HEIGHT;
	VictoryButtons.Add(NewButton);
}

//EXIT
void FVictoryEdAlignMode::Exit()
{
	UsingMouseInstantMove = false;
}
//ENTER
void FVictoryEdAlignMode::Enter()
{
	//FEdMode::Enter();
	//~~~~~~~~
	
	//Victory Title Appears
	VictoryTitleAppears();
	ReEntering = true;
	
	//Clear Snap Key!
	SnapKeyPressed = false;
	
	//Clear keys
	MinusIsDown = false;
	PlusIsDown = false;
	ShiftDown = false;
	
	//Get an actor, not sure if this matters
	//TObjectIterator< AActor> ActorItr;
	
	//Trace
	RV_TraceParams = FCollisionQueryParams(FName(TEXT("HUDRMBDown")), true, NULL);
	
	//~~~~~~~~~~~~~~~~
	
	//testing
	CHECK_VSELECTED
	
	//CreateUModel();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

FDropToClosestSurfaceData* FVictoryEdAlignMode::GetActorWithShortestDrop(TArray<FDropToClosestSurfaceData>& TheData)
{
	//No Actors
	if(TheData.Num() < 1) return NULL;
	
	//Only 1 Actor
	if(TheData.Num() < 2) return &TheData[0];
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
	float SmallestDistance = 100000000;
	int32 SmallestDataIndex = 0;
	
	//Find Smallest
	for(int32 b = 0; b < TheData.Num(); b++)
	{
		if( TheData[b].DistSquared < SmallestDistance)
		{
			SmallestDistance = TheData[b].DistSquared;
			SmallestDataIndex = b;
		}
	}
	
	//Valid?
	if(TheData.IsValidIndex(SmallestDataIndex))
	{
		return &TheData[SmallestDataIndex];
	}
	
	return NULL;
}
void FVictoryEdAlignMode::DropSelectedActorsToNearestSurface()
{
	if(!VictoryEngine) return;
	//~~~~~~~~~~~~~
	

	//ONLY WORKING WITH SMAs at the moment, due to nature of rest of plugin
	AStaticMeshActor* AsSMA = NULL;
	
	//cause I dont know how to make the widge move after dropping one or group of objects
	
	//~~~~~~~~~~~~~~
	// Closest Surface Data
	//~~~~~~~~~~~~~~
	TArray<FDropToClosestSurfaceData> ClosestSurfaceData;
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//TRACE PARAMETERS
	FCollisionQueryParams TraceParams(FName(TEXT("VictoryEd Trace")), true, NULL);
	TraceParams.bTraceComplex = true;
	//TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = false;
	
	//Ignore  ALL Selected Actors
	for(FSelectionIterator VSelectItr = VictoryEngine->GetSelectedActorIterator(); 
		VSelectItr; ++VSelectItr )
	{
		VSelectItrActor = Cast<AActor>(*VSelectItr);
		if(!VSelectItrActor) continue;
		//~~~~~~~~~~~~~~~~~~
		
		//Add
		TraceParams.AddIgnoredActor(VSelectItrActor);
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
	//~~~~~~~~~~~
	//Hit Result
	FHitResult TheHit;
	//~~~~~~~~~~~

	
	//Iterate Over Selected Actors
	FVector ActorLoc;
	FVector BottomPoint;
	FVector OffsetFromCenter;
	for(FSelectionIterator VSelectItr = VictoryEngine->GetSelectedActorIterator(); 
		VSelectItr; ++VSelectItr )
	{
		AsSMA = Cast<AStaticMeshActor>(*VSelectItr);
		if(!AsSMA) continue;
		//~~~~~~~~~~~~~~~~~~
		
		//~~~~~~~~~~~~~~~
		//~~~ Do the Trace ~~~
		//~~~~~~~~~~~~~~~

		//Re-initialize hit info
		TheHit = FHitResult(ForceInit);
		
		//Trace!
		ActorLoc = AsSMA->GetActorLocation();
		GetWorld()->LineTraceSingle(
			TheHit,		//result
			ActorLoc,								
			ActorLoc + FVector(0,0,-20000),
			ECC_Pawn, //collision channel
			TraceParams
		);
		
		//Hit any Actor?
		if(!TheHit.GetActor()) continue;
		//~~~~~~~~~~~~~~~~~
		
		
		//See .h
		GetBottomSurfacePoint(AsSMA,BottomPoint);
		
		
		//Offset From Center
		OffsetFromCenter = ActorLoc - BottomPoint;
			
		//~~~ ADD Closest Surface Data ~~~
		FDropToClosestSurfaceData NewData;
		NewData.TheActor = AsSMA;
		NewData.SurfaceLoc = TheHit.ImpactPoint + OffsetFromCenter;
		NewData.DistSquared = FVector::DistSquared(AsSMA->GetActorLocation(),NewData.SurfaceLoc);
		ClosestSurfaceData.Add(NewData);
		
	}
	
	
	//Get Smallest Drop Data
	FDropToClosestSurfaceData* SmallestDropData = GetActorWithShortestDrop(ClosestSurfaceData);
	if(!SmallestDropData) return;
	//~~~~~~~~~~~~~~~
	
	//Get Smallest Drop Actor
	AStaticMeshActor* AnchorActor = SmallestDropData->TheActor;
	if(!AnchorActor) return;
	//~~~~~~~~~~~~~~~
	
	
	
	//Get Relative Offsets of this to all other actors in selection
	const FVector Origin = AnchorActor->GetActorLocation();
	TArray<FVector> AnchorOffsets;
	AActor* AsActor;
	for(FSelectionIterator VSelectItr = VictoryEngine->GetSelectedActorIterator(); 
		VSelectItr; ++VSelectItr )
	{
		AsActor = Cast<AActor>(*VSelectItr);
		if(!AsActor) continue;
		//~~~~~~~~~~~~~~~~~~
		
		AnchorOffsets.Add(AsActor->GetActorLocation() - Origin);
	}
	
	
	//~~~ Move Anchor Actor ~~~
	
	//Move to impact, Plus the offset from nearest surface of object
	AnchorActor->SetActorLocation(SmallestDropData->SurfaceLoc);
	
	//Move all other actors to Anchor Actor + Offset
	int32 AnchorItr = 0;
	for(FSelectionIterator VSelectItr = VictoryEngine->GetSelectedActorIterator(); 
		VSelectItr; ++VSelectItr )
	{
		AsActor = Cast<AActor>(*VSelectItr);
		if(!AsActor) continue;
		//~~~~~~~~~~~~~~~~~~
		
		//VALID?
		if(!AnchorOffsets.IsValidIndex(AnchorItr)) break;
		
		AsActor->SetActorLocation(SmallestDropData->SurfaceLoc + AnchorOffsets[AnchorItr]);
		
		//INC
		AnchorItr++;
	}
	
	
	//~~~~~~~~~~~~~~
	
	//Update Display!
	DoSingleDisplayUpdate = true;
	
	
	//~~~ RESELECT SINGLE ACTOR TO MOVE WIDGET
	if(VictoryEngine->GetSelectedActorCount() < 2)
	{
		//~~~~~~~~~~~~~~~~~
		//Deselect without Noting
		VictoryEngine->SelectNone(false, true, false);
		VictoryEngine->SelectActor(AnchorActor,true,true);
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

FPositionVertexBuffer* FVictoryEdAlignMode::GetVerticies(AStaticMeshActor* TheSMA)
{
	if(!TheSMA) return NULL;
	if(!TheSMA->IsValidLowLevel()) return NULL;
	if(!TheSMA->StaticMeshComponent) return NULL;
	if(!TheSMA->StaticMeshComponent->StaticMesh) return NULL;
	if(!TheSMA->StaticMeshComponent->StaticMesh->RenderData) return NULL;
	
	//Valid LOD ?
	if(TheSMA->StaticMeshComponent->StaticMesh->RenderData->LODResources.Num() <= 0) return NULL;
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
	//~~~~~~~~~~~~~~~~~~~~
	//				Vertex Buffer Check
	FPositionVertexBuffer* VertexBuffer = 
		&TheSMA->StaticMeshComponent->StaticMesh->RenderData->LODResources[0].PositionVertexBuffer;
	
	//~~~~~~~~~~~~~~~~~~~~
	//			Count Too High?
	if(VertexBuffer->GetNumVertices() > MAX_VERTEX_COUNT_FOR_DRAWING)
	{
		//UE_LOG(Victory, Error, TEXT("Vertex Count too high to draw! %d"), VertexBuffer->GetNumVertices() );
		return NULL;
		//~~~~~~~
	}
	
	//Return Vertex Buffer
	return &TheSMA->StaticMeshComponent->StaticMesh->RenderData->LODResources[0].PositionVertexBuffer;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*
UENUM()
enum EInputEvent
{
	IE_Pressed              =0,
	IE_Released             =1,
	IE_Repeat               =2,
	IE_DoubleClick          =3,
	IE_Axis                 =4,
	IE_MAX                  =5,
};
*/

//Pressed
void FVictoryEdAlignMode::InputKeyPressed(FKey Key)
{
	//UE_LOG(Victory, Error, TEXT("Key pressed %s"), *EKeys::GetKeyName(Key).ToString() );
	
	if(!VictoryEngine) return;
	//~~~~~~~~~~~~
	
	//RMB
	if(Key == EKeys::RightMouseButton) 
	{
		RMBDown = true;
		return;
		//~~~~~~
	}
	
	//Left Mouse
	if(Key == EKeys::LeftMouseButton)
	{
		LeftMouseClick();
		return;
		//~~~~~~
	}
	
	//X Key
	if(Key == EKeys::X) 
	{
		XDown = true;
		return;
	}
	
	//Y Key
	if(Key == EKeys::Y) 
	{
		DropSelectedActorsToNearestSurface();
		return;
	}
	
	//U Key
	if(Key == EKeys::U) 
	{
		YDown = true;
		DrawVerticiesMode++;
		if(DrawVerticiesMode > 2) DrawVerticiesMode = 0;
		return;
	}
	
	//K Key
	if(Key == EKeys::K) 
	{
		//No modifier keys
		if(	Key == EKeys::LeftShift || Key == EKeys::RightShift ||
			Key == EKeys::LeftControl || Key == EKeys::RightControl ||
			Key == EKeys::LeftAlt || Key == EKeys::RightAlt
		) return;
		
		if(VictoryEngine->VSelectedActor)
			VictoryEngine->VSelectedActor->SetActorRotation(FRotator::ZeroRotator);
		return;
	}
	
	//SHIFT KEY
	if(Key == EKeys::LeftShift || Key == EKeys::RightShift) 
	{
		ShiftDown = true;
		return;
	}
	
	
	//- Key
	if(Key == EKeys::Underscore) 
	{
		MinusIsDown = true;
		PendingButtonRefresh = true;
		return;
	}
	//+ Key
	if(Key == EKeys::Equals) 
	{
		PlusIsDown = true;
		PendingButtonRefresh = true;
		return;
	}
	
	//T Key
	if(Key == EKeys::T)
	{
		UsingMouseInstantMove = !UsingMouseInstantMove;
		return;
		//~~~~~~~~~
	}
	
	//V Key
	if(Key == EKeys::V)
	{
		//CTRL KEY
		if(Key == EKeys::LeftControl || Key == EKeys::RightControl) return;
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
		SnapKeyPressed = true;
		return;
		//~~~~~~~~~
	}
	//B Key
	if(Key == EKeys::B)
	{
		VertexDisplayChoice++;
		if(VertexDisplayChoice > 4) VertexDisplayChoice = 0;
		return;
		//~~~~~~~~~
	}
	
	
	//Restore Previous Editor Mode
	if(Key == EKeys::P)
	{
		//Clear all Verticies
	
		//Exit
		VictoryEngine->ExitVictoryAlignMode();
		
		return;
		//~~~~~~
	}
}




//Released
void FVictoryEdAlignMode::InputKeyReleased(FKey Key)
{
	//Vertex Key
	if(Key == EKeys::V)
	{
		//UNDO SAVE BEFORE MOVE
		const FScopedTransaction Transaction( NSLOCTEXT("Snap","VictoryGame", "Victory Vertex Snap" )  );
		VictoryEngine->VSelectedActor->Modify();

		SnapKeyPressed = false;
	}
	//RMB
	else if(Key == EKeys::RightMouseButton)
	{
		PendingButtonRefresh = true;
		RMBDown = false;
	}
	
	//LMB
	else if(Key == EKeys::LeftMouseButton)
	{
		PendingButtonRefresh = true;
	}
	
	//SHIFT KEY
	if(Key == EKeys::LeftShift || Key == EKeys::RightShift) ShiftDown = false;
	
	//- Key
	else if(Key == EKeys::Underscore) MinusIsDown = false;
	
	//+ Key
	else if(Key == EKeys::Equals)  PlusIsDown = false;
	
	//X Key
	else if(Key == EKeys::X) XDown = false;
	
	//Y Key
	else if(Key == EKeys::Y) YDown = false;
	
}

//Double Click
void FVictoryEdAlignMode::InputKeyDoubleClick(FKey Key)
{
	//UE_LOG(Victory, Error, TEXT("Key DOUBLE CLICK %s"), *EKeys::GetKeyName(Key).ToString() );
}

bool FVictoryEdAlignMode::InputKey(FEditorViewportClient* ViewportClient,FViewport* Viewport,FKey Key,EInputEvent Event)
{
	//Middle Mouse
	if (Key == EKeys::MiddleMouseButton) 			MiddleMouseEvent(Event);
	else if(Event == EInputEvent::IE_Pressed) 		InputKeyPressed(Key);
	else if(Event == EInputEvent::IE_Released) 	InputKeyReleased(Key);
	else if(Event == EInputEvent::IE_DoubleClick) 	InputKeyDoubleClick(Key);
	
	return false;
}

void FVictoryEdAlignMode::ProcessMouseInstantMove(FEditorViewportClient* ViewportClient)
{
	CHECK_VSELECTED
	if(!ViewportClient) return;
	//~~~~~~~~~~~~~~~
	
	//Re-initialize hit info
	RV_Hit = FHitResult(ForceInit);
	
	//Clear Previous Ignore Actors
	RV_TraceParams.IgnoreActors.Empty();
	
	//Ignore All Selected Actors!!!
	for(FSelectionIterator VSelectItr = VictoryEngine->GetSelectedActorIterator(); 
		VSelectItr; ++VSelectItr )
	{
		VSelectItrActor = Cast<AActor>(*VSelectItr);
		if(!VSelectItrActor) continue;
		//~~~~~~~~~~~~~~~~~~
		
		//Add
		RV_TraceParams.AddIgnoredActor(VSelectItrActor);
	}
	
	//Trace
	GetWorld()->LineTraceSingle(
		RV_Hit,		//result
		CursorWorldPos,	//start
		CursorWorldPos + 100000 * CursorWorldDir , //end
		ECC_WorldStatic, //collision channel
		RV_TraceParams
	);
	
	//Store Hit Actor
	RV_HitActor = RV_Hit.GetActor();
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//				Update Highlighted Actor
	
		//does  not equal same		  does not equal the main Selected Actor
	if(RV_HitActor != HighlightedActor && RV_HitActor != VictoryEngine->VSelectedActor )
	{
		//potentially deselect any highlighted
		HighlightedActor = Cast<AStaticMeshActor>(RV_HitActor);
		
		//Refresh Vertex Buffer
		HighlightedVertexBuffer = NULL;
		if(HighlightedActor) HighlightedVertexBuffer = GetVerticies(HighlightedActor);
		
		
	}
	
	if(RV_Hit.bBlockingHit)
	{
		//INVALIDATE THE DISPLAY
		ViewportClient->Viewport->InvalidateDisplay();
	}
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//			PROCESS MOUSE INSTANT MOVE?
	if(!UsingMouseInstantMove) return;
	//~~~~~~~~~~~~~~~~~~~~~~~~
	
	//If not hit something, use default distance
	if(!RV_Hit.bBlockingHit) RV_Hit.ImpactPoint = CursorWorldPos + DEFAULT_INSTANT_MOVE_DISTANCE * CursorWorldDir;
	
	
	//		Move the Primary, the Origin!
	RV_Vect = RV_Hit.ImpactPoint;
		
		//Use Existing Z of Object
	RV_Vect.Z = VictoryEngine->VSelectedActor->GetActorLocation().Z;
		
		//Update Location
	VictoryEngine->VSelectedActor->SetActorLocation(RV_Vect);	
		
		//Save New Origin Location
	const FVector NewOrigin(RV_Vect);
	
	//~~~~~~~
	
	//Update the Others According to the Moved Origin
	RV_Int32 = 0;
	for(FSelectionIterator VSelectItr = VictoryEngine->GetSelectedActorIterator(); 
		VSelectItr; ++VSelectItr )
	{
		VSelectItrActor = Cast<AActor>(*VSelectItr);
		if(!VSelectItrActor) 
		{
			RV_Int32++;
			continue;
		}
		//~~~~~~~~~~~~~~~~~~
		
		//Get New Origin
		RV_Vect = NewOrigin;
		
		//Add Offset
		if(VictoryEngine->SelectionRelativeOffsets.IsValidIndex(RV_Int32))
		{
			RV_Vect+=VictoryEngine->SelectionRelativeOffsets[RV_Int32];
		}
		
		//Update Location
		VSelectItrActor->SetActorLocation(RV_Vect);
		
		//Inc
		RV_Int32++;
	}
	
	//INVALIDATE THE DISPLAY
	ViewportClient->Viewport->InvalidateDisplay();
	
}

bool FVictoryEdAlignMode::InputDelta( FEditorViewportClient* InViewportClient,FViewport* InViewport,FVector& InDrag,FRotator& InRot,FVector& InScale )
{
	//only works if LMB is down
	
	return false;
}

bool FVictoryEdAlignMode::InputAxis(FEditorViewportClient* InViewportClient,FViewport* Viewport,int32 ControllerId,FKey Key,float Delta,float DeltaTime)
{
	//only works if any mouse button is down
	
	return false;
}

bool FVictoryEdAlignMode::MouseMove(FEditorViewportClient* ViewportClient,FViewport* Viewport,int32 x, int32 y)
{
	if(!ViewportClient) return false;
	
	//~~~~~~~~~~~~~~~
	//	ESSENTIAL UPDATES
	MouseLocation.X=x;
	MouseLocation.Y=y;
	
	
	
	return false;
}

//Middle Mouse Event
void FVictoryEdAlignMode::MiddleMouseEvent(EInputEvent Event)
{
	//Pressed
	if(Event == EInputEvent::IE_Pressed)
	{
		
	
		return;
	}
	
	//Released
	if(Event == EInputEvent::IE_Released)
	{
		
		return;
	}
}

//Left Mouse Click
void FVictoryEdAlignMode::LeftMouseClick()
{
	CHECK_VSELECTED
	
	//SelectedVertexForSelectedActor = -1;
}
bool FVictoryEdAlignMode::HandleClick(FEditorViewportClient* InViewportClient, HHitProxy *HitProxy, const FViewportClick &Click)
{

	return false;
}

//~~~

//``
void FVictoryEdAlignMode::GetSelectedVertexLocation(FVector& LocOut)
{
	CHECK_VSELECTED
	
	if(!SelectedVertexBuffer)  return;	
	//~~~~~~~~~~~~~~~~~~
	
	const FTransform SMATransform = VictoryEngine->VSelectedActor->GetTransform();
	const FVector SMALocation 		= SMATransform.GetLocation();
	const int32 VertexCount 			= SelectedVertexBuffer->GetNumVertices();
	for(int32 Itr = 0; Itr < VertexCount; Itr++)
	{
		if(Itr == SelectedVertexForSelectedActor) 
		{
			LocOut = SMALocation + SMATransform.TransformVector(SelectedVertexBuffer->VertexPosition(Itr));
			return;
			//~~~~
		}
	}
}

void FVictoryEdAlignMode::DoVertexSnap(const FVector& Dest)
{
	CHECK_VSELECTED
	
	
	FVector SelectedVertexLocation;
	
	//Initial Location
	GetSelectedVertexLocation(SelectedVertexLocation);
	
	//Match rotation of owner of target vertex?
	if(ShiftDown)
	{
		VictoryEngine->VSelectedActor->SetActorRotation(RV_Hit.ImpactNormal.Rotation());
		//Recalc Selected Vertex After Rotation
		GetSelectedVertexLocation(SelectedVertexLocation);
		
		//UE_LOG(Victory, Error, TEXT("WHAT IS HIT NORMAL %s"), *RV_Hit.ImpactNormal.Rotation().ToString() );
	}
	
	//No modifier, just regular click
	
	//1. calc distance between selected vertex and JSMA location
	//2. move the JSMA to the target vertex
	//3. subtract the distance
	
	//Distance Offset
	const FVector LocOffset = SelectedVertexLocation - VictoryEngine->VSelectedActor->GetActorLocation();
	
	//Set Location
	VictoryEngine->VSelectedActor->SetActorLocation(Dest - LocOffset);
	
	//Refresh After Moving
	PendingButtonRefresh = true;
	
}

void FVictoryEdAlignMode::RefreshVertexButtons(const FSceneView* View)
{
	CHECK_VSELECTED
	
	if(!View) return;
	//~~~~~~~~~~~~~
	
	//No Longer Pending
	PendingButtonRefresh = false;
	
	//Refresh
	SelectedActorButtons.Empty();
	HighlightedActorButtons.Empty();
	
	//~~~ Vars ~~~
	const float ButtonHalfSize = CurrentVerticiesScale/2;
	
	//~~~~~~~~~~~~~~~~~~~~~
	//			Selected Actor
	//~~~~~~~~~~~~~~~~~~~~~
	if(!VictoryEngine->VSelectedActor->IsValidLowLevel() ) return;
	
	if(!SelectedVertexBuffer)  return;	
	//~~~~~~~~~~~~~~~~~~
	
	FTransform SMATransform = VictoryEngine->VSelectedActor->GetTransform();
	FVector SMALocation 		= SMATransform.GetLocation();
	const int32 VertexCount 			= SelectedVertexBuffer->GetNumVertices();
	for(int32 Itr = 0; Itr < VertexCount; Itr++)
	{
		//Get Rotated Scaled Translated Vertex Pos
		VertexWorldSpace = SMALocation + SMATransform.TransformVector(SelectedVertexBuffer->VertexPosition(Itr));
		
		//Get 2D Center
		VictoryProject(View, 
			VertexWorldSpace, 
			Vertex2DCenter 
		);
		
		FVButton NewButton;
		NewButton.Vibe 			= 	Itr;
		NewButton.PointInWorld 	= 	VertexWorldSpace;
		NewButton.minX			=	Vertex2DCenter.X - ButtonHalfSize;
		NewButton.maxX			=	Vertex2DCenter.X + ButtonHalfSize;
		NewButton.minY			=	Vertex2DCenter.Y - ButtonHalfSize;
		NewButton.maxY			=	Vertex2DCenter.Y + ButtonHalfSize;

		//Add Button
		SelectedActorButtons.Add(NewButton);
	}
	
	//~~~~~~~~~~~~~~~~~~~~~
	//			Highlighted Actor
	//~~~~~~~~~~~~~~~~~~~~~
	if(!HighlightedActor) return;
	if(!HighlightedActor->IsValidLowLevel()) return;
	
	if(!HighlightedVertexBuffer)  return;	
	//~~~~~~~~~~~~~~~~~~
	
	SMATransform = HighlightedActor->GetTransform();
	SMALocation 		= SMATransform.GetLocation();
	const int32 VertexCount2 			= HighlightedVertexBuffer->GetNumVertices();
	for(int32 Itr = 0; Itr < VertexCount2; Itr++)
	{
		//Get Rotated Scaled Translated Vertex Pos
		VertexWorldSpace = SMALocation + SMATransform.TransformVector(HighlightedVertexBuffer->VertexPosition(Itr));
		
		//Get 2D Center
		VictoryProject(View, 
			VertexWorldSpace, 
			Vertex2DCenter 
		);
		
		FVButton NewButton;
		NewButton.Vibe 			= 	Itr;
		NewButton.PointInWorld 	= 	VertexWorldSpace;
		NewButton.minX			=	Vertex2DCenter.X - ButtonHalfSize;
		NewButton.maxX			=	Vertex2DCenter.X + ButtonHalfSize;
		NewButton.minY			=	Vertex2DCenter.Y - ButtonHalfSize;
		NewButton.maxY			=	Vertex2DCenter.Y + ButtonHalfSize;

		//Add Button
		HighlightedActorButtons.Add(NewButton);
	}
	
}

//```
void FVictoryEdAlignMode::PDI_DrawVerticies(const FSceneView* View, FPrimitiveDrawInterface* PDI, const FPositionVertexBuffer* VertexBuffer, const FTransform& SMATransform, bool DrawingSelectedActor)
{
	CHECK_VSELECTED
	
	if(!View) 			return;
	if(!PDI) 			return;
	if(!VertexBuffer) 	return;
	 
	if(!VictoryEngine->VSelectedActor->IsA(AStaticMeshActor::StaticClass())) return;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	//Location
	const FVector SMALocation 			= SMATransform.GetLocation();
	 
	//Get Count
	//		how is it crashing here?
	const int32 VertexCount = VertexBuffer->GetNumVertices();
	
	//~~~~~~~~~~~~~~~~~~~~~
	//			Selected Actor
	//~~~~~~~~~~~~~~~~~~~~~
	if(DrawingSelectedActor)
	{
	for(int32 Itr = 0; Itr < VertexCount; Itr++)
	{
		//Selected Vertex?
		if(SelectedVertexForSelectedActor == Itr )
		{
			//Draw to the PDI
			PDI->DrawPoint(
				SMALocation + SMATransform.TransformVector(VertexBuffer->VertexPosition(Itr)),
				RV_Yellow,
				CurrentVerticiesScale*VERTEX_SELECTED_MULT,
				0 //depth
			);
			continue;
		}
		else if(HighlightedVertexForSelectedActor == Itr)
		{
			//Draw to the PDI
			PDI->DrawPoint(
				SMALocation + SMATransform.TransformVector(VertexBuffer->VertexPosition(Itr)),
				FLinearColor(0,1,1,1),
				CurrentVerticiesScale*VERTEX_SELECTED_MULT,
				0 //depth
			);
			continue;
		}
		else
		{
			if(DrawVerticiesMode < 2) continue;
			//~~~~~~~~~~~~~~~~~~~
			
			//Spheres
			if(VertexDisplayChoice == VERTEX_DISPLAY_SPHERE)
			{
				if(VertexCount > MAX_VERTEX_COUNT_FOR_DRAWING_SPHERES)
				{
				DrawWireBox(
					PDI,
					BoxFromPointWithSize(SMALocation + SMATransform.TransformVector(VertexBuffer->VertexPosition(Itr)),CurrentVerticiesScale*0.5),
					RV_VRed,
					0
				);
				continue;
				}
				else
				{
				DrawWireSphere(
					PDI, 
					SMALocation + SMATransform.TransformVector(VertexBuffer->VertexPosition(Itr)),
					RV_VRed, 
					CurrentVerticiesScale*VERTEX_SHAPE_MULT, 
					12, 
					0
				);
				continue;
				}
			}
			
			//Diamond
			else if(VertexDisplayChoice == VERTEX_DISPLAY_DIAMOND3D)
			{
			DrawWireSphere(
				PDI, 
				SMALocation + SMATransform.TransformVector(VertexBuffer->VertexPosition(Itr)),
				RV_VRed, 
				CurrentVerticiesScale*VERTEX_SHAPE_MULT, 
				4, 
				0
			);
			continue;
			}
			
			//Box
			else if(VertexDisplayChoice == VERTEX_DISPLAY_3DBOX)
			{
			DrawWireBox(
				PDI,
				BoxFromPointWithSize(SMALocation + SMATransform.TransformVector(VertexBuffer->VertexPosition(Itr)),CurrentVerticiesScale*VERTEX_SHAPE_MULT),
				RV_VRed,
				0
			);
			continue;
			}
			
			//Stars
			else if(VertexDisplayChoice == VERTEX_DISPLAY_STARS)
			{
			DrawWireStar(
				PDI,
				SMALocation + SMATransform.TransformVector(VertexBuffer->VertexPosition(Itr)),
				CurrentVerticiesScale, 
				RV_VRed,
				0
			);
			continue;
			}
			
			//Rect
			else if(VertexDisplayChoice == VERTEX_DISPLAY_RECT)
			{
			//Draw to the PDI
			PDI->DrawPoint(
				SMALocation + SMATransform.TransformVector(VertexBuffer->VertexPosition(Itr)),
				RV_Red,
				CurrentVerticiesScale,
				0 //depth
			);
			continue;
			}
			
		}
			
	}
	}
	
	//~~~~~~~~~~~~~~~~~~~~~
	//			Highlighted Actor
	//~~~~~~~~~~~~~~~~~~~~~
	else
	{
	for(int32 Itr = 0; Itr < VertexCount; Itr++)
	{
		if(HighlightedVertexForHighlightedActor == Itr)
		{
			//Draw to the PDI
			PDI->DrawPoint(
				SMALocation + SMATransform.TransformVector(VertexBuffer->VertexPosition(Itr)),
				FLinearColor(0,1,1,1),
				CurrentVerticiesScale*VERTEX_SELECTED_MULT,
				0 //depth
			);
			continue;
		}
		
		//~~~~~~~~~~~~~~~~~~~
		if(DrawVerticiesMode < 2) continue;
		//~~~~~~~~~~~~~~~~~~~
		
		
		//Spheres
		if(VertexDisplayChoice == VERTEX_DISPLAY_SPHERE)
		{
			if(VertexCount > MAX_VERTEX_COUNT_FOR_DRAWING_SPHERES)
			{
			DrawWireBox(
				PDI,
				BoxFromPointWithSize(SMALocation + SMATransform.TransformVector(VertexBuffer->VertexPosition(Itr)),CurrentVerticiesScale*VERTEX_SHAPE_MULT),
				RV_VBlue,
				0
			);
			continue;
			}
			else
			{
			DrawWireSphere(
				PDI, 
				SMALocation + SMATransform.TransformVector(VertexBuffer->VertexPosition(Itr)),
				RV_VBlue, 
				CurrentVerticiesScale*VERTEX_SHAPE_MULT, 
				12, 
				0
			);
			continue;
			}
		}
		
		//Diamond
		else if(VertexDisplayChoice == VERTEX_DISPLAY_DIAMOND3D)
		{
		DrawWireSphere(
			PDI, 
			SMALocation + SMATransform.TransformVector(VertexBuffer->VertexPosition(Itr)),
			RV_VBlue, 
			CurrentVerticiesScale*VERTEX_SHAPE_MULT, 
			4, 
			0
		);
		continue;
		}
			
		//Box
		else if(VertexDisplayChoice == VERTEX_DISPLAY_3DBOX)
		{
		DrawWireBox(
			PDI,
			BoxFromPointWithSize(SMALocation + SMATransform.TransformVector(VertexBuffer->VertexPosition(Itr)),CurrentVerticiesScale),
			RV_VBlue,
			0
		);
		continue;
		}
		
		//Stars
		else if(VertexDisplayChoice == VERTEX_DISPLAY_STARS)
		{
		
		DrawWireStar(
			PDI,
			SMALocation + SMATransform.TransformVector(VertexBuffer->VertexPosition(Itr)),
			CurrentVerticiesScale, 
			RV_VBlue,
			0
		);
		continue;
		}
		
		//Rect
		else if(VertexDisplayChoice == VERTEX_DISPLAY_RECT)
		{
		//Draw to the PDI
		PDI->DrawPoint(
			SMALocation + SMATransform.TransformVector(VertexBuffer->VertexPosition(Itr)),
			RV_Blue,
			CurrentVerticiesScale,
			0 //depth
		);
		continue;
		}
	}	
	}
}


void FVictoryEdAlignMode::DrawHotkeyToolTip(FCanvas* Canvas)
{
	if(!Canvas) return;
	//~~~~~~~~~
	
	RV_yStart = MouseLocation.Y + 16;
	
	//Background
	DrawVictoryRect(Canvas, 
		MouseLocation.X - 7, 
		RV_yStart - 7,
		420,
		VICTORY_TEXT_HEIGHT * 10 + 120,
		FLinearColor(0,0,1,0.777)
	);
	
	DrawVictoryTextWithColor(Canvas, 
		"~~~ Victory Editor Mode Hotkeys ~~~", 
		MouseLocation.X,RV_yStart,
		RV_Yellow
	);
	
	RV_yStart += VICTORY_TEXT_HEIGHT * 2;
	DrawVictoryTextWithColor(Canvas, 
		"P ~ Restore previous Editor Mode", 
		MouseLocation.X,RV_yStart,
		RV_Yellow
	);
	
	RV_yStart += VICTORY_TEXT_HEIGHT + 20;
	DrawVictoryTextWithColor(Canvas, 
		"Y ~ Drop Objects to Nearest Surface!", 
		MouseLocation.X,RV_yStart,
		RV_Yellow
	);
	
	RV_yStart += VICTORY_TEXT_HEIGHT + 20;
	DrawVictoryTextWithColor(Canvas, 
		"T ~ Toggle Instant Mouse Move Mode", 
		MouseLocation.X,RV_yStart,
		RV_Yellow
	);
	
	RV_yStart += VICTORY_TEXT_HEIGHT;
	DrawVictoryTextWithColor(Canvas, 
		"K ~ Reset Rotation", 
		MouseLocation.X,RV_yStart,
		RV_Yellow
	);
	
	RV_yStart += VICTORY_TEXT_HEIGHT + 20;
	DrawVictoryTextWithColor(Canvas, 
		"V ~ Select Verticies for Snapping!", 
		MouseLocation.X,RV_yStart,
		RV_Yellow
	);
	
	RV_yStart += VICTORY_TEXT_HEIGHT + 5;
	DrawVictoryTextWithColor(Canvas, 
		"SHIFT + V ~ Snap by Surface Normal", 
		MouseLocation.X,RV_yStart,
		RV_Yellow
	);
	
	RV_yStart += VICTORY_TEXT_HEIGHT + 5;
	DrawVictoryTextWithColor(Canvas, 
		"B ~ Toggle Different Vertex Types!!!", 
		MouseLocation.X,RV_yStart,
		RV_Yellow
	);
	
	RV_yStart += VICTORY_TEXT_HEIGHT + 4;
	DrawVictoryTextWithColor(Canvas, 
		"Hold + or - ~ Change Vertex Size!", 
		MouseLocation.X,RV_yStart,
		RV_Yellow
	);
	
	RV_yStart += VICTORY_TEXT_HEIGHT + 4;
	DrawVictoryTextWithColor(Canvas, 
		"U ~ Toggle Hiding of All Verticies", 
		MouseLocation.X,RV_yStart,
		RV_Yellow
	);
}

void FVictoryEdAlignMode::DrawVictoryText(FCanvas* Canvas, const FString& TheStr, float X, float Y, float TheScale )
{
	if(!Canvas) return;
	//~~~~~~~~~
	
	FCanvasTextItem NewText(
		FVector2D(X,Y),
		FText::FromString(TheStr),
		VictoryEngine->GetVictoryFont(),
		FLinearColor(1,0,1,1)
	);
	NewText.Scale.Set(TheScale,TheScale);
	NewText.bOutlined = true;
	NewText.Draw(Canvas);
}
void FVictoryEdAlignMode::DrawVictoryTextWithColor(FCanvas* Canvas, const FString& TheStr, float X, float Y, FLinearColor TheColor, float TheScale )
{
	if(!Canvas) return;
	//~~~~~~~~~
	
	FCanvasTextItem NewText(
		FVector2D(X,Y),
		FText::FromString(TheStr),
		VictoryEngine->GetVictoryFont(),
		TheColor
	);
	NewText.Scale.Set(TheScale,TheScale);
	NewText.bOutlined = true;
	
	//Outline gets its alpha from the main color
	NewText.OutlineColor = FLinearColor(0,0,0,TheColor.A * 2);
	
	NewText.Draw(Canvas);
}

void FVictoryEdAlignMode::DrawVictoryLine(
	FCanvas* Canvas, 
	const FVector2D& Start, 
	const FVector2D& End, 
	FLinearColor TheColor, 
	float Thick
)
{
	if(!Canvas) return;
	//~~~~~~~~~
	
	FCanvasLineItem NewLine(Start,End);
	NewLine.SetColor(TheColor);
	NewLine.LineThickness = Thick;
	NewLine.Draw(Canvas);
}
void FVictoryEdAlignMode::DrawVictoryRect(FCanvas* Canvas, float X, float Y, float Width, float Height, FLinearColor Color)
{
	if(!Canvas) return;
	//~~~~~~~~~
	
	FCanvasTileItem TileItem( 
		FVector2D(X, Y), 
		FVector2D( Width, Height ), 
		Color 
	);
   
    TileItem.BlendMode = SE_BLEND_Translucent;
	TileItem.Draw(Canvas);
	
	/*
	Canvas->DrawTile( 
		X, Y, 
		Width, Height, 
		0, 0, 1, 1, 
		Color
	);
	*/

}

int32 FVictoryEdAlignMode::FindClosestOfButtons(TArray<FVButton*>& Inbuttons)
{
	float CurDistance;
	float MinDistance = 10000000000;
	int32 MinIndex = -1;
	for(int32 Itr=0; Itr < Inbuttons.Num(); Itr++)
	{
		CurCheckButton = Inbuttons[Itr];
		if(!CurCheckButton) continue;
		
		//world space location stored in the Button itself
		CurDistance = FVector::DistSquared(CurCheckButton->PointInWorld,CursorWorldPos);
		
		//Min Check
		if(CurDistance < MinDistance)
		{
			MinDistance = CurDistance;
			MinIndex = Itr;
		}
	}
	
	//VALIDITY CHECK
	if(!Inbuttons.IsValidIndex(MinIndex))
		return -1;
		
	else return MinIndex;
}

void FVictoryEdAlignMode::CheckCursorInButtons(FCanvas* Canvas)
{	
	if(!Canvas) return;
	//~~~~~~~~~

	//Reset Highlighted
	HighlightedVertexForSelectedActor = -1;
	HighlightedVertexForHighlightedActor = -1;
	
	//~~~~~~~~~~~~~~~~~~~
	//Main Buttons
	//~~~~~~~~~~~~~~~~~~~
	RV_Int32=0;
	while(VictoryButtons.IsValidIndex(RV_Int32))
	{
		CurCheckButton = &VictoryButtons[RV_Int32];
		
		//check cursor in bounds
		if (CurCheckButton->minX <= MouseLocation.X && MouseLocation.X <= CurCheckButton->maxX &&
			CurCheckButton->minY <= MouseLocation.Y && MouseLocation.Y <= CurCheckButton->maxY )
		{
			//Active Button Type
			ActiveButton_Vibe = CurCheckButton->Vibe; 
			
			if(ActiveButton_Vibe == BUTTON_VIBE_VICTORYHOTKEYS)
			{
				DrawHotkeyToolTip(Canvas);
				FadeInVictoryTitle = true;
				VictoryTitleVisible = false; //prevent a duel between the two
				return;
				//~~~~~
			}
		}
		
		//~~~~~~~
		RV_Int32++;
	}
	
	//~~~~~~~~~~~~~~~~~~~
	//Selected Actor Vertex Buttons
	//~~~~~~~~~~~~~~~~~~~
	RV_Int32=0;
	ClosestSelectedActorButtons.Empty();
	while(SelectedActorButtons.IsValidIndex(RV_Int32))
	{
		CurCheckButton = &SelectedActorButtons[RV_Int32];
		//check cursor in bounds
		if (CurCheckButton->minX <= MouseLocation.X && MouseLocation.X <= CurCheckButton->maxX &&
			CurCheckButton->minY <= MouseLocation.Y && MouseLocation.Y <= CurCheckButton->maxY )
		{
			ClosestSelectedActorButtons.Add(&SelectedActorButtons[RV_Int32]);
		}
		
		//~~~~~~~
		RV_Int32++;
	}
	
	//Find closest of potentially highlighted/selected
	const int32 FoundIndex = FindClosestOfButtons(ClosestSelectedActorButtons);
	if(FoundIndex != -1)
	{
		CurCheckButton = ClosestSelectedActorButtons[FoundIndex];
		if(CurCheckButton)
		{
			//Active Button Type
			ActiveButton_Vibe = CurCheckButton->Vibe; 
			
			//Highlight Vertex if hovered over!
			HighlightedVertexForSelectedActor = ActiveButton_Vibe;
			
			//~~~~~~~~~~~~~~~~~~~~~~
			//			VERTEX SELECTION
			//~~~~~~~~~~~~~~~~~~~~~~
			//Vertex Selected and not same?
			if(SnapKeyPressed && SelectedVertexForSelectedActor != ActiveButton_Vibe )
			{
				SelectedVertexForSelectedActor = ActiveButton_Vibe;
				return;
				//~~~~~
			}
		}
	}
		
	
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
	
	//~~~~~~~~~~~~~~~~~~~~~~
	//Highlighted Actor Vertex Buttons
	//~~~~~~~~~~~~~~~~~~~~~~
	
	RV_Int32=0;
	ClosestHighlightedActorButtons.Empty();
	while(HighlightedActorButtons.IsValidIndex(RV_Int32))
	{
		CurCheckButton = &HighlightedActorButtons[RV_Int32];
		//check cursor in bounds
		if (CurCheckButton->minX <= MouseLocation.X && MouseLocation.X <= CurCheckButton->maxX &&
			CurCheckButton->minY <= MouseLocation.Y && MouseLocation.Y <= CurCheckButton->maxY )
		{
			ClosestHighlightedActorButtons.Add(&HighlightedActorButtons[RV_Int32]);
		}
		
		//~~~~~~~
		RV_Int32++;
	}
	
	//Find closest of potentially highlighted
	
	const int32 FoundIndex2 = FindClosestOfButtons(ClosestHighlightedActorButtons);
	if(FoundIndex2 != -1)
	{
		CurCheckButton = ClosestHighlightedActorButtons[FoundIndex2];
		if(CurCheckButton)
		{
			//Active Button Type
			ActiveButton_Vibe = CurCheckButton->Vibe; 
			
			//Highlight Vertex if hovered over!
			HighlightedVertexForHighlightedActor = ActiveButton_Vibe;
			
			//Vertex Selected?
			if(SnapKeyPressed && SelectedVertexForSelectedActor != -1)
			{
				DoVertexSnap(CurCheckButton->PointInWorld);
			}
		}
	}
}

//Victory Project
void FVictoryEdAlignMode::VictoryProject(const FSceneView* View,const FVector& WorldPoint,FVector2D& OutPixelLocation)
{
	if(!View) return;
	//~~~~~~~~~
	
	/** Transforms a point from the view's world-space into pixel coordinates relative to the view's X,Y (left, top). */
	View->WorldToPixel(WorldPoint,OutPixelLocation);
}

//Victory DeProject
void FVictoryEdAlignMode::VictoryDeProject(const FSceneView* View,const FVector2D& ScreenPoint, FVector& out_WorldOrigin, FVector& out_WorldDirection)
{
	if(!View) return;
	//~~~~~~~~~
	
	//NEEDS TO BE MADE CONST TO BE ABLE TO USE THIS
	/** transforms 2D screen coordinates into a 3D world-space origin and direction 
	 * @param ScreenPos - screen coordinates in pixels
	 * @param out_WorldOrigin (out) - world-space origin vector
	 * @param out_WorldDirection (out) - world-space direction vector
	 */
	//View->DeprojectFVector2D(ScreenPoint, out_WorldOrigin, out_WorldDirection);

	
	/** transforms 2D screen coordinates into a 3D world-space origin and direction 
	 * @param ScreenPos - screen coordinates in pixels
	 * @param ViewRect - view rectangle
	 * @param InvViewMatrix - inverse view matrix
	 * @param InvProjMatrix - inverse projection matrix
	 * @param out_WorldOrigin (out) - world-space origin vector
	 * @param out_WorldDirection (out) - world-space direction vector
	 */
	FSceneView::DeprojectScreenToWorld(
		ScreenPoint, 
		View->ViewRect, 
		View->InvViewMatrix, 
		View->ViewMatrices.GetInvProjMatrix(), 
		out_WorldOrigin, 
		out_WorldDirection
	);

}
	

//Draw Using the More Fundamental Method, PDI
void FVictoryEdAlignMode::Render(const FSceneView* View,FViewport* Viewport,FPrimitiveDrawInterface* PDI)
{
	//~~~ Verticies ~~~
	if(DrawVerticiesMode > 0 )
	{
	
	if(SelectedVertexBuffer && VictoryEngine->VSelectedActor)
	{
		PDI_DrawVerticies(View,	PDI,
			SelectedVertexBuffer,VictoryEngine->VSelectedActor->GetTransform(),
			true
		);
	}
	
	//Dont draw if Highlighted == Selected, drawn already
	if(HighlightedVertexBuffer && HighlightedActor && HighlightedActor != VictoryEngine->VSelectedActor) 
	{
		PDI_DrawVerticies( View,	PDI,
			HighlightedVertexBuffer, HighlightedActor->GetTransform(),
			false //Highlighted Actor
		);
	}
	
	} //end of do draw verticies
	//~~~~~~~~~~~~~~~~~~~
}

//Draw HUD
void FVictoryEdAlignMode::DrawHUD(FEditorViewportClient* ViewportClient,FViewport* Viewport,const FSceneView* View,FCanvas* Canvas)
{
	if(!Canvas) return;
	if(!VictoryEngine) return;
	//~~~~~~~~~~~
	
	//Title Button
	DrawVictoryTextWithColor(Canvas, "Victory Editor Hotkeys", 10,VICTORY_TITLE_HEIGHT, FLinearColor(1,0,1,VictoryTitleAlpha));
		
	//~~~ Cursor ~~~
	if(!UsingMouseInstantMove) CheckCursorInButtons(Canvas);
	
	//~~~ Make Buttons? ~~~
	if(PendingButtonRefresh) RefreshVertexButtons(View);
	
	//~~~~~~~~~~~~~~~~~~~
	//~~~~~~~~~~~~~~~~~~~
	//~~~~~~~~~~~~~~~~~~~
	
	//~~~ Cursor World Space ~~~
	VictoryDeProject(View, MouseLocation,CursorWorldPos,CursorWorldDir);
	
	//Calc Delta
	RV_Vect = CursorWorldPos + CursorWorldDir * CURSOR_DELTA_DISTANCE_CALC;
	CursorWorldDelta = RV_Vect - CursorWorldPrevPos;
	CursorWorldDelta /= 4;
	if(CursorWorldDelta.SizeSquared() < 3) CursorWorldDelta = FVector::ZeroVector;
	if(CursorWorldDelta.SizeSquared() > 10000) CursorWorldDelta = FVector::ZeroVector;
	
	//Save Prev
	CursorWorldPrevPos = RV_Vect;
	
	//~~~~~~~~~~~~~~~
	
	//SUPER HYPER BUTTON REFRESHING
	//		due to the slide physx camera thing when moving
	if(!UsingMouseInstantMove) PendingButtonRefresh = true;
}

void FVictoryEdAlignMode::Tick_VictoryTitle(FEditorViewportClient* ViewportClient)
{
	CurTimeSpan = FDateTime::Now() - VictoryTitleAppearTime;
	
	//If > then Allowed Visible Duration, start to fade out
	if(CurTimeSpan.GetTotalSeconds() > VICTORY_TITLE_VISIBLE_DURATION)
	{
		if(VictoryTitleAlpha <= 0)
		{
			VictoryTitleVisible = false;
			VictoryTitleAlpha = 0;
			return;
		}
		
		VictoryTitleAlpha -= 0.01;
	}
	
	//INVALIDATE THE DISPLAY
	ViewportClient->Viewport->InvalidateDisplay();
}
void FVictoryEdAlignMode::Tick_VictoryTitleFadeIn()
{
	if(VictoryTitleAlpha >=1)
	{
		VictoryTitleAppears();
		return;
	}
	
	VictoryTitleAlpha += 0.01;
}

//TICK
void FVictoryEdAlignMode::Tick_VictoryInterface(FEditorViewportClient* ViewportClient)
{
	if(!ViewportClient) return;
	//~~~~~~~~~~~
	
	//Re-activate Real-time each time enter Mode
	/*
	if(ReEntering)
	{
		ReEntering = false;
		ViewportClient->SetRealtime(true,true);
	}
	*/
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//							HUD
	ViewportSize = ViewportClient->Viewport->GetSizeXY();
	
	//Victory Title Tick
	if(VictoryTitleVisible) Tick_VictoryTitle(ViewportClient);
	
	//Fading in?
	if(FadeInVictoryTitle) 
	{
		Tick_VictoryTitleFadeIn();
		ViewportClient->Viewport->InvalidateDisplay();
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~						
	//			Move cursor if RMB is down
	//if(RMBDown) ViewportClient->Viewport->SetMouse(ViewportSize.X/2, ViewportSize.Y/2);
}

bool FVictoryEdAlignMode::ShouldDrawWidget() const
{
	if(UsingMouseInstantMove) return false;
	return true;
}


bool FVictoryEdAlignMode::GetCursor(EMouseCursor::Type& OutCursor) const
{
	if(UsingMouseInstantMove)
	{
		OutCursor = EMouseCursor::GrabHand;
		return true;
	}
	
	return false; //no modified cursor
}

void FVictoryEdAlignMode::Tick(FEditorViewportClient* ViewportClient,float DeltaTime)
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//				Single Invalidate
	if(DoSingleDisplayUpdate) 
	{
		ViewportClient->Viewport->InvalidateDisplay();
		DoSingleDisplayUpdate = false;
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//				HUD Elements and Input
	Tick_VictoryInterface(ViewportClient);
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//				TRACE CURSOR POSITION
	ProcessMouseInstantMove(ViewportClient);
	
	
	
	//~~~ Begin Vertex Related ~~~
	CHECK_VSELECTED
	
	//Minus or Plus?
	if(MinusIsDown) CurrentVerticiesScale -= 0.333;
	else if(PlusIsDown) CurrentVerticiesScale += 0.333;
	if(CurrentVerticiesScale < 2) CurrentVerticiesScale = 2;
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//					New Selection Actor?
	if(VictoryEngine->SelectedActorVerticiesNeedsUpdating)
	{
		VictoryEngine->SelectedActorVerticiesNeedsUpdating = false;
		SelectedVertexBuffer = GetVerticies(Cast<AStaticMeshActor>(VictoryEngine->VSelectedActor));
		PendingButtonRefresh = true;
	}
	
	if(VictoryEngine->ClearSelectedVertex)
	{
		VictoryEngine->ClearSelectedVertex = false;
		SelectedVertexForSelectedActor = -1;
	}
}