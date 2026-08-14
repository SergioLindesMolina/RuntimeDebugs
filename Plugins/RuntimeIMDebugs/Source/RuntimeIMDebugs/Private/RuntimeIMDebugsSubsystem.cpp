// Fill out your copyright notice in the Description page of Project Settings.

#include "RuntimeIMDebugsSubsystem.h"
#include "RuntimeIMDebugsSettings.h"
#include "RuntimeIMDebugsLog.h"


void URuntimeIMDebugsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	//Initialize the Default tab and Default Section
	const URuntimeIMDebugsSettings* Settings = GetDefault<URuntimeIMDebugsSettings>();

	//Get the user desired name for the 2 default properties
	DefaultTab = Settings->DefaultTabName;
	DefaultSection = Settings->DefaultSectionName;

	AddTab(DefaultTab);
	AddDebugSection(DefaultTab, DefaultSection);
}

void URuntimeIMDebugsSubsystem::Deinitialize()
{
	Tabs.Empty();	

	Super::Deinitialize();
}

void URuntimeIMDebugsSubsystem::ShowWindow()
{
#if WITH_EDITOR
	OnWindowCommand.Broadcast(ERuntimeIMDebugWindowCommand::Show);
#endif
}

void URuntimeIMDebugsSubsystem::HideWindow()
{
#if WITH_EDITOR
	OnWindowCommand.Broadcast(ERuntimeIMDebugWindowCommand::Hide);
#endif
}

void URuntimeIMDebugsSubsystem::ToggleWindow()
{
#if WITH_EDITOR
	OnWindowCommand.Broadcast(ERuntimeIMDebugWindowCommand::Toggle);	
#endif
}

int URuntimeIMDebugsSubsystem::AddTab(const FName InID, const FString& InLabel)
{
	FName ResolvedTabID = ResolveTabID(InID);

	auto FoundTabIndex = Tabs.IndexOfByPredicate([ResolvedTabID](const FDebugTab& InTab)
		{
			return InTab.ID == ResolvedTabID;
		});


	if (FoundTabIndex == INDEX_NONE)
	{
		FString TabLabel = InLabel.IsEmpty() ? ResolvedTabID.ToString() : InLabel;
		return Tabs.Add(FDebugTab(ResolvedTabID, TabLabel));
		
	}
	else
	{
		UE_LOG(LogRuntimeIMDebugs, Error, TEXT("Can't add DebugTab %s with id %s , because is already assigned to another debug tab : %s ")
			,*InLabel ,*ResolvedTabID.ToString(), *Tabs[FoundTabIndex].Label);
		return FoundTabIndex;
	}
}

int URuntimeIMDebugsSubsystem::AddDebugSection(const FName InTabID, const FName InID, const FString& InLabel, int InDrawPriority)
{
	if (FDebugTab * SelectedTab = GetTab(InTabID))
	{
		FName ProcessedSectionID = ResolveDebugSectionID(InID);

		auto FoundDebugSectionIndex = SelectedTab->DebugSections.IndexOfByPredicate([ProcessedSectionID](const FDebugSection& DebugSection) { return DebugSection.Field.ID == ProcessedSectionID; });
		
		if (FoundDebugSectionIndex == INDEX_NONE)
		{
			FString SectionLabel = InLabel.IsEmpty() ? ProcessedSectionID.ToString() : InLabel;
			int CreatedDebugSectionIndex = SelectedTab->DebugSections.Emplace(ProcessedSectionID, SectionLabel, InDrawPriority);

			//Given that tabs only contains one type, DebugSections
			//Always sort the array after adding a new DebugSection to avoid sorting each frame in the draw window

			SelectedTab->DebugSections.StableSort([](const FDebugSection& A, const FDebugSection& B)
				{
					return A.Field.DrawPriority < B.Field.DrawPriority;
				});


			return CreatedDebugSectionIndex;
		}
		else
		{
			UE_LOG(LogRuntimeIMDebugs, Error, TEXT("Can't add the DebugSection %s with id : '%s' , because is already exist in the tab : '%s' ")
				, *InLabel, *InID.ToString(), *InTabID.ToString());
			return FoundDebugSectionIndex;
		}
	}
	else
	{
		return INDEX_NONE;
	}

}

TArray<FDebugTab>& URuntimeIMDebugsSubsystem::GetTabs()
{
	return Tabs;
}


 FDebugTab* URuntimeIMDebugsSubsystem::GetTab(const FName InID)
{
	 FName ResolvedTabID = ResolveTabID(InID);

	 auto FoundTab = Tabs.FindByPredicate([ResolvedTabID](const FDebugTab& InTab) { return InTab.ID == ResolvedTabID; });

	 if (FoundTab) 
	 {
		 return FoundTab;
	 }
	 else
	 {
		 UE_LOG(LogRuntimeIMDebugs, Error, TEXT("Can't found any DebugTab with id %s"), *InID.ToString());
		 return nullptr;
	 }

}

 const FDebugTab* URuntimeIMDebugsSubsystem::GetTab(const FName InID) const
 {
	 FName ResolvedTabID = ResolveTabID(InID);

	 auto FoundTab = Tabs.FindByPredicate([ResolvedTabID](const FDebugTab& InTab) { return InTab.ID == ResolvedTabID; });

	 if (FoundTab) 
	 {
		 return FoundTab;
	 }
	 else
	 {
		 UE_LOG(LogRuntimeIMDebugs, Error, TEXT("Can't found any DebugTab with id %s"), *InID.ToString());
		 return nullptr;
	 }
 }

 FDebugSection* URuntimeIMDebugsSubsystem::GetDebugSection(const FName InTabID, const FName InID)
 {
	 if (FDebugTab* SelectedTab = GetTab(InTabID)) 
	 {
		 FName ProcessedSectionID = ResolveDebugSectionID(InID);

		 auto FoundDebugSection = SelectedTab->DebugSections.FindByPredicate([ProcessedSectionID](FDebugSection& InDebugSection) { return InDebugSection.Field.ID == ProcessedSectionID; });

		 return FoundDebugSection;
	 }
	 else
	 {
		return nullptr;
	 }
 }

 FDebugSection* URuntimeIMDebugsSubsystem::GetDebugSection(FDebugTab* InTab, const FName InID)
 {
	 if (InTab)
	 {
		 FName ProcessedSectionID = ResolveDebugSectionID(InID);

		 auto FoundDebugSection = InTab->DebugSections.FindByPredicate([ProcessedSectionID](const FDebugSection& InDebugSection) { return InDebugSection.Field.ID == ProcessedSectionID; });

		 return FoundDebugSection;
	 }
	 else
	 {
		 return nullptr;
	 }
 }

 const FDebugSection* URuntimeIMDebugsSubsystem::GetDebugSection(const FName InTabID, const FName InID) const
 {
	 if (const FDebugTab* SelectedTab = GetTab(InTabID))
	 {
		 FName ProcessedSectionID = ResolveDebugSectionID(InID);

		 auto FoundDebugSection = SelectedTab->DebugSections.FindByPredicate([ProcessedSectionID](const FDebugSection& InDebugSection) { return InDebugSection.Field.ID == ProcessedSectionID; });

		 return FoundDebugSection;
	 }
	 else
	 {
		 return nullptr;
	 }
 }

 FDebugTab* URuntimeIMDebugsSubsystem::GetOrCreateTab(const FName InTabID)
 {
	 //Check if the selected tab exist or if the passed id is empty in which case the default tab will be used
	 if (FDebugTab* SelectedTab = GetTab(InTabID))
	 {	
		 return SelectedTab;
	 }
	 else
	 {
		 //If the selected tab does not exist we create the tab
		 int CreatedTabIndex = AddTab(InTabID, InTabID.ToString());
		 return &Tabs[CreatedTabIndex];
	 }
 }

 FDebugSection* URuntimeIMDebugsSubsystem::GetOrCreateDebugSection(const FName InTabID, const FName InSectionID)
 {
	 //We need a valid tab to add or find a section
	 FDebugTab* SelectedTab = GetOrCreateTab(InTabID);
	 if (!SelectedTab) 
	 {
		return nullptr;
	 }
	 


	 //Check if the selected Debug section exist or if the passed debug section id is empty in wich case the default section will be used 
	 if (FDebugSection* SelectedSection =  GetDebugSection(SelectedTab, InSectionID))
	 {
		 return SelectedSection;
	 }
	 else
	 {
		 int CreatedSectionIndex = AddDebugSection(InTabID, InSectionID);
		 return &SelectedTab->DebugSections[CreatedSectionIndex];
	 }
 }

 const FName URuntimeIMDebugsSubsystem::ResolveTabID(const FName InTabID) const
 {
	 return InTabID.IsNone() ? DefaultTab : InTabID;
 }

 const FName URuntimeIMDebugsSubsystem::ResolveDebugSectionID(const FName InSectionID) const
 {
	 return InSectionID.IsNone() ? DefaultSection : InSectionID;
 }

void URuntimeIMDebugsSubsystem::AddButton(const FName InTabID, const FName InSectionID, const FName InID, const FString& InLabel, int InDrawPriority)
{	
	if (FDebugSection* SelectedSection = GetOrCreateDebugSection(InTabID, InSectionID)) 
	{
		auto FoundButton = SelectedSection->Buttons.FindByPredicate([InID](const FDebugButton& Button) { return Button.Field.ID == InID; });
		
		if (FoundButton)
		{
			UE_LOG(LogRuntimeIMDebugs, Error,
				TEXT("Can't add the DebugButton %s with id : '%s' , because is already assigned to the DebugButton : '%s' ")
				, *InLabel, *InID.ToString(), *FoundButton->Label);
		}
		else
		{
			FString ButtonLabel = InLabel.IsEmpty() ? InID.ToString() : InLabel;
			SelectedSection->Buttons.Emplace(InID, ButtonLabel, InDrawPriority);
		}
	}
	else
	{
		UE_LOG(LogRuntimeIMDebugs, Error, TEXT("Error tring to create the Button with id %s"), *InID.ToString());
	}		
	
}


void URuntimeIMDebugsSubsystem::AddToggle(const FName InTabID, const FName InSectionID, const FName InID, const FString& InLabel, bool InValue, int InDrawPriority)
{
	if (FDebugSection* SelectedSection = GetOrCreateDebugSection(InTabID, InSectionID))
	{
		auto FoundToggle = SelectedSection->Toggles.FindByPredicate([InID](const FDebugToggle& Toggle) { return Toggle.Field.ID == InID; });

		if (FoundToggle)
		{
			UE_LOG(LogRuntimeIMDebugs, Error, TEXT("Can't add the DebugToggle %s with id : '%s' , because is already assigned to the DebugToggle : '%s' ")
				,*InLabel ,*InID.ToString() ,*FoundToggle->Label);
		}
		else
		{
			FString ToggleLabel = InLabel.IsEmpty() ? InID.ToString() : InLabel;
			SelectedSection->Toggles.Emplace(InID, ToggleLabel, InValue, InDrawPriority);
		}
	}
}

bool URuntimeIMDebugsSubsystem::GetToggleState(const FName InTabID, const FName InSectionID, const FName InID) const
{
	if (const FDebugSection* SelectedSection = GetDebugSection(InTabID, InSectionID))
	{
		auto FoundToggle = SelectedSection->Toggles.FindByPredicate([InID](const FDebugToggle& InToggle)
			{ return InToggle.Field.ID == InID;});
		
		if (FoundToggle)
		{
			return FoundToggle->Value;
		}
		else
		{
			UE_LOG(LogRuntimeIMDebugs, Error, TEXT("Can't get toggle value, the id : '%s' , is not assigned to any FDebugToggle"),
				*InID.ToString());
			return false;
		}
	}
	else
	{
		return false;
	}	
}

void URuntimeIMDebugsSubsystem::SetToggleState(const FName InTabID, const FName InSectionID, const FName InID, bool InValue)
{
	if (FDebugSection* SelectedSection = GetDebugSection(InTabID, InSectionID))
	{
		auto FoundToggle = SelectedSection->Toggles.FindByPredicate([InID](const FDebugToggle& InToggle)
			{ return InToggle.Field.ID == InID;});
		
		if (FoundToggle)
		{
			FoundToggle->Value = InValue;
		}
		else
		{
			UE_LOG(LogRuntimeIMDebugs, Error, TEXT("Can't set toggle value the id : '%s' , is not assigned to any FDebugToggle"),
				*InID.ToString());
		}
	}

}

//TODO ADD comprobation to check if already exist a toggle with that id and returns value to know of succes 
void URuntimeIMDebugsSubsystem::AddSpinBox(const FName InTabID, const FName InSectionID, const FName InID, const FString& InLabel, float InValue ,
	float InMin, float InMax, int InDrawPriority)
{
	if (FDebugSection* SelectedSection = GetOrCreateDebugSection(InTabID, InSectionID))
	{
		auto FoundSpinBox = SelectedSection->SpinBoxes.FindByPredicate([InID](const FDebugSpinBox& InSpinBox) { return InSpinBox.Field.ID == InID; });

		if (FoundSpinBox)
		{
			UE_LOG(LogRuntimeIMDebugs, Error, TEXT("Can't add the DebugSpinBox %s with id : '%s' , because is already assigned to the DebugSpinBox : '%s' ")
				, *InLabel, *InID.ToString(), *FoundSpinBox->Label);
		}
		else
		{			
			FString SpinBoxLabel = InLabel.IsEmpty() ? InID.ToString() : InLabel;
			SelectedSection->SpinBoxes.Emplace(InID, SpinBoxLabel, InValue, InMin, InMax, InDrawPriority);
		}		
	}
}

float URuntimeIMDebugsSubsystem::GetSpinBoxValue(const FName InTabID, const FName InSectionID, const FName InID) const
{
	if (const FDebugSection* SelectedSection = GetDebugSection(InTabID, InSectionID))
	{
		auto FoundSpinBox = SelectedSection->SpinBoxes.FindByPredicate([InID](const FDebugSpinBox& InSpinBox)
			{ return InSpinBox.Field.ID == InID;});
		
		if (FoundSpinBox)
		{
			return FoundSpinBox->Value;
		}
		else
		{
			UE_LOG(LogRuntimeIMDebugs, Error, TEXT("Can't get SpinBox value the id : '%s' , is not assigned to any FDebugSpinBox"),
				*InID.ToString());
			return 0.0f;
		}
	}
	else
	{
		return 0.0f;
	}
}

void URuntimeIMDebugsSubsystem::SetSpinBoxValue(const FName InTabID, const FName InSectionID, const FName InID, float InValue)
{
	if (FDebugSection* SelectedSection = GetDebugSection(InTabID, InSectionID))
	{
		auto FoundSpinBox = SelectedSection->SpinBoxes.FindByPredicate([InID](const FDebugSpinBox& InSpinBox)
			{ return InSpinBox.Field.ID == InID;});
		
		if (FoundSpinBox)
		{
			FoundSpinBox->Value = InValue;
		}
		else
		{
			UE_LOG(LogRuntimeIMDebugs, Error, TEXT("Can't set SpinBox value the id : '%s' , is not assigned to any FDebugSpinBox"),
				*InID.ToString());
		}
	}
}

void URuntimeIMDebugsSubsystem::AddFloatField(const FName InTabID, const FName InSectionID, const FName InID, const FString& InLabel, float InValue, int InDrawPriority)
{
	if (FDebugSection* SelectedSection = GetOrCreateDebugSection(InTabID, InSectionID))
	{
		auto FoundFloatField = SelectedSection->FloatFields.FindByPredicate([InID](const FDebugFloatField& FloatField)
			{ return FloatField.Field.ID == InID; });

		if (FoundFloatField)
		{
			UE_LOG(LogRuntimeIMDebugs, Error, TEXT("Can't add the DebugFloatField %s with id : '%s' , because is already assigned to the DebugFloatField : '%s' ")
				, *InLabel, *InID.ToString(), *FoundFloatField->Label);
		}
		else
		{
			FString FloatLabel = InLabel.IsEmpty() ? InID.ToString() : InLabel;
			SelectedSection->FloatFields.Emplace(InID, FloatLabel, FString::SanitizeFloat(InValue), InValue, InDrawPriority);
		}		
	}
}

float URuntimeIMDebugsSubsystem::GetFloatFieldValue(const FName InTabID, const FName InSectionID, const FName InID) const
{
	if (const FDebugSection* SelectedSection = GetDebugSection(InTabID, InSectionID))
	{
		auto FoundFloatField = SelectedSection->FloatFields.FindByPredicate([InID](const FDebugFloatField& InFloatField)
			{ return InFloatField.Field.ID == InID; });
		
		if (FoundFloatField)
		{
			return FoundFloatField->Value;
		}
		else
		{
			UE_LOG(LogRuntimeIMDebugs, Error, TEXT("Can't get FloatField value the id : '%s' , is not assigned to any FDebugFloatField"),
				*InID.ToString());
			return 0.0f;
		}
	}
	else
	{
		return 0.0f;
	}
}

void URuntimeIMDebugsSubsystem::SetFloatFieldValue(const FName InTabID, const FName InSectionID, const FName InID, float InValue)
{
	if (FDebugSection* SelectedSection = GetDebugSection(InTabID, InSectionID))
	{
		auto FoundFloatField = SelectedSection->FloatFields.FindByPredicate([InID](const FDebugFloatField& InFloatField)
			{ return InFloatField.Field.ID == InID; });

		if (FoundFloatField)
		{
			FoundFloatField->Value = InValue;
			FoundFloatField->CachedStringAsValue = FString::SanitizeFloat(InValue);
		}
		else
		{
			UE_LOG(LogRuntimeIMDebugs, Error, TEXT("Can't set FloatField value the id : '%s' , is not assigned to any FDebugFloatField"),
				*InID.ToString());
		}
	}
}

void URuntimeIMDebugsSubsystem::AddComboBox(const FName InTabID, const FName InSectionID, const FName InID, const FString& InLabel, const TArray<FString>& InOptions, int InIndex, int InDrawPriority)
{
	if (FDebugSection* SelectedSection = GetOrCreateDebugSection(InTabID, InSectionID))
	{
		auto FoundComboBox = SelectedSection->ComboBoxes.FindByPredicate([InID](const FDebugComboBox& InComboBox)
			{ return InComboBox.Field.ID == InID; });

		if (FoundComboBox)
		{
			UE_LOG(LogRuntimeIMDebugs, Error, TEXT("Can't add the DebugComboBox %s with id : '%s' , because is already assigned to the DebugComboBox : '%s' ")
				, *InLabel, *InID.ToString(), *FoundComboBox->Label);
		}
		else
		{
			FString ComboBoxLabel = InLabel.IsEmpty() ? InID.ToString() : InLabel;
			SelectedSection->ComboBoxes.Emplace(InID, ComboBoxLabel, InOptions, InIndex, InDrawPriority);
		}
	}
}

int URuntimeIMDebugsSubsystem::GetComboBoxIndex(const FName InTabID, const FName InSectionID, const FName InID) const
{
	if (const FDebugSection* SelectedSection = GetDebugSection(InTabID, InSectionID))
	{
		auto FoundComboBox = SelectedSection->ComboBoxes.FindByPredicate([InID](const FDebugComboBox& InComboBox)
			{ return InComboBox.Field.ID == InID; });

		if (FoundComboBox)
		{
			return FoundComboBox->Index;
		}
		else
		{
			UE_LOG(LogRuntimeIMDebugs, Error, TEXT("Can't get ComboBox indes the id : '%s' , is not assigned to any FDebugComboBox"),
				*InID.ToString());
			return -1;
		}
	}
	else
	{
		return -1;
	}
}

void URuntimeIMDebugsSubsystem::SetComboBoxIndex(const FName InTabID, const FName InSectionID, const FName InID, int InIndex)
{
	if (FDebugSection* SelectedSection = GetDebugSection(InTabID, InSectionID))
	{
		auto FoundComboBox = SelectedSection->ComboBoxes.FindByPredicate([InID](const FDebugComboBox& InComboBox)
			{ return InComboBox.Field.ID == InID; });

		if (FoundComboBox)
		{
			FoundComboBox->Index = InIndex;
		}
		else
		{
			UE_LOG(LogRuntimeIMDebugs, Error, TEXT("Can't set ComboBox index the id : '%s' , is not assigned to any FDebugComboBox"),
				*InID.ToString());
		}
	}
}

void URuntimeIMDebugsSubsystem::AddText(const FName InTabID, const FName InSectionID, const FName InID, const FString& InLabel, const FString& InText, int InDrawPriority)
{
	if (FDebugSection* SelectedSection = GetOrCreateDebugSection(InTabID, InSectionID))
	{
		auto FoundTextField = SelectedSection->TextFields.FindByPredicate([InID](const FDebugTextField& InTextField)
			{ return InTextField.Field.ID == InID; });

		if (FoundTextField)
		{
			UE_LOG(LogRuntimeIMDebugs, Error, TEXT("Can't add the DebugTextField %s with id : '%s' , because is already assigned to the DebugTextField : '%s' ")
				, *InLabel, *InID.ToString(), *FoundTextField->Label);
		}
		else
		{
			FString TextFieldLabel = InLabel.IsEmpty() ? InID.ToString() : InLabel;
			SelectedSection->TextFields.Emplace(InID, TextFieldLabel, InText, InDrawPriority);
		}
	}
}

FString URuntimeIMDebugsSubsystem::GetText(const FName InTabID, const FName InSectionID, const FName InID) const
{
	if (const FDebugSection* SelectedSection = GetDebugSection(InTabID, InSectionID))
	{
		auto FoundTextField = SelectedSection->TextFields.FindByPredicate([InID](const FDebugTextField& InTextField)
			{ return InTextField.Field.ID == InID; });

		if (FoundTextField)
		{
			return FoundTextField->Text;
		}
		else
		{
			UE_LOG(LogRuntimeIMDebugs, Error, TEXT("Can't get TextField indes the id : '%s' , is not assigned to any FDebugTextField"),
				*InID.ToString());
			return "";
		}
	}
	else
	{
		return "";
	}
}

void URuntimeIMDebugsSubsystem::SetText(const FName InTabID, const FName InSectionID, const FName InID, const FString& InText)
{
	if (FDebugSection* SelectedSection = GetDebugSection(InTabID, InSectionID))
	{
		auto FoundTextField = SelectedSection->TextFields.FindByPredicate([InID](const FDebugTextField& InTextField)
			{ return InTextField.Field.ID == InID; });

		if (FoundTextField)
		{
			FoundTextField->Text = InText;
		}
		else
		{
			UE_LOG(LogRuntimeIMDebugs, Error, TEXT("Can't set TextField index the id : '%s' , is not assigned to any FDebugTextField"),
				*InID.ToString());
		}
	}
}
