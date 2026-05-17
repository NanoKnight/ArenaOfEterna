#include "ActorSelectionLock.h"

#include "Editor.h"
#include "Engine/Selection.h"
#include "GameFramework/Actor.h"
#include "Framework/Application/SlateApplication.h"
#include "LevelEditorViewport.h"
#include "SceneView.h"
#include "CollisionQueryParams.h"

bool FActorSelectionLockInputProcessor::HandleMouseButtonUpEvent(
	FSlateApplication& SlateApp,
	const FPointerEvent& MouseEvent)
{
	if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		FramesToCheck = 10;
		FixSelectionAfterClick();
	}

	return false;
}

void FActorSelectionLockInputProcessor::Tick(
	const float DeltaTime,
	FSlateApplication& SlateApp,
	TSharedRef<ICursor> Cursor)
{
	if (FramesToCheck > 0)
	{
		FramesToCheck--;
		FixSelectionAfterClick();
	}
}

void FActorSelectionLockInputProcessor::FixSelectionAfterClick()
{
	if (!GEditor)
	{
		return;
	}

	USelection* SelectedActors = GEditor->GetSelectedActors();

	if (!SelectedActors)
	{
		return;
	}

	AActor* LockedSelectedActor = nullptr;

	for (FSelectionIterator It(*SelectedActors); It; ++It)
	{
		AActor* Actor = Cast<AActor>(*It);

		if (Actor && Actor->ActorHasTag(TEXT("SelectionLocked")))
		{
			LockedSelectedActor = Actor;
			break;
		}
	}

	if (!LockedSelectedActor)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Locked actor deselected: %s"), *LockedSelectedActor->GetName());

	GEditor->SelectActor(LockedSelectedActor, false, false);
	GEditor->NoteSelectionChange();

	FViewport* Viewport = GEditor->GetActiveViewport();

	if (!Viewport)
	{
		return;
	}

	FLevelEditorViewportClient* ViewportClient =
		static_cast<FLevelEditorViewportClient*>(Viewport->GetClient());

	if (!ViewportClient || !ViewportClient->GetWorld())
	{
		return;
	}

	FSceneViewFamilyContext ViewFamily(
		FSceneViewFamily::ConstructionValues(
			Viewport,
			ViewportClient->GetScene(),
			ViewportClient->EngineShowFlags
		)
	);

	FSceneView* SceneView = ViewportClient->CalcSceneView(&ViewFamily);

	if (!SceneView)
	{
		return;
	}

	const int32 MouseX = Viewport->GetMouseX();
	const int32 MouseY = Viewport->GetMouseY();

	FViewportCursorLocation CursorLocation(
		SceneView,
		ViewportClient,
		MouseX,
		MouseY
	);

	const FVector LineStart = CursorLocation.GetOrigin();
	const FVector LineEnd = LineStart + CursorLocation.GetDirection() * 1000000.f;

	FCollisionQueryParams Params;
	Params.bTraceComplex = true;
	Params.AddIgnoredActor(LockedSelectedActor);

	TArray<FHitResult> Hits;

	ViewportClient->GetWorld()->LineTraceMultiByChannel(
		Hits,
		LineStart,
		LineEnd,
		ECC_Visibility,
		Params
	);

	for (const FHitResult& Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();

		if (!HitActor)
		{
			continue;
		}

		if (HitActor->ActorHasTag(TEXT("SelectionLocked")))
		{
			continue;
		}

		GEditor->SelectActor(HitActor, true, true);
		GEditor->NoteSelectionChange();

		UE_LOG(LogTemp, Warning, TEXT("Selected actor behind locked actor: %s"), *HitActor->GetName());

		break;
	}
}

void FActorSelectionLockModule::StartupModule()
{
	UE_LOG(LogTemp, Warning, TEXT("ActorSelectionLock Plugin Started"));

	InputProcessor = MakeShared<FActorSelectionLockInputProcessor>();

	FSlateApplication::Get().RegisterInputPreProcessor(InputProcessor);
}

void FActorSelectionLockModule::ShutdownModule()
{
	if (InputProcessor.IsValid() && FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().UnregisterInputPreProcessor(InputProcessor);
	}

	InputProcessor.Reset();
}

IMPLEMENT_MODULE(FActorSelectionLockModule, ActorSelectionLock)