
#include "RuntimeIMDebugsWindow.h"
#include "RuntimeIMDebugsSubsystem.h"
#include "RuntimeIMDebugsSettings.h"
#include "RuntimeIMDebugsLog.h"

#include "Engine.h"

#include "Framework/Docking/TabManager.h"
#include "Widgets/Docking/SDockTab.h"
#include "Styling/AppStyle.h"


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
		if (URuntimeIMDebugsSubsystem* DebugSubsystem = World->GetSubsystem<URuntimeIMDebugsSubsystem>())
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

void FRuntimeIMDebugsExposed::DrawButton(const FDebugButton& InButton, URuntimeIMDebugsSubsystem* InDebugSubsystem)
{
	if (SlateIM::Button(*InButton.Label))
	{
		InDebugSubsystem->OnButtonPressed.Broadcast(InButton.Field.ID);
	}
}

void FRuntimeIMDebugsExposed::DrawToggle(FDebugToggle& InToggle, URuntimeIMDebugsSubsystem* InDebugSubsystem)
{
	if (SlateIM::CheckBox(*InToggle.Label, InToggle.Value))
	{
		InDebugSubsystem->OnToggleChanged.Broadcast(InToggle.Field.ID, InToggle.Value);
	}
}

void FRuntimeIMDebugsExposed::DrawSpinBox(FDebugSpinBox& InSpinBox, URuntimeIMDebugsSubsystem* InDebugSubsystem)
{
	SlateIM::BeginHorizontalStack();
	SlateIM::Text(InSpinBox.Label);
	if (SlateIM::SpinBox(InSpinBox.Value, InSpinBox.Min, InSpinBox.Max))
	{
		InDebugSubsystem->OnSpinBoxChanged.Broadcast(InSpinBox.Field.ID, InSpinBox.Value);
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

void FRuntimeIMDebugsExposed::DrawComboBox(FDebugComboBox& InComboBox, URuntimeIMDebugsSubsystem* InDebugSubsystem)
{
	SlateIM::BeginHorizontalStack();
	SlateIM::Text(InComboBox.Label);

	if (SlateIM::ComboBox(InComboBox.Options, InComboBox.Index, false))
	{
		InDebugSubsystem->OnDebugComboBoxChanged.Broadcast(InComboBox.Field.ID, InComboBox.Index);
	}

	SlateIM::EndHorizontalStack();
}

void FRuntimeIMDebugsExposed::DrawTextField(FDebugTextField& InTextField, URuntimeIMDebugsSubsystem* InDebugSubsystem)
{
	SlateIM::BeginHorizontalStack();
	SlateIM::Text(InTextField.Label);
	SlateIM::Text(InTextField.Text);
	SlateIM::EndHorizontalStack();
}

void FRuntimeIMDebugsExposed::DrawDebugSection(FDebugSection& InDebugSection, URuntimeIMDebugsSubsystem* InDebugSubsystem)
{	
	const URuntimeIMDebugsSettings* Settings = GetDefault<URuntimeIMDebugsSettings>();

	SlateIM::AutoSize();
	SlateIM::BeginBorder(
		&Settings->SectionBorderBrush,
		Orient_Vertical,
		false);

	// Header
	SlateIM::AutoSize();
	SlateIM::BeginBorder(
		&Settings->SectionBorderHeaderBrush,
		Orient_Vertical,
		false,
		FMargin(8.f, 6.f));

	SlateIM::Text(InDebugSection.Label);

	SlateIM::EndBorder();
	
	SlateIM::Padding(FMargin(10.f, 0.f, 10.f, 5.f));
	SlateIM::BeginVerticalStack();
	
	//Add all the debug fileds to an array of draw entires to sort them by draw priority 

	TArray<FDebugFieldDrawListEntry> DrawEntries;

	for (FDebugFloatField& FloatField : InDebugSection.FloatFields)
	{
		DrawEntries.Add(FDebugFieldDrawListEntry(FloatField.Field.DrawPriority, [this, &FloatField, InDebugSubsystem]()
			{
				DrawFloatField(FloatField, InDebugSubsystem);
			}));
	}

	for (FDebugComboBox& ComboBox : InDebugSection.ComboBoxes)
	{
		DrawEntries.Add(FDebugFieldDrawListEntry(ComboBox.Field.DrawPriority, [this, &ComboBox, InDebugSubsystem]()
			{
				DrawComboBox(ComboBox, InDebugSubsystem);
			}));
	}

	for (FDebugTextField& TextField : InDebugSection.TextFields)
	{
		DrawEntries.Add(FDebugFieldDrawListEntry(TextField.Field.DrawPriority, [this, &TextField, InDebugSubsystem]()
			{
				DrawTextField(TextField, InDebugSubsystem);
			}));
	}

	for (FDebugToggle& Toggle : InDebugSection.Toggles)
	{
		DrawEntries.Add(FDebugFieldDrawListEntry(Toggle.Field.DrawPriority, [this, &Toggle, InDebugSubsystem]()
			{
				DrawToggle(Toggle, InDebugSubsystem);
			}));
	}

	for (FDebugSpinBox& SpinBox : InDebugSection.SpinBoxes)
	{
		DrawEntries.Add(FDebugFieldDrawListEntry(SpinBox.Field.DrawPriority, [this, &SpinBox, InDebugSubsystem]()
			{
				DrawSpinBox(SpinBox, InDebugSubsystem);
			}));
	}

	for (const FDebugButton& Button : InDebugSection.Buttons)
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


	for (const FDebugFieldDrawListEntry& DebugEntry : DrawEntries)
	{
		DebugEntry.DrawFunction();
	}
	
	SlateIM::EndVerticalStack();
	SlateIM::EndBorder();
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

		for (FDebugSection& DebugSection : InTab.DebugSections)
		{
			DrawDebugSection(DebugSection, InDebugSubsystem);		
		}

		SlateIM::EndScrollBox();
	}
	SlateIM::EndTab();
}

////
// 
// FRuntimeIMDebugsDockable
//
///

/*Static variables of FTraeIMDebugExposed initialization of the FName */

const FName FRuntimeIMDebugsDockable::TabId(TEXT("RuntimeDebugWindow"));

TSharedPtr<FRuntimeIMDebugsExposed> FRuntimeIMDebugsDockable::GetRuntimeIMWidget()
{
	return RuntimeIMWidget;
}

const FName FRuntimeIMDebugsDockable::GetTabId()
{
	return TabId;
}

// When the engine module starts, this function is called to register the tab spawner
// with the global tab manager and create an instance of FRuntimeIMDebugsExposed,
// which contains the widget used for debugging. It also registers the command
// used to show and hide the widget.
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

		WindowCommandHandle = URuntimeIMDebugsSubsystem::OnWindowCommand.AddStatic(&FRuntimeIMDebugsDockable::HandleWindowCommand);
	}
}

// When the module is shutting down, the tab spawner is unregistered from the global tab manager,
// and RuntimeIMWidget is reset to release the widget. The command is also unregistered.
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
		
		URuntimeIMDebugsSubsystem::OnWindowCommand.Remove(WindowCommandHandle);
		
		RuntimeIMWidget.Reset();
	}

}

void FRuntimeIMDebugsDockable::RecreateWidget()
{
	RuntimeIMWidget.Reset();

	RuntimeIMWidget = MakeShared<FRuntimeIMDebugsExposed>();
	RuntimeIMWidget->EnableWidget();
}


void FRuntimeIMDebugsDockable::OnStartPIE()
{
	TryToReasignWidgetAsContentToTab();
}

void FRuntimeIMDebugsDockable::HandleWindowCommand(ERuntimeIMDebugWindowCommand InCommand)
{
	switch (InCommand)
	{
	case ERuntimeIMDebugWindowCommand::Show:
		
		TryInvokeTab();
		
		break;
	case ERuntimeIMDebugWindowCommand::Hide:

		RequestCloseTab();

		break;
	case ERuntimeIMDebugWindowCommand::Toggle:

		ToggleTab();

		break;
	default:
		break;
	}

}

// When a PIE session is started, if the current tab content is not assigned to the widget
// created by the plugin (FRuntimeIMDebugsExposed), this function changes the content of the
// SDockTab used by the plugin to point to the correct widget. This can happen when the engine
// is closed while the tab is still attached, and the engine is opened again with the tab
// already existing. 
void FRuntimeIMDebugsDockable::TryToReasignWidgetAsContentToTab()
{
	if (RuntimeIMTab.IsValid() && RuntimeIMTab.Pin().IsValid())
	{
		//UE_LOG(LogRuntimeIMDebugs, Warning, TEXT("Weak pointer to runtime tab is  valid on pie"));

		TSharedRef<SWidget> CurrentRuntimeTabContent = RuntimeIMTab.Pin()->GetContent();

		//UE_LOG(LogRuntimeIMDebugs, Warning, TEXT("ContentTab type: %s"), *CurrentRuntimeTabContent->GetType().ToString());

		check(RuntimeIMWidget.IsValid());

		if (CurrentRuntimeTabContent->GetType() != RuntimeIMWidget->GetExposedWidget()->GetType())
		{
			RuntimeIMTab.Pin()->SetContent(RuntimeIMWidget->GetExposedWidget());
		}

	}
}

void FRuntimeIMDebugsDockable::TryInvokeTab() 
{
	FGlobalTabmanager::Get()->TryInvokeTab(FRuntimeIMDebugsDockable::GetTabId());

}

void FRuntimeIMDebugsDockable::RequestCloseTab()
{
	if (RuntimeIMTab.IsValid() && RuntimeIMTab.Pin().IsValid())
	{
		RuntimeIMTab.Pin()->RequestCloseTab();
	}
}

void FRuntimeIMDebugsDockable::ToggleTab()
{
	if (RuntimeIMTab.IsValid() && RuntimeIMTab.Pin().IsValid())
	{
		RequestCloseTab();
	}
	else
	{
		TryInvokeTab();
	}
}

TSharedRef<SDockTab> FRuntimeIMDebugsDockable::SpawnTab(const FSpawnTabArgs& Args)
{
	check(RuntimeIMWidget.IsValid());

	TSharedRef<SDockTab> Tab = SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			RuntimeIMWidget->GetExposedWidget()
		];

	RuntimeIMTab = Tab;

	return Tab;
}

