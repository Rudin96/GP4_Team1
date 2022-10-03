#pragma once

#define LOG(Duration, Color, Message) GEngine->AddOnScreenDebugMessage(INDEX_NONE, Duration, Color, Message);
#define DEBUG_SPHERE(Location, Size, Color, Duration) DrawDebugSphere(GetWorld(), Location, Size, 10, Color, false, Duration);
#define DEBUG_LINE(Start, End, Color, Duration) DrawDebugLine(GetWorld(), Start, End, Color, false, Duration);