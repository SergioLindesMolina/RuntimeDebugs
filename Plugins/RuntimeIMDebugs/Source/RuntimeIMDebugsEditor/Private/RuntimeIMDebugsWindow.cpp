
#include "RuntimeIMDebugsWindow.h"
#include "RuntimeIMDebugsSubsystem.h"

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

void FRuntimeIMDebugsExposed::DrawDebugSection(FDebugSection& InDebugSection, URuntimeIMDebugsSubsystem* InDebugSubsystem)
{	
	SlateIM::AutoSize();
	SlateIM::BeginBorder(
		&InDebugSubsystem->TestBrush,
		Orient_Vertical,
		false);

	// Header
	SlateIM::AutoSize();
	SlateIM::BeginBorder(
		&InDebugSubsystem->TestBrush2,
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
		WindowCommandHandle = URuntimeIMDebugsSubsystem::OnWindowCommand.AddStatic(&FRuntimeIMDebugsDockable::HandleWindowCommand);
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
		//URuntimeIMDebugsSubsystem::OnWindowCommand.Remove(WindowCommandHandle);
		//This causes a crash when the editor is closing - TODO Find the root reason
		//RuntimeIMWidget->DisableWidget();
		
		URuntimeIMDebugsSubsystem::OnWindowCommand.Remove(WindowCommandHandle);
		
		RuntimeIMWidget.Reset();
	}

}

void FRuntimeIMDebugsDockable::HandleWindowCommand(ERuntimeIMDebugWindowCommand InCommand)
{
	TSharedPtr<SDockTab> DockTab;

	switch (InCommand)
	{
	case ERuntimeIMDebugWindowCommand::Show:
		
		FGlobalTabmanager::Get()->TryInvokeTab(FRuntimeIMDebugsDockable::GetTabId());
		
		break;
	case ERuntimeIMDebugWindowCommand::Hide:

		 DockTab = FGlobalTabmanager::Get()->FindExistingLiveTab(FRuntimeIMDebugsDockable::GetTabId());
		
		if (DockTab.IsValid())
		{
			DockTab->RequestCloseTab();
		}

		break;
	case ERuntimeIMDebugWindowCommand::Toggle:

		DockTab = FGlobalTabmanager::Get()->FindExistingLiveTab(FRuntimeIMDebugsDockable::GetTabId());
		
		if (DockTab.IsValid())
		{
			DockTab->RequestCloseTab();
		}
		else
		{
			FGlobalTabmanager::Get()->TryInvokeTab(FRuntimeIMDebugsDockable::GetTabId());
		}

		break;
	default:
		break;
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

