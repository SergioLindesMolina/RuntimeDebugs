// Copyright Epic Games, Inc. All Rights Reserved.

#include "RuntimeIMDebugsEditor.h"
#include "RuntimeIMDebugsWindow.h"


void FRuntimeIMDebugsModuleEditor::StartupModule()
{

	FRuntimeIMDebugsDockable::RegisterTab();

	WorldCleanUpHandle = FWorldDelegates::OnWorldBeginTearDown.AddRaw(this,	&FRuntimeIMDebugsModuleEditor::OnWorldBeginTearDown);
	StartPIEHandle = FEditorDelegates::StartPIE.AddRaw(this, &FRuntimeIMDebugsModuleEditor::OnStartPIE);
}

void FRuntimeIMDebugsModuleEditor::ShutdownModule()
{

	if(WorldCleanUpHandle.IsValid())
		FWorldDelegates::OnWorldBeginTearDown.Remove(WorldCleanUpHandle);
	
	if (StartPIEHandle.IsValid())
		FEditorDelegates::StartPIE.Remove(StartPIEHandle);

	FRuntimeIMDebugsDockable::UnregisterTab();

}

void FRuntimeIMDebugsModuleEditor::OnWorldBeginTearDown(UWorld* InWorld) 
{

	FRuntimeIMDebugsDockable::OnWorldBeginTearDown(InWorld);
}

void FRuntimeIMDebugsModuleEditor::OnStartPIE(const bool InIsSimulating)
{
	FRuntimeIMDebugsDockable::OnStartPIE();

}
	
IMPLEMENT_MODULE(FRuntimeIMDebugsModuleEditor, RuntimeIMDebugsEditor)


