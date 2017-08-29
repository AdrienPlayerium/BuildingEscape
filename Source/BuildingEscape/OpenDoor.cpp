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
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void UOpenDoor::OpenDoor()
{
	AActor* Owner = GetOwner();
	Owner->SetActorRotation(FRotator(0.0f, 0.0f , 0.0f));
	isDoorOpen = true;
}

void UOpenDoor::CloseDoor()
{
	AActor* Owner = GetOwner();
	Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
	isDoorOpen = false;
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Pool trigger Volume if actor in the volume
	if (PressurePlate->IsOverlappingActor(ActorThatOpens))
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

