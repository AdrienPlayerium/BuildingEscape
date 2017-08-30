// ue course 2017

#include "OpenDoor.h"
#include "Engine/World.h"
#include "Gameframework/Actor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("unassigned PressurePlate"));
	}
}

void UOpenDoor::OpenDoor()
{
	Owner->SetActorRotation(FRotator(0.0f, 0.0f , 0.0f));
	isDoorOpen = true;
}

void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
	isDoorOpen = false;
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Pool trigger Volume if actor in the volume
	if (GetTotalMassOfActorsOnPlate() > TriggerMass)
	{
	//	UE_LOG(LogTemp, Warning, TEXT("Door is opening"));
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	//GetTime
	if (isDoorOpen && GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	{
		UE_LOG(LogTemp, Warning, TEXT("Door is closing"));
		CloseDoor();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("Unassigned pressurePlate"));
		return 0;
	}
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	for (const auto& Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}

