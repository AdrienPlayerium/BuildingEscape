// ue course 2017

#include "Grabber.h"
#include "Engine/World.h"
#include "Gameframework/Actor.h"
#include "DrawDebugHelpers.h"
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// Find (assumed) attached physics handle
	FindPhysicsHandleComponent();
	// Setup (assumed) attached input component
	SetupInputComponent();

}

void UGrabber::FindPhysicsHandleComponent()
{
	/// Look for physic handle
	physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!physicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing PhysicsHandle component "), *(GetOwner()->GetName()))
	}
}

void UGrabber::SetupInputComponent()
{
	/// Look for InputComponent (only appears in runtime)
	inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (inputComponent)
	{
		inputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		inputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing InputComponent component "), *(GetOwner()->GetName()))
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"));

	// Try and reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();
	// If we hit something then attach a physics handle
	if (ActorHit && physicsHandle)
	{
		physicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true // allow rotation
		);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab released"));

	// release physics handle
	if (physicsHandle)
	{
		physicsHandle->ReleaseComponent();
	}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
	if (physicsHandle && physicsHandle->GrabbedComponent)
	{
		physicsHandle->SetTargetLocation(GetReachLineEnd());
	}

}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	/// Line-trace (ray-cast) out of reach distance
	FHitResult Hit;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	///See what we hit
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(ActorHit->GetName()))
	}
	return Hit;
}

FVector UGrabber::GetReachLineEnd()
{
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT playerViewPointLocation,
		OUT playerViewPointRotation
	);

	return playerViewPointLocation + playerViewPointRotation.Vector() * Reach;
}

FVector UGrabber::GetReachLineStart()
{
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT playerViewPointLocation,
		OUT playerViewPointRotation
	);

	return playerViewPointLocation;
}

