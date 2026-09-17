// Fill out your copyright notice in the Description page of Project Settings.


#include "RuntimeDebugsGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "RuntimeIMDebugsSubsystem.h"
#include "ProjectileData.h"

void URuntimeDebugsGameInstance::Init() 
{
	Super::Init();

	PostLoadMapWithWorldHandle = FWorldDelegates::OnWorldInitializedActors.AddUObject(this, &URuntimeDebugsGameInstance::OnWorldInitialized);

}

void URuntimeDebugsGameInstance::Shutdown() 
{
	if (PostLoadMapWithWorldHandle.IsValid()) 
	{
		FWorldDelegates::OnWorldInitializedActors.Remove(PostLoadMapWithWorldHandle);
	}

	Super::Shutdown();
}


void URuntimeDebugsGameInstance::OnWorldInitialized(const FActorsInitializedParams&)
{
	OnWorldInitialized();

	DebugLoadLevelIndex = 0;

	if (UWorld* World = GetWorld())
	{
		URuntimeIMDebugsSubsystem* DebugsSubsytem = World->GetSubsystem<URuntimeIMDebugsSubsystem>();

		if (DebugsSubsytem)
		{
			DebugsSubsytem->OnToggleChanged.AddUniqueDynamic(this, &URuntimeDebugsGameInstance::OnDebugToggleChanged);
			DebugsSubsytem->OnSpinBoxChanged.AddUniqueDynamic(this, &URuntimeDebugsGameInstance::OnDebugSpinBoxChanged);
			DebugsSubsytem->OnDebugComboBoxChanged.AddUniqueDynamic(this, &URuntimeDebugsGameInstance::OnDebugComboBoxChanged);
			DebugsSubsytem->OnButtonPressed.AddUniqueDynamic(this, &URuntimeDebugsGameInstance::OnDebugButtonPressed);

			DebugsSubsytem->AddComboBox("None", "None", "SelectLoadingLevel", "", LevelNames);
			DebugsSubsytem->AddButton("None", "None", "LoadLevel", "");
			
			for (UProjectileData* ProjectileData : DebugProjectilesData)
			{
				FString FullName = GetNameSafe(ProjectileData);
				FString DataName;

				//Remove the prefix from the asset name
				FullName.Split("DT_", nullptr, &DataName);

				DebugsSubsytem->AddSpinBox("Projectiles", FName(DataName), "Damage", "", ProjectileData->Data.Damage, 0,  100);
				DebugsSubsytem->AddSpinBox("Projectiles", FName(DataName), "LifeTime", "", ProjectileData->Data.LifeTime, 0 , 100);
				DebugsSubsytem->AddToggle("Projectiles", FName(DataName), "HommingProjectile", "", ProjectileData->Data.HomingProjectile);
				DebugsSubsytem->AddSpinBox("Projectiles", FName(DataName), "HommingAcceleration", "", ProjectileData->Data.HomingAcceleration, 1000, 10000);
				DebugsSubsytem->AddSpinBox("Projectiles", FName(DataName), "MaxSpeed", "", ProjectileData->Data.MaxSpeed, 0, 5000);
				DebugsSubsytem->AddSpinBox("Projectiles", FName(DataName), "GravityScale", "", ProjectileData->Data.GravityScale, -10, 10);

			}
		}
	}
}

UProjectileData* URuntimeDebugsGameInstance::GetProjectileDataByName(TArray<UProjectileData*> InProjectiles, FName InProjectileName) const
{	
	if (InProjectiles.IsEmpty()) 
	{
		return nullptr;
	}

	const FString TargetNameStr = InProjectileName.ToString();

	for (TObjectPtr<UProjectileData> ProjectileData : InProjectiles)
	{
		if (!ProjectileData)
		{
			continue;
		}

		FString FullName = GetNameSafe(ProjectileData);
		FString DataName;
		
		//Remove the prefix from the asset name
		FullName.Split("DT_", nullptr, &DataName);

		if (DataName.Equals(TargetNameStr))
		{
			return ProjectileData;			
		}
		
	}

	return nullptr;

}

void URuntimeDebugsGameInstance::OnDebugToggleChanged(const FName InTabID, const FName InSectionID, const FName InID, bool InValue)
{
	UProjectileData* CurrentProjectileData = GetProjectileDataByName(DebugProjectilesData, InSectionID);

	if (!IsValid(CurrentProjectileData))
		return;

	if (InID == "HommingProjectile")
	{
		CurrentProjectileData->Data.HomingProjectile = InValue;
	}
}

void URuntimeDebugsGameInstance::OnDebugSpinBoxChanged(const FName InTabID, const FName InSectionID, const FName InID, float InValue)
{
	UProjectileData* CurrentProjectileData = GetProjectileDataByName(DebugProjectilesData, InSectionID);

	if (!IsValid(CurrentProjectileData))
		return;

	if (InID == "Damage")
	{
		CurrentProjectileData->Data.Damage = InValue;
	}
	else if (InID == "LifeTime")
	{
		CurrentProjectileData->Data.LifeTime = InValue;
	}
	else if (InID == "HommingAcceleration")
	{
		CurrentProjectileData->Data.HomingAcceleration = InValue;
	}
	else if (InID == "MaxSpeed")
	{
		CurrentProjectileData->Data.MaxSpeed = InValue;
	}
	else if (InID == "GravityScale")
	{
		CurrentProjectileData->Data.GravityScale = InValue;
	}
}

void URuntimeDebugsGameInstance::OnDebugComboBoxChanged(const FName InTabID, const FName InSectionID, const FName InID, int InIndex)
{
	if (InID == "SelectLoadingLevel")
	{
		DebugLoadLevelIndex = InIndex;
	}
}

void URuntimeDebugsGameInstance::OnDebugButtonPressed(const FName InTabID, const FName InSectionID, const FName InID)
{
	if (InID == "LoadLevel")
	{
		UWorld* World = GetWorld();

		if (!IsValid(World)) 
		{
			return;
		}

		if (!LevelNames.IsEmpty() && LevelNames.IsValidIndex(DebugLoadLevelIndex))
		{
			UGameplayStatics::OpenLevel(World, FName(LevelNames[DebugLoadLevelIndex]));
		}
	}
}
