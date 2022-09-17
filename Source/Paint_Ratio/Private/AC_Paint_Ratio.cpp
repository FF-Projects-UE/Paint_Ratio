// Fill out your copyright notice in the Description page of Project Settings.

#include "AC_Paint_Ratio.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetRenderingLibrary.h"

// Sets default values for this component's properties
UAC_Paint_Ratio::UAC_Paint_Ratio()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UAC_Paint_Ratio::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle Handle_Prepare;
	this->GetOwner()->GetWorldTimerManager().SetTimer(Handle_Prepare, this, &UAC_Paint_Ratio::PrepareRenderTarget, 0.2, false);

}

// Called every frame
void UAC_Paint_Ratio::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAC_Paint_Ratio::PrepareRenderTarget()
{
	UKismetRenderingLibrary::ClearRenderTarget2D(GEngine->GetCurrentPlayWorld(), CRT_Drawing);

	int32 Size_X = 0;
	int32 Size_Y = 0;
	CRT_Drawing->GetSize(Size_X, Size_Y);
	
	Pixel_Count = Size_X * Size_Y;
}

void UAC_Paint_Ratio::RecordPaintedPixels(FColor AlphaColor)
{
	TArray<FColor> Array_Colors;
	UKismetRenderingLibrary::ReadRenderTarget(GEngine->GetCurrentPlayWorld(), CRT_Drawing, Array_Colors, true);

	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, AlphaColor, Array_Colors]()
		{
			for (int32 Pixel_Index = 0; Pixel_Index < Array_Colors.Num(); Pixel_Index++)
			{
				if (Array_Colors[Pixel_Index] == AlphaColor)
				{
					Painted_Pixels.Add((FString::FromInt(AlphaColor.R) + "-" + FString::FromInt(AlphaColor.G) + "-" + FString::FromInt(AlphaColor.B) + "-" + FString::FromInt(AlphaColor.A) + "_" + FString::FromInt(Pixel_Index)), Array_Colors[Pixel_Index]);
				}
			}

			AsyncTask(ENamedThreads::GameThread, []()
				{

				}
			);
		}
	);
}

void UAC_Paint_Ratio::GetColorRatio(FPaintRatio DelegatePaintRatio, FColor WantedColor)
{
	TArray<FString> Array_Keys;
	Painted_Pixels.GetKeys(Array_Keys);

	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [DelegatePaintRatio, WantedColor, Array_Keys, this]()
		{
			TArray<FString> Array_Wanted;
			for (int32 KeyIndex = 0; KeyIndex < Array_Keys.Num(); KeyIndex++)
			{
				TArray<FString> Key_Sections;
				Array_Keys[KeyIndex].ParseIntoArray(Key_Sections, TEXT("_"));

				if (Key_Sections[0] == (FString::FromInt(WantedColor.R) + "-" + FString::FromInt(WantedColor.G) + "-" + FString::FromInt(WantedColor.B) + "-" + FString::FromInt(WantedColor.A)))
				{
					Array_Wanted.Add(Key_Sections[0]);
				}
			}

			double Ratio = Array_Wanted.Num() / Pixel_Count;

			AsyncTask(ENamedThreads::GameThread, [DelegatePaintRatio, Ratio]()
				{
					DelegatePaintRatio.ExecuteIfBound(Ratio);
				}
			);
		}
	);
}