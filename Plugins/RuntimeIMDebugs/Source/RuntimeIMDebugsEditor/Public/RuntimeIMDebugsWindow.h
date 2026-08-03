#pragma once

#include "SlateIM.h"
#include "SlateIMWidgetBase.h"

struct FDebugTab;
class URuntimeIMDebugsSubsystem;
struct FDebugButton;
struct FDebugToggle;
struct FDebugSpinBox;
struct FDebugFloatField;
struct FDebugComboBox;
struct FDebugSection;
struct FDebugTextField;

enum class ERuntimeIMDebugWindowCommand : uint8;

class FRuntimeIMDebugsExposed : public FSlateIMExposedBase
{
public:

	FRuntimeIMDebugsExposed()
		: FSlateIMExposedBase(TEXT("Debug"))
	{ }

	void DrawContent(float DeltaTime) override;

	void DrawButton(const FDebugButton& InButton, URuntimeIMDebugsSubsystem* InDebugSubsystem);
	
	void DrawToggle(FDebugToggle& Inoggle, URuntimeIMDebugsSubsystem* InDebugSubsystem);
	
	void DrawSpinBox(FDebugSpinBox& InSpinBox, URuntimeIMDebugsSubsystem* InDebugSubsystem);
	
	void DrawFloatField(FDebugFloatField& InFloatField, URuntimeIMDebugsSubsystem* InDebugSubsystem);

	void DrawComboBox(FDebugComboBox& InCombobox, URuntimeIMDebugsSubsystem* InDebugSubsystem);

	void DrawTextField(FDebugTextField& InTextField, URuntimeIMDebugsSubsystem* InDebugSubsystem);

	void DrawDebugSection(FDebugSection& InDebugSection, URuntimeIMDebugsSubsystem* InDebugSubsystem);

	void DrawTab(FDebugTab& InTab, URuntimeIMDebugsSubsystem* InDebugSubsystem);


};

class FRuntimeIMDebugsDockable 
{
public:
	static void RegisterTab();
	static void UnregisterTab();
	static void HandleWindowCommand(ERuntimeIMDebugWindowCommand InCommand);
	static TSharedPtr<FRuntimeIMDebugsExposed> GetRuntimeIMWidget();
	static const FName GetTabId();

private:

	static TSharedPtr<FRuntimeIMDebugsExposed> RuntimeIMWidget;
	static TSharedRef<SDockTab> SpawnTab(const FSpawnTabArgs& Args);
	static const FName TabId;

	inline static FDelegateHandle WindowCommandHandle;

};
