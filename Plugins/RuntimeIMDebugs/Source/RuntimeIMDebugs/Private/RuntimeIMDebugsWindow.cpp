
#include "RuntimeIMDebugsWindow.h"
#include "Engine.h"
#include "RuntimeIMDebugsSubsystem.h"

#include "Framework/Docking/TabManager.h"
#include "Widgets/Docking/SDockTab.h"
#include "Styling/AppStyle.h"

#if WITH_EDITOR

void FRuntimeIMDebugsExposed::DrawContent(float DeltaTime)
{
	SlateIM::BeginBorder(FAppStyle::GetBrush("ToolPanel.GroupBorder"), Orient_Vertical, false);
	SlateIM::Fill();
	SlateIM::HAlign(HAlign_Fill);
	SlateIM::VAlign(VAlign_Fill);
	SlateIM::BeginTabGroup(TEXT("DebugSections"));
	SlateIM::BeginTabStack();

	UWorld* World = nullptr;

	for (const FWorldContext& Context : GEngine->GetWorldContexts())
	{
		if (Context.WorldType == EWorldType::PIE || Context.WorldType == EWorldType::Game)
		{
			World = Context.World();
			break;
		}
	}

	if (World) 
	{
		if (URuntimeIMDebugsSubsystem* DebugSubsystem = World->GetGameInstance()->GetSubsystem<URuntimeIMDebugsSubsystem>())
		{
			TArray<FDebugTab>& TabsToDraw = DebugSubsystem->GetTabs();

			for (FDebugTab& Tab : TabsToDraw)
			{
				DrawTab(Tab, DebugSubsystem);
			}
		}
	}

	SlateIM::EndTabStack();
	SlateIM::EndTabGroup();
	SlateIM::EndBorder();
}

void FRuntimeIMDebugsExposed::DrawButton(const FDebugButton& Button, URuntimeIMDebugsSubsystem* InDebugSubsystem)
{
	if (SlateIM::Button(*Button.Label))
	{
		InDebugSubsystem->OnButtonPressed.Broadcast(Button.Field.ID);
	}
}

void FRuntimeIMDebugsExposed::DrawToggle(FDebugToggle& Toggle, URuntimeIMDebugsSubsystem* InDebugSubsystem)
{
	if (SlateIM::CheckBox(*Toggle.Label, Toggle.Value))
	{
		InDebugSubsystem->OnToggleChanged.Broadcast(Toggle.Field.ID, Toggle.Value);
	}
}

void FRuntimeIMDebugsExposed::DrawSlider(FDebugSlider& Slider, URuntimeIMDebugsSubsystem* InDebugSubsystem)
{
	SlateIM::BeginHorizontalStack();
	SlateIM::Text(Slider.Label);
	if (SlateIM::Slider(Slider.Value, 0, 100, 1))
	{
		InDebugSubsystem->OnSliderChanged.Broadcast(Slider.Field.ID, Slider.Value);
	}

	SlateIM::EndHorizontalStack();
}

void FRuntimeIMDebugsExposed::DrawFloatField(FDebugFloatField& InFloatField, URuntimeIMDebugsSubsystem* InDebugSubsystem)
{
	SlateIM::BeginHorizontalStack();
	SlateIM::Text(InFloatField.Label);

	if (SlateIM::EditableText(InFloatField.CachedStringAsValue))
	{
		InFloatField.Value = FCString::Atof(*InFloatField.CachedStringAsValue);
		InDebugSubsystem->OnDebugFloatFieldChanged.Broadcast(InFloatField.Field.ID, InFloatField.Value);
	}

	SlateIM::EndHorizontalStack();
}

void FRuntimeIMDebugsExposed::DrawTab(FDebugTab& InTab, URuntimeIMDebugsSubsystem* InDebugSubsystem)
{
	if (SlateIM::BeginTab(InTab.ID, FSlateIcon(), FText::FromString(InTab.Label)))
	{
		SlateIM::Fill();
		SlateIM::HAlign(HAlign_Fill);
		SlateIM::VAlign(VAlign_Fill);
		SlateIM::Padding({ 5.f });
		SlateIM::BeginScrollBox();

		TArray<FDebugFieldDrawListEntry> DrawEntries;

		for (FDebugFloatField& FloatField : InTab.FloatFields)
		{
			DrawEntries.Add(FDebugFieldDrawListEntry(FloatField.Field.DrawPriority, [this, &FloatField, InDebugSubsystem]() 
				{
					DrawFloatField(FloatField, InDebugSubsystem);
				}));
		}

		for (FDebugToggle& Toggle : InTab.Toggles)
		{
			DrawEntries.Add(FDebugFieldDrawListEntry(Toggle.Field.DrawPriority, [this, &Toggle, InDebugSubsystem]()
				{
					DrawToggle(Toggle, InDebugSubsystem);
				}));
		}

		for (FDebugSlider& Slider : InTab.Sliders)
		{
			DrawEntries.Add(FDebugFieldDrawListEntry(Slider.Field.DrawPriority, [this, &Slider, InDebugSubsystem]()
				{
					DrawSlider(Slider, InDebugSubsystem);
				}));
		}

		for (const FDebugButton& Button : InTab.Buttons)
		{
			DrawEntries.Add(FDebugFieldDrawListEntry(Button.Field.DrawPriority, [this, &Button, InDebugSubsystem]()
				{
					DrawButton(Button, InDebugSubsystem);
				}));
		}

		DrawEntries.StableSort([](const FDebugFieldDrawListEntry& A, const FDebugFieldDrawListEntry& B)
			{
				return A.DrawPriority < B.DrawPriority;
			});


		for(const FDebugFieldDrawListEntry& DebugEntry : DrawEntries)
		{
			DebugEntry.DrawFunction();
		}

		SlateIM::EndScrollBox();
	}
	SlateIM::EndTab();
}

/*Static variables of FTraeIMDebugExposed initialization of the shared pointer and the fname */

TSharedPtr<FRuntimeIMDebugsExposed> FRuntimeIMDebugsDockable::RuntimeIMWidget = nullptr;

const FName FRuntimeIMDebugsDockable::TabId(TEXT("RuntimeDebugWindow"));

TSharedPtr<FRuntimeIMDebugsExposed> FRuntimeIMDebugsDockable::GetRuntimeIMWidget()
{
	return RuntimeIMWidget;
}

const FName FRuntimeIMDebugsDockable::GetTabId()
{
	return TabId;
}

void FRuntimeIMDebugsDockable::RegisterTab()
{
	TSharedRef<FGlobalTabmanager> TabManager = FGlobalTabmanager::Get();

	if (!TabManager->HasTabSpawner(TabId)) 
	{
		TabManager->RegisterNomadTabSpawner(
			TabId,
			FOnSpawnTab::CreateStatic(&FRuntimeIMDebugsDockable::SpawnTab))
			.SetDisplayName(NSLOCTEXT("RuntimeDebugsWindow", "RuntimeDebugsWindowTab", "RuntimeDebugsWindow"))
			.SetMenuType(ETabSpawnerMenuType::Hidden);
	}

	if (!RuntimeIMWidget.IsValid())
	{
		RuntimeIMWidget = MakeShared<FRuntimeIMDebugsExposed>();
		RuntimeIMWidget->EnableWidget();
	}

}

void FRuntimeIMDebugsDockable::UnregisterTab()
{
	TSharedRef<FGlobalTabmanager> TabManager = FGlobalTabmanager::Get();
	if (TabManager->HasTabSpawner(TabId)) 
	{
		TabManager->UnregisterNomadTabSpawner(TabId);
	}

	if (RuntimeIMWidget.IsValid())
	{
		//This causes a crash when the editor is closing - TODO Find the root reason
		//RuntimeIMWidget->DisableWidget();
		RuntimeIMWidget.Reset();
	}

}

TSharedRef<SDockTab> FRuntimeIMDebugsDockable::SpawnTab(const FSpawnTabArgs& Args)
{
	check(RuntimeIMWidget.IsValid());

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			RuntimeIMWidget->GetExposedWidget()
		];
}

#endif

