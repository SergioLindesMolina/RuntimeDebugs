#pragma once

#include "SlateIM.h"
#include "SlateIMWidgetBase.h"


struct FDebugTab;
class URuntimeIMDebugsSubsystem;
struct FDebugButton;
struct FDebugToggle;
struct FDebugSlider;
struct FDebugFloatField;

#if WITH_EDITOR

class FRuntimeIMDebugsExposed : public FSlateIMExposedBase
{
public:

	FRuntimeIMDebugsExposed()
		: FSlateIMExposedBase(TEXT("Debug"))
	{ }

	void DrawContent(float DeltaTime) override;

	void DrawButton(const FDebugButton& Button, URuntimeIMDebugsSubsystem* InDebugSubsystem);
	
	void DrawToggle(FDebugToggle& Toggle, URuntimeIMDebugsSubsystem* InDebugSubsystem);
	
	void DrawSlider(FDebugSlider& Slider, URuntimeIMDebugsSubsystem* InDebugSubsystem);
	
	void DrawTab(FDebugTab& InTab, URuntimeIMDebugsSubsystem* InDebugSubsystem);

	void DrawFloatField(FDebugFloatField& InFloatField, URuntimeIMDebugsSubsystem* InDebugSubsystem);

};

class FRuntimeIMDebugsDockable 
{
public:
	static void RegisterTab();
	static void UnregisterTab();
	static TSharedPtr<FRuntimeIMDebugsExposed> GetRuntimeIMWidget();
	static const FName GetTabId();

private:

	static TSharedPtr<FRuntimeIMDebugsExposed> RuntimeIMWidget;
	static TSharedRef<SDockTab> SpawnTab(const FSpawnTabArgs& Args);
	static const FName TabId;
};


#endif