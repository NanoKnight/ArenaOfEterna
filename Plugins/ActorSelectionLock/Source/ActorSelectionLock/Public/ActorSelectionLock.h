// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Framework/Application/IInputProcessor.h"

class FActorSelectionLockInputProcessor : public IInputProcessor
{
public:
	virtual void Tick(const float DeltaTime,FSlateApplication& SlateApp,TSharedRef<ICursor> Cursor) override {}

	virtual bool HandleMouseButtonUpEvent(FSlateApplication& SlateApp,const FPointerEvent& MouseEvent) override;

private:
	void FixSelectionAfterClick();
};

class FActorSelectionLockModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	TSharedPtr<FActorSelectionLockInputProcessor> InputProcessor;
};