// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDMainWidget.h"

void UHUDMainWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	m_PBShotTime = Cast<UProgressBar>(GetWidgetFromName(TEXT("PBShot")));
	m_TBShotCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("TBShotCount")));
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

void UHUDMainWidget::AddShotCount(int Count)
{
	int32 UICount = FCString::Atoi(*m_TBShotCount->GetText().ToString());

	UICount += Count;

	m_TBShotCount->SetText(FText::FromString(FString::Printf(TEXT("%d"), UICount)));
}

void UHUDMainWidget::ResetShotCount()
{
	m_TBShotCount->SetText(FText::FromString(FString::Printf(TEXT("0"))));
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
	m_PBShotTime->SetPercent(Time >= 3.f ? 1.f : Time / 3.f);
}