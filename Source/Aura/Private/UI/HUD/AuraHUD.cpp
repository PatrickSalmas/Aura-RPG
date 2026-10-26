// Copyright Salmas Corporation


#include "UI/HUD/AuraHUD.h"

#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "UI/WidgetController/ProgressionMenuWidgetController.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbackToDependencies();
	}
	return OverlayWidgetController;
}

UAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);
		AttributeMenuWidgetController->BindCallbackToDependencies();
	}
	return AttributeMenuWidgetController;
}

USpellMenuWidgetController* AAuraHUD::GetSpellMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (SpellMenuWidgetController == nullptr)
	{
		SpellMenuWidgetController = NewObject<USpellMenuWidgetController>(this, SpellMenuWidgetControllerClass);
		SpellMenuWidgetController->SetWidgetControllerParams(WCParams);
		SpellMenuWidgetController->BindCallbackToDependencies();
	}
	return SpellMenuWidgetController;
}

UProgressionMenuWidgetController* AAuraHUD::GetProgressionMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (ProgressionMenuWidgetController == nullptr)
	{
		ProgressionMenuWidgetController = NewObject<UProgressionMenuWidgetController>(this, ProgressionMenuWidgetControllerClass);
		ProgressionMenuWidgetController->SetWidgetControllerParams(WCParams);
		ProgressionMenuWidgetController->BindCallbackToDependencies();
	}

	return ProgressionMenuWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay widget class uninitialized, please fill out BP_AuraHUD"))
	checkf(OverlayWidgetControllerClass, TEXT("Overlay widget controller class uninitialized, please fill out BP_AuraHUD"))
	checkf(ProgressionMenuWidgetControllerClass, TEXT("Progression menu widget controller class uninitialized, please fill out BP_AuraHUD"))

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);
	GetProgressionMenuWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();
	Widget->AddToViewport();
}

void AAuraHUD::OpenProgressionMenu(const FGameplayTag& SelectionTag)
{
	if (!ProgressionMenuWidgetController)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot open progression menu because its widget controller has not been initialized."));
		return;
	}

	if (!ProgressionMenuWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot open progression menu because ProgressionMenuWidgetClass is not assigned."));
		return;
	}

	if (!ProgressionMenuWidget)
	{
		ProgressionMenuWidget = CreateWidget<UAuraUserWidget>(GetOwningPlayerController(), ProgressionMenuWidgetClass);

		if (!ProgressionMenuWidget)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create the progression menu widget."));
			return;
		}

		ProgressionMenuWidget->SetWidgetController(ProgressionMenuWidgetController);
	}

	if (!ProgressionMenuWidgetController->OpenProgressionSelection(SelectionTag))
	{
		return;
	}

	if (!ProgressionMenuWidget->IsInViewport())
	{
		ProgressionMenuWidget->AddToViewport(50);
	}
}
