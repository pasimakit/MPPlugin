// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/UI/Menu/Multiplayer/SMenuMultiplayerWidget.h"

#include "CommonBorder.h"
#include "CommonGameInstance.h"
#include "Framework/Data/SGameData.h"
#include "Framework/Data/StaticGameData.h"
#include "Framework/UI/SPrimaryGameLayout.h"
#include "Framework/UI/Menu/SButtonBaseWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Framework/UI/Menu/Multiplayer/SMenuHostSessionWidget.h"
#include "Framework/UI/Menu/Multiplayer/SMenuJoinSessionWidget.h"

void USMenuMultiplayerWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if(QuickPlayButton)
	{
		QuickPlayButton->OnClicked().AddUObject(this, &USMenuMultiplayerWidget::OnQuickPlayButtonClicked);
	}

	if(HostButton)
	{
		HostButton->OnClicked().AddUObject(this, &USMenuMultiplayerWidget::OnHostButtonClicked);
	}

	if(JoinButton)
	{
		JoinButton->OnClicked().AddUObject(this, &USMenuMultiplayerWidget::OnJoinButtonClicked);
	}

	if(ExitButton)
	{
		ExitButton->OnClicked().AddUObject(this, &USMenuMultiplayerWidget::OnExitButtonClicked);
	}
	
	SetSpinnerDisplay(false);
}

UWidget* USMenuMultiplayerWidget::NativeGetDesiredFocusTarget() const
{
	if(HostButton)
	{
		return HostButton;
	}
	
	return Super::NativeGetDesiredFocusTarget();
}

void USMenuMultiplayerWidget::OnHostButtonClicked()
{
	if(HostWidgetClass == nullptr) return;

	if(const UWorld* WorldContext = GetWorld())
	{
		if(UPrimaryGameLayout* RootLayout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(WorldContext))
		{
			RootLayout->PushWidgetToLayerStackAsync<UCommonActivatableWidget>(UILayerTags::TAG_UI_LAYER_MENU, true, HostWidgetClass,
				[this](EAsyncWidgetLayerState State, UCommonActivatableWidget* Screen)
				{
					switch (State)
					{
					case EAsyncWidgetLayerState::AfterPush:
						return;
					case EAsyncWidgetLayerState::Canceled:
						return;
					}
				});
		}
	}
}

void USMenuMultiplayerWidget::OnJoinButtonClicked()
{
	if(JoinWidgetClass == nullptr) return;

	if(const UWorld* WorldContext = GetWorld())
	{
		if(UPrimaryGameLayout* RootLayout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(WorldContext))
		{
			RootLayout->PushWidgetToLayerStackAsync<UCommonActivatableWidget>(UILayerTags::TAG_UI_LAYER_MENU, true, JoinWidgetClass,
				[this](EAsyncWidgetLayerState State, UCommonActivatableWidget* Screen)
				{
					switch (State)
					{
					case EAsyncWidgetLayerState::AfterPush:
						return;
					case EAsyncWidgetLayerState::Canceled:
						return;
					}
				});
		}
	}
}

void USMenuMultiplayerWidget::OnExitButtonClicked()
{
	if(GetOwningPlayer())
	{
		if(const UWorld* WorldContext = GetWorld())
		{
			UKismetSystemLibrary::QuitGame(WorldContext, GetOwningPlayer(), EQuitPreference::Quit, false);
		}
	}
}

void USMenuMultiplayerWidget::OnQuickPlayButtonClicked()
{
	if(!GetOwningPlayer() || !GetOwningPlayer()->GetWorld()) return;

	if(const UCommonGameInstance* GameInstance = Cast<UCommonGameInstance>(GetOwningPlayer()->GetWorld()->GetGameInstance()))
	{
		if(UCommonUserSubsystem* UserSubsystem = GameInstance->GetSubsystem<UCommonUserSubsystem>())
		{
			UserSubsystem->OnUserInitializeComplete.AddDynamic(this, &USMenuMultiplayerWidget::OnUserOnlineLogin);
			UserSubsystem->TryToLoginForOnlinePlay(0);
		}
	}
}

void USMenuMultiplayerWidget::OnUserOnlineLogin(const UCommonUserInfo* UserInfo, bool bSuccess, FText Error,
												ECommonUserPrivilege RequestedPrivilege, ECommonUserOnlineContext OnlineContext)
{
	if(bSuccess)
	{
		QuickPlaySession();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[USMenuMultiplayerWidget::OnUserOnlineLogin] Failed to login online"));
	}
}

void USMenuMultiplayerWidget::QuickPlaySession()
{
	if(!GetOwningPlayer() || !GetOwningPlayer()->GetWorld()) return;

	if(const UCommonGameInstance* GameInstance = Cast<UCommonGameInstance>(GetOwningPlayer()->GetWorld()->GetGameInstance()))
	{
		if(UCommonSessionSubsystem* SessionSubsystem = GameInstance->GetSubsystem<UCommonSessionSubsystem>())
		{
			UCommonSession_HostSessionRequest* HostSessionRequest = CreateQuickPlayHostSessionRequest();
			SetSpinnerDisplay(true);
			SessionSubsystem->QuickPlaySession(GetOwningPlayer(), HostSessionRequest);
			SessionSubsystem->OnJoinSessionCompleteEvent.AddUObject(this, &USMenuMultiplayerWidget::OnJoinSessionComplete);
		}
	}
}

UCommonSession_HostSessionRequest* USMenuMultiplayerWidget::CreateQuickPlayHostSessionRequest()
{
	UCommonSession_HostSessionRequest* Request = NewObject<UCommonSession_HostSessionRequest>();
	FPrimaryAssetId QuickPlayId = FPrimaryAssetId(FPrimaryAssetType(S_DATA_ASSET_TYPE_GAMEDATA), S_DATA_ASSET_TYPE_GAMEDATA_QUICKPLAY);

	if(QuickPlayId.IsValid())
	{
		if(const UAssetManager* AssetManager = UAssetManager::GetIfInitialized())
		{
			if(const USGameData* GameData = Cast<USGameData>(AssetManager->GetPrimaryAssetObject(QuickPlayId)))
			{
				const FString GameDataName = QuickPlayId.PrimaryAssetName.ToString();
				Request->ModeNameForAdvertisement = GameDataName;
				Request->OnlineMode = ECommonSessionOnlineMode::Online;
				Request->bUseLobbies = true;
				Request->MapID = GameData->MapID;
				Request->ExtraArgs = GameData->ExtraArgs;
				Request->ExtraArgs.Add(S_MP_SETTINGS_GAMEMODE, GameDataName);
				Request->MaxPlayerCount = GameData->MaxPlayerCount;
			}
		}
	}
	return Request;
}

void USMenuMultiplayerWidget::OnJoinSessionComplete(const FOnlineResultInformation& Result) const
{
	SetSpinnerDisplay(Result.bWasSuccessful);
}

void USMenuMultiplayerWidget::SetSpinnerDisplay(const bool bDisplay) const
{
	if(SpinnerDisplay)
	{
		SpinnerDisplay->SetVisibility(bDisplay? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}


