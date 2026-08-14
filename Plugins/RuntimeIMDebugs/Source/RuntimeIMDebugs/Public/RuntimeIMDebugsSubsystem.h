// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "RuntimeIMDebugsSubsystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDebugButtonPressed, const FName, ID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDebugToggleChanged, const FName, ID, bool, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDebugSpinBoxChanged, const FName, ID, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDebugFloatFieldChanged, const FName, ID, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDebugComboBoxChanged, const FName, ID, int, Index);

enum class ERuntimeIMDebugWindowCommand : uint8
{
    Show,
    Hide,
    Toggle
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnWindowCommand, ERuntimeIMDebugWindowCommand);

//Used to order the difrent field given the draw priority 
struct FDebugFieldDrawListEntry 
{
    FDebugFieldDrawListEntry() : DrawPriority(0) {}
    FDebugFieldDrawListEntry(
        int InDrawPriority, 
        TFunction<void()> InDrawFunction)
        : DrawPriority(InDrawPriority), 
        DrawFunction(MoveTemp(InDrawFunction))
    {}

    int32 DrawPriority;

    TFunction<void()> DrawFunction;
};

USTRUCT(BlueprintType)
struct FDebugField
{
    GENERATED_BODY()

    FDebugField() 
        : ID(NAME_None), 
        DrawPriority(0) 
    {}
    
    FDebugField(
        FName InID, 
        int InDrawPriority = 0) 
        : ID(InID), 
        DrawPriority(InDrawPriority) 
    {}

    /*Must be unique*/
    UPROPERTY(BlueprintReadOnly, Category = "DebugField")
    FName ID;

    UPROPERTY(BlueprintReadOnly, Category = "DebugField")
    int DrawPriority;
};

USTRUCT(BlueprintType)
struct FDebugTextField 
{
    GENERATED_BODY()

    FDebugTextField()
        : Field(NAME_None, 0)
        , Label("")
        , Text("")
    {
    }

    FDebugTextField(FName InID, const FString& InLable, const FString& InText, int InDrawPriority)
        : Field(InID, InDrawPriority)
        , Label(InLable)
        , Text(InText)
    {
    }

    UPROPERTY(BlueprintReadOnly, Category = "DebugTextField")
    FDebugField Field;

    UPROPERTY(BlueprintReadOnly, Category = "DebugTextField")
    FString Label;

    UPROPERTY(BlueprintReadOnly, Category = "DebugTextField")
    FString Text;
};

USTRUCT(BlueprintType)
struct FDebugFloatField 
{
    GENERATED_BODY()

    FDebugFloatField()
        : Field(NAME_None, 0)
        , Label("")
        , CachedStringAsValue(FString::SanitizeFloat(0))
        , Value(0.f)
    {
    }

    FDebugFloatField(FName InID, const FString& InLable, const FString& InCachedString, float InValue, int InDrawPriority)
        : Field(InID, InDrawPriority)
        , Label(InLable)
        , CachedStringAsValue(InCachedString)
        , Value(InValue)
    {
    }

    UPROPERTY(BlueprintReadOnly, Category = "DebugFloatField")
    FDebugField Field;

    UPROPERTY(BlueprintReadOnly, Category = "DebugFloatField")
    FString Label;

    UPROPERTY(BlueprintReadOnly, Category = "DebugFloatField")
    FString CachedStringAsValue;

    UPROPERTY(BlueprintReadOnly, Category = "DebugFloatField")
    float Value;
};

USTRUCT(BlueprintType)
struct FDebugComboBox
{
    GENERATED_BODY()

    FDebugComboBox()
        : Field(NAME_None, 0)
        , Label("")
        , Options(TArray<FString>{})
        , Index(0)
    {
    }

    FDebugComboBox(FName InID, const FString& InLable, const TArray<FString>& InOptions, int InIndex, int InDrawPriority)
        : Field(InID, InDrawPriority)
        , Label(InLable)
        , Options(InOptions)
        , Index(InIndex)
    {
    }

    UPROPERTY(BlueprintReadOnly, Category = "DebugComboBox")
    FDebugField Field;

    UPROPERTY(BlueprintReadOnly, Category = "DebugComboBox")
    FString Label;

    UPROPERTY(BlueprintReadOnly, Category = "DebugComboBox")
    TArray<FString> Options;

    UPROPERTY(BlueprintReadOnly, Category = "DebugComboBox")
    int Index;
};

USTRUCT(BlueprintType)
struct FDebugSpinBox 
{
    GENERATED_BODY()

    FDebugSpinBox()
        : Field(NAME_None, 0)
        , Label("")
        , Value(0.f)
        , Min(0.f)
        , Max(1.0f)
    {
    }

    FDebugSpinBox(FName InID, const FString& InLable, float InValue, float InMin, float InMax, int InDrawPriority)
        : Field(InID, InDrawPriority)
        , Label(InLable)
        , Value(InValue)
        , Min(InMin)
        , Max(InMax)
    {
    }

    UPROPERTY(BlueprintReadOnly, Category = "DebugSpinBox")
    FDebugField Field;

    UPROPERTY(BlueprintReadOnly, Category = "DebugSpinBox")
    FString Label;

    UPROPERTY(BlueprintReadOnly, Category = "DebugSpinBox")
    float Value;

    UPROPERTY(BlueprintReadOnly, Category = "DebugSpinBox")
    float Min;
    
    UPROPERTY(BlueprintReadOnly, Category = "DebugSpinBox")
    float Max;
};

USTRUCT(BlueprintType)
struct FDebugToggle
{
    GENERATED_BODY()

    FDebugToggle()
        : Field(NAME_None, 0)
        , Label("")
        , Value(false)
    {
    }

    FDebugToggle(FName InID, const FString& InLable, bool InValue, int InDrawPriority)
        : Field(InID, InDrawPriority)
        , Label(InLable)
        , Value(InValue)
    {
    }

    UPROPERTY(BlueprintReadOnly, Category = "DebugToggle")
    FDebugField Field;

    UPROPERTY(BlueprintReadOnly, Category = "DebugToggle")
    FString Label;

    UPROPERTY(BlueprintReadOnly, Category = "DebugToggle")
    bool Value;

};

USTRUCT(BlueprintType)
struct FDebugButton 
{
    GENERATED_BODY()

    FDebugButton()
        : Field(NAME_None, 0)
        , Label("")
    {
    }

    FDebugButton(FName InID, const FString& InLable, int InDrawPriority)
        : Field(InID, InDrawPriority)
        , Label(InLable)
    {
    }

    UPROPERTY(BlueprintReadOnly, Category = "DebugButton")
    FDebugField Field;

    UPROPERTY(BlueprintReadOnly, Category = "DebugButton")
    FString Label;

};

USTRUCT(BlueprintType)
struct FDebugSection
{
    GENERATED_BODY()

    FDebugSection()
        : Field(NAME_None, 0),
        Label("Default")
    {
    }

    FDebugSection(FName InID, const FString InLabel, int InDrawPriority)
        : Field(InID, InDrawPriority),
        Label(InLabel)
    {
    }


    UPROPERTY(BlueprintReadOnly, Category = "DebugSection")
    FDebugField Field;

    UPROPERTY(BlueprintReadOnly, Category = "DebugSection")
    FString Label;

    UPROPERTY(BlueprintReadOnly, Category = "DebugSection")
    TArray<FDebugButton> Buttons;
    UPROPERTY(BlueprintReadOnly, Category = "DebugSection")
    TArray<FDebugToggle> Toggles;
    UPROPERTY(BlueprintReadOnly, Category = "DebugSection")
    TArray<FDebugSpinBox> SpinBoxes;
    UPROPERTY(BlueprintReadOnly, Category = "DebugSection")
    TArray<FDebugFloatField> FloatFields;
    UPROPERTY(BlueprintReadOnly, Category = "DebugSection")
    TArray<FDebugComboBox> ComboBoxes;
    UPROPERTY(BlueprintReadOnly, Category = "DebugSection")
    TArray<FDebugTextField> TextFields;

};

USTRUCT(BlueprintType)
struct FDebugTab
{
    GENERATED_BODY()

    FDebugTab() 
        : ID(NAME_None), 
        Label("") 
    {}
    
    FDebugTab(FName InID, const FString& InLable) 
        : ID(InID), 
        Label(InLable)
    {}

    /*Must be unique*/
    UPROPERTY(BlueprintReadOnly, Category = "DebugTab")
    FName ID;

    UPROPERTY(BlueprintReadOnly, Category = "DebugTab")
    FString Label;

    UPROPERTY(BlueprintReadOnly, Category = "DebugTab")
    TArray<FDebugSection> DebugSections;

};


/**
 * 
 */
UCLASS()
class RUNTIMEIMDEBUGS_API URuntimeIMDebugsSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:

    //Delegate to comunicate with the editor window to open and close the window
    inline static FOnWindowCommand OnWindowCommand;

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem")
    void ShowWindow();

    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem")
    void HideWindow();

    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem")
    void ToggleWindow();

    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem")
    int AddTab(const FName InID, const FString& InLabel = TEXT(""));

    TArray<FDebugTab>& GetTabs();

    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem")
    int AddDebugSection(const FName InTabID, const FName InID, const FString& InLabel = TEXT(""), int InDrawPriority = 0);

    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem")
    void AddButton(const FName InTabID, const FName InSectionID, const FName InID, const FString& InLabel = TEXT(""), int InDrawPriority = 0);

    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem")
    void AddToggle(const FName InTabID, const FName InSectionID, const FName InID, const FString& InLabel = TEXT(""), bool InValue = false, int InDrawPriority = 0);
    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem")
    bool GetToggleState(const FName InTabID, const FName InSectionID, const FName InID) const;
    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem")
    void SetToggleState(const FName InTabID, const FName InSectionID, const FName InID, bool InValue);
   
    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem")
    void AddSpinBox(const FName InTabID, const FName InSectionID, const FName InID, const FString& InLabel = TEXT(""), float InValue = 0.f,
        float InMin = 0.0f, float InMax = 1.0f, int InDrawPriority = 0);
    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem")
    float GetSpinBoxValue(const FName InTabID, const FName InSectionID, const FName InID) const;
    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem")
    void SetSpinBoxValue(const FName InTabID, const FName InSectionID, const FName InID, float InValue);

    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem")
    void AddFloatField(const FName InTabID , const FName InSectionID, const FName InID, const FString& InLabel = TEXT(""), float InValue = 0.f, int InDrawPriority = 0);
    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem")
    float GetFloatFieldValue(const FName InTabID, const FName InSectionID, const FName InID) const;
    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem")
    void SetFloatFieldValue(const FName InTabID, const FName InSectionID, const FName InID, float InValue);

    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem")
    void AddComboBox(const FName InTabID, const FName InSectionID, const FName InID, const FString& InLabel, const TArray<FString>& InOptions, int InIndex = 0, int InDrawPriority = 0);
    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem")
    int GetComboBoxIndex(const FName InTabID, const FName InSectionID, const FName InID) const;
    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem")
    void SetComboBoxIndex(const FName InTabID, const FName InSectionID, const FName InID, int InIndex);

    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem")
    void AddText(const FName InTabID, const FName InSectionID, const FName InID, const FString& InLabel = TEXT(""), const FString& InText = "", int InDrawPriority = 0);

    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem")
    FString GetText(const FName InTabID, const FName InSectionID, const FName InID) const;

    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem")
    void SetText(const FName InTabID, const FName InSectionID, const FName InID, const FString& InText);


    UPROPERTY(BlueprintAssignable, Category = "RuntimeDebugsSubsystem")
    FOnDebugButtonPressed OnButtonPressed;
    UPROPERTY(BlueprintAssignable, Category = "RuntimeDebugsSubsystem")
    FOnDebugToggleChanged OnToggleChanged;
    UPROPERTY(BlueprintAssignable, Category = "RuntimeDebugsSubsystem")
    FOnDebugSpinBoxChanged OnSpinBoxChanged;
    UPROPERTY(BlueprintAssignable, Category = "RuntimeDebugsSubsystem")
    FOnDebugFloatFieldChanged OnDebugFloatFieldChanged;
    UPROPERTY(BlueprintAssignable, Category = "RuntimeDebugsSubsystem")
    FOnDebugComboBoxChanged OnDebugComboBoxChanged;

protected:

    FDebugTab* GetTab(const FName InID);

    FDebugTab* GetTabChecked(const FName InID);

    const FDebugTab* GetTab(const FName InID) const;

    const FDebugTab* GetTabChecked(const FName InID) const;

    FDebugSection* GetDebugSection(const FName InTabID, const FName InID);

    FDebugSection* GetDebugSection(FDebugTab* InTab, const FName InID);
    
    const FDebugSection* GetDebugSection(const FName InTabID, const FName InID) const;

    FDebugTab* GetOrCreateTab(const FName InTabID);

    FDebugSection* GetOrCreateDebugSection(const FName InTabID, const FName InSectionID);

    const FName ResolveTabID(const FName InTabID) const;

    const FName ResolveDebugSectionID(const FName InSectionID) const;

    UPROPERTY(BlueprintReadOnly, Category = "RuntimeDebugsSubsystem")
    TArray<FDebugTab> Tabs;

private:

    //Runtime value of the default tab name and the default section name , they get initialized upon construction when the game start 
    // and then should no be changed
    FName DefaultTab;
    FName DefaultSection;
    
};