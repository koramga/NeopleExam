// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDMainWidget.h"

void UHUDMainWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	m_PBShotTime = Cast<UProgressBar>(GetWidgetFromName(TEXT("PBShot")));

	m_TBTotalProjectileCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("TBTotalProjectileCount")));

	m_TBProjectileCount.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("TBNormalProjectileCount"))));
	m_TBProjectileCount.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("TBChargeProjectileCount"))));
	m_TBProjectileCount.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("TBSplitProjectileCount"))));
	m_TBProjectileCount.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("TBReflectProjectileCount"))));

	m_BTTotalProjectileResetCount = Cast<UButton>(GetWidgetFromName(TEXT("BTResetProjectileCount")));

	m_BTTotalProjectileResetCount->OnClicked.AddDynamic(this, &UHUDMainWidget::TotalResetButtonClick);

	m_BTProjectileResetCount.Add(Cast<UButton>(GetWidgetFromName(TEXT("BTResetNormalProjectileCount"))));
	m_BTProjectileResetCount.Add(Cast<UButton>(GetWidgetFromName(TEXT("BTResetChargeProjectileCount"))));
	m_BTProjectileResetCount.Add(Cast<UButton>(GetWidgetFromName(TEXT("BTResetSplitProjectileCount"))));
	m_BTProjectileResetCount.Add(Cast<UButton>(GetWidgetFromName(TEXT("BTResetReflectProjectileCount"))));

	m_BTProjectileResetCount[0]->OnClicked.AddDynamic(this, &UHUDMainWidget::NormalResetButtonClick);
	m_BTProjectileResetCount[1]->OnClicked.AddDynamic(this, &UHUDMainWidget::ChargeResetButtonClick);
	m_BTProjectileResetCount[2]->OnClicked.AddDynamic(this, &UHUDMainWidget::SplitResetButtonClick);
	m_BTProjectileResetCount[3]->OnClicked.AddDynamic(this, &UHUDMainWidget::ReflectResetButtonClick);
	
}

void UHUDMainWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UHUDMainWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UHUDMainWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UHUDMainWidget::TotalResetButtonClick()
{
	ResetProjectileCount();
}

void UHUDMainWidget::NormalResetButtonClick()
{
	ResetProjectileCount(EProjectileType::Normal);
}

void UHUDMainWidget::ChargeResetButtonClick()
{
	ResetProjectileCount(EProjectileType::Charge);
}

void UHUDMainWidget::SplitResetButtonClick()
{
	ResetProjectileCount(EProjectileType::Split);
}

void UHUDMainWidget::ReflectResetButtonClick()
{
	ResetProjectileCount(EProjectileType::Reflect);
}

UTextBlock* UHUDMainWidget::FindTextBlockFromProjectileType(EProjectileType ProjectileType)
{
	if (EProjectileType::None == ProjectileType)
	{
		return nullptr;
	}

	return m_TBProjectileCount[static_cast<int>(ProjectileType) - 1];
}

void UHUDMainWidget::AddProjectileCount(EProjectileType ProjectileType, int Count)
{
	UTextBlock* TextBlock = FindTextBlockFromProjectileType(ProjectileType);

	if (IsValid(TextBlock))
	{
		int32 UICount = FCString::Atoi(*TextBlock->GetText().ToString());

		UICount += Count;

		TextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d"), UICount)));

		int32 TotalCount = FCString::Atoi(*m_TBTotalProjectileCount->GetText().ToString());

		TotalCount += Count;

		m_TBTotalProjectileCount->SetText(FText::FromString(FString::Printf(TEXT("%d"), TotalCount)));
	}
}

void UHUDMainWidget::ResetProjectileCount(EProjectileType ProjectileType)
{
	UTextBlock* TextBlock = FindTextBlockFromProjectileType(ProjectileType);

	if (IsValid(TextBlock))
	{
		int32 UICount = FCString::Atoi(*TextBlock->GetText().ToString());

		TextBlock->SetText(FText::FromString(FString::Printf(TEXT("0"))));

		int32 TotalCount = FCString::Atoi(*m_TBTotalProjectileCount->GetText().ToString());

		TotalCount -= UICount;

		m_TBTotalProjectileCount->SetText(FText::FromString(FString::Printf(TEXT("%d"), TotalCount)));	
	}
}

void UHUDMainWidget::ResetProjectileCount()
{
	m_TBTotalProjectileCount->SetText(FText::FromString(FString::Printf(TEXT("0"))));

	for (UTextBlock* TextBlock : m_TBProjectileCount)
	{
		TextBlock->SetText(FText::FromString(FString::Printf(TEXT("0"))));
	}
}

void UHUDMainWidget::VisibleProgressBar(bool IsVisible)
{
	if (IsVisible)
	{
		m_PBShotTime->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		m_PBShotTime->SetVisibility(ESlateVisibility::Collapsed);
	}
}

bool UHUDMainWidget::IsVisibleProgressBar() const
{
	if (ESlateVisibility::Collapsed == m_PBShotTime->GetVisibility())
	{
		return false;
	}

	return true;

}

void UHUDMainWidget::SetProgressBarTime(float Time)
{
	m_PBShotTime->SetPercent(Time >= FULL_CHARGETIME ? 1.f : Time / FULL_CHARGETIME);
}