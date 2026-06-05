// Fill out your copyright notice in the Description page of Project Settings.

#include "RuntimeIMDebugsSubsystem.h"
#include "RuntimeIMDebugsWindow.h"
#include "RuntimeIMDebugsLog.h"

void URuntimeIMDebugsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	//Initial Default tab
	AddTab(DefaultTab, DefaultTab.ToString());
}

void URuntimeIMDebugsSubsystem::Deinitialize()
{
	Tabs.Empty();
	Super::Deinitialize();
}

void URuntimeIMDebugsSubsystem::ShowWindow()
{
#if WITH_EDITOR
	FGlobalTabmanager::Get()->TryInvokeTab(FRuntimeIMDebugsDockable::GetTabId());
#endif
}

void URuntimeIMDebugsSubsystem::HideWindow()
{
#if WITH_EDITOR
	TSharedPtr<SDockTab> DockTab = FGlobalTabmanager::Get()->FindExistingLiveTab(FRuntimeIMDebugsDockable::GetTabId());
	if (DockTab.IsValid()) 
	{
		DockTab->RequestCloseTab();
	}	
#endif
}

void URuntimeIMDebugsSubsystem::ToggleWindow()
{
#if WITH_EDITOR
	TSharedPtr<SDockTab> DockTab = FGlobalTabmanager::Get()->FindExistingLiveTab(FRuntimeIMDebugsDockable::GetTabId());
	if (DockTab.IsValid())
	{
		DockTab->RequestCloseTab();
	}
	else
	{
		FGlobalTabmanager::Get()->TryInvokeTab(FRuntimeIMDebugsDockable::GetTabId());
	}
#endif
}

void URuntimeIMDebugsSubsystem::AddTab(const FName InID, const FString& InLabel)
{
	auto FoundTab = Tabs.FindByPredicate([InID](const FDebugTab& InTab)
		{
			return InTab.ID == InID;
		});

	if (!FoundTab)
	{
		FString TabLabel = InLabel.IsEmpty() ? InID.ToString() : InLabel;
		Tabs.Add(FDebugTab(InID, TabLabel));
	}
	else
	{
		UE_LOG(LogRuntimeIMDebugs, Error, TEXT("Can't add DebugTab %s with id %s , because is already assigned to another debug tab : %s ")
			,*InLabel ,*InID.ToString(), *FoundTab->Label);
	}
}

TArray<FDebugTab>& URuntimeIMDebugsSubsystem::GetTabs()
{
	return Tabs;
}

 FDebugTab* URuntimeIMDebugsSubsystem::GetTab(const FName InID)
{
	 auto FoundTab = Tabs.FindByPredicate([InID](const FDebugTab& InTab) { return InTab.ID == InID; });

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
	 auto FoundTab = Tabs.FindByPredicate([InID](const FDebugTab& InTab) { return InTab.ID == InID; });

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
	


void URuntimeIMDebugsSubsystem::AddButton(const FName InTabID, const FName InID, const FString& InLabel, int InDrawPriority)
{
	if ( FDebugTab* SelectedTab = GetTab(InTabID)) 
	{
		auto FoundButton = SelectedTab->Buttons.FindByPredicate([InID](const FDebugButton& Button) { return Button.Field.ID == InID; });

		if (FoundButton)
		{
			UE_LOG(LogRuntimeIMDebugs, Error, 
				TEXT("Can't add the DebugButton %s with id : '%s' , because is already assigned to the DebugButton : '%s' ")
				,*InLabel ,*InID.ToString() ,*FoundButton->Label);
		}
		else
		{
			FString ButtonLabel = InLabel.IsEmpty() ? InID.ToString() : InLabel;
			SelectedTab->Buttons.Emplace(InID, ButtonLabel, InDrawPriority);
		}		
	}

}

void URuntimeIMDebugsSubsystem::AddToggle(const FName InTabID, const FName InID, const FString& InLabel, bool InValue, int InDrawPriority)
{
	if (FDebugTab* SelectedTab = GetTab(InTabID))
	{
		auto FoundToggle = SelectedTab->Toggles.FindByPredicate([InID](const FDebugToggle& Toggle) { return Toggle.Field.ID == InID; });

		if (FoundToggle)
		{
			UE_LOG(LogRuntimeIMDebugs, Error, TEXT("Can't add the DebugToggle %s with id : '%s' , because is already assigned to the DebugToggle : '%s' ")
				,*InLabel ,*InID.ToString() ,*FoundToggle->Label);
		}
		else
		{
			FString ToggleLabel = InLabel.IsEmpty() ? InID.ToString() : InLabel;
			SelectedTab->Toggles.Emplace(InID, ToggleLabel, InValue, InDrawPriority);
		}
	}
}

bool URuntimeIMDebugsSubsystem::GetToggleState(const FName InTabID, const FName InID) const
{
	if (const FDebugTab* SelectedTab = GetTab(InTabID))
	{
		auto FoundToggle = SelectedTab->Toggles.FindByPredicate([InID](const FDebugToggle& InToggle) 
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

void URuntimeIMDebugsSubsystem::SetToggleState(const FName InTabID, const FName InID, bool InValue)
{
	if (FDebugTab* SelectedTab = GetTab(InTabID))
	{
		auto FoundToggle = SelectedTab->Toggles.FindByPredicate([InID](const FDebugToggle& InToggle) 
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
void URuntimeIMDebugsSubsystem::AddSlider(const FName InTabID, const FName InID, const FString& InLabel, float InValue, int InDrawPriority)
{
	if (FDebugTab* SelectedTab = GetTab(InTabID))
	{
		auto FoundSlider = SelectedTab->Sliders.FindByPredicate([InID](const FDebugSlider& Slider) { return Slider.Field.ID == InID; });

		if (FoundSlider)
		{
			UE_LOG(LogRuntimeIMDebugs, Error, TEXT("Can't add the DebugSlider %s with id : '%s' , because is already assigned to the DebugSlider : '%s' ")
				, *InLabel, *InID.ToString(), *FoundSlider->Label);
		}
		else
		{
			FString SliderLabel = InLabel.IsEmpty() ? InID.ToString() : InLabel;
			SelectedTab->Sliders.Emplace(InID, SliderLabel, InValue, InDrawPriority);
		}		
	}
}

float URuntimeIMDebugsSubsystem::GetSliderValue(const FName InTabID, const FName InID) const
{
	if (const FDebugTab* SelectedTab = GetTab(InTabID))
	{
		auto FoundSlider = SelectedTab->Sliders.FindByPredicate([InID](const FDebugSlider& InSlider) 
			{ return InSlider.Field.ID == InID;});
		
		if (FoundSlider)
		{
			return FoundSlider->Value;
		}
		else
		{
			UE_LOG(LogRuntimeIMDebugs, Error, TEXT("Can't get slider value the id : '%s' , is not assigned to any FDebugSlider"),
				*InID.ToString());
			return 0.0f;
		}
	}
	else
	{
		return 0.0f;
	}
}

void URuntimeIMDebugsSubsystem::SetSliderValue(const FName InTabID, const FName InID, float InValue)
{
	if (FDebugTab* SelectedTab = GetTab(InTabID))
	{
		auto FoundSlider = SelectedTab->Sliders.FindByPredicate([InID](const FDebugSlider& InSlider) 
			{ return InSlider.Field.ID == InID;});
		
		if (FoundSlider)
		{
			FoundSlider->Value = InValue;
		}
		else
		{
			UE_LOG(LogRuntimeIMDebugs, Error, TEXT("Can't set slider value the id : '%s' , is not assigned to any FDebugSlider"),
				*InID.ToString());
		}
	}
}

void URuntimeIMDebugsSubsystem::AddFloatField(const FName InTabID, const FName InID, const FString& InLabel, float InValue, int InDrawPriority)
{
	if (FDebugTab* SelectedTab = GetTab(InTabID))
	{
		auto FoundFloatField = SelectedTab->FloatFields.FindByPredicate([InID](const FDebugFloatField& FloatField) 
			{ return FloatField.Field.ID == InID; });

		if (FoundFloatField)
		{
			UE_LOG(LogRuntimeIMDebugs, Error, TEXT("Can't add the DebugFloatField %s with id : '%s' , because is already assigned to the DebugFloatField : '%s' ")
				, *InLabel, *InID.ToString(), *FoundFloatField->Label);
		}
		else
		{
			FString FloatLabel = InLabel.IsEmpty() ? InID.ToString() : InLabel;
			SelectedTab->FloatFields.Emplace(InID, FloatLabel, FString::SanitizeFloat(InValue), InValue, InDrawPriority);
		}		
	}
}

float URuntimeIMDebugsSubsystem::GetFloatFieldValue(const FName InTabID, const FName InID) const
{
	if (const FDebugTab* SelectedTab = GetTab(InTabID))
	{
		auto FoundFloatField = SelectedTab->FloatFields.FindByPredicate([InID](const FDebugFloatField& InFloatField)
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

void URuntimeIMDebugsSubsystem::SetFloatFieldValue(const FName InTabID, const FName InID, float InValue)
{
	if (FDebugTab* SelectedTab = GetTab(InTabID))
	{
		auto FoundFloatField = SelectedTab->FloatFields.FindByPredicate([InID](const FDebugFloatField& InFloatField)
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
