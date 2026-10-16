// Copyright Epic Games, Inc. All Rights Reserved.

#include "RuntimeIMDebugsEditor.h"
#include "RuntimeIMDebugsWindow.h"


void FRuntimeIMDebugsModuleEditor::StartupModule()
{
	FRuntimeIMDebugsDockable::RegisterTab();

	StartPIEHandle = FEditorDelegates::StartPIE.AddRaw(this, &FRuntimeIMDebugsModuleEditor::OnStartPIE);
	EditorPreExitHandle = FEditorDelegates::OnEditorPreExit.AddRaw(this, &FRuntimeIMDebugsModuleEditor::OnEditorPreExit);

}

void FRuntimeIMDebugsModuleEditor::ShutdownModule()
{
	if (StartPIEHandle.IsValid())
		FEditorDelegates::StartPIE.Remove(StartPIEHandle);

	if (EditorPreExitHandle.IsValid())
		FEditorDelegates::OnEditorPreExit.Remove(EditorPreExitHandle);
}


void FRuntimeIMDebugsModuleEditor::OnStartPIE(const bool InIsSimulating)
{
	FRuntimeIMDebugsDockable::OnStartPIE();

}

void FRuntimeIMDebugsModuleEditor::OnEditorPreExit()
{
	//UE_LOG(LogTemp, Warning, TEXT(" EDITOR PRE EXIT"));

	FRuntimeIMDebugsDockable::UnregisterTab();
}
	
IMPLEMENT_MODULE(FRuntimeIMDebugsModuleEditor, RuntimeIMDebugsEditor)


