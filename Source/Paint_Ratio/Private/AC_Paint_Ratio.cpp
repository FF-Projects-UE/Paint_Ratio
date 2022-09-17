// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_Paint_Ratio.h"
#include "GameFramework/Actor.h"

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

	FTimerHandle Handle_CRT_Clear;
	this->GetOwner()->GetWorldTimerManager().SetTimer(Handle_CRT_Clear, this, &UAC_Paint_Ratio::ClearRenderTarget, 0.2, false);

	FTimerHandle Handle_Timer;
	this->GetOwner()->GetWorldTimerManager().SetTimer(Handle_Timer, this, &UAC_Paint_Ratio::GetPaintedPixels, ReadInterval, true);
}

// Called every frame
void UAC_Paint_Ratio::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAC_Paint_Ratio::ClearRenderTarget()
{
	UKismetRenderingLibrary::ClearRenderTarget2D(GEngine->GetCurrentPlayWorld(), CRT_Drawing);
}

void UAC_Paint_Ratio::GetPaintedPixels()
{
	TArray<FColor> Array_Colors;
	UKismetRenderingLibrary::ReadRenderTarget(GEngine->GetCurrentPlayWorld(), CRT_Drawing, Array_Colors, true);

	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, Array_Colors]()
		{
			for (int32 Pixel_Index = 0; Pixel_Index < Array_Colors.Num(); Pixel_Index++)
			{
				if (Array_Colors[Pixel_Index] == AlphaColor)
				{
					Painted_Pixels.Add((FString::FromInt(Pixel_Index) + "_" + FString::FromInt(AlphaColor.R) + "-" + FString::FromInt(AlphaColor.G) + "-" + FString::FromInt(AlphaColor.B) + "-" + FString::FromInt(AlphaColor.A)), Array_Colors[Pixel_Index]);
				}
			}

			double Ratio = UKismetMathLibrary::Divide_DoubleDouble(Painted_Pixels.Num(), Array_Colors.Num());

			AsyncTask(ENamedThreads::GameThread, [this, Ratio]()
				{
					OutRatio = Ratio;

					if (bAllowDebugMessage == true)
					{	
						GEngine->AddOnScreenDebugMessage(0, ReadInterval, FColor::Red, FString::SanitizeFloat(Ratio));
					}
				}
			);
		}
	);
}