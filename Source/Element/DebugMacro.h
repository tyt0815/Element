#pragma once

// String Log
#define SCREEN_LOG(Key, DebugMessage)\
if(GEngine)\
{ FColor LogColor; \
switch (Key){ case 0: LogColor = FColor::Red; break; case 1: LogColor = FColor::Green; break; case 2: LogColor = FColor::Blue; break;\
				case 3: LogColor = FColor::Yellow; break; case 4: LogColor = FColor::Cyan; break; case 5: LogColor = FColor::Magenta; break;  }\
GEngine->AddOnScreenDebugMessage(Key, 5, LogColor, DebugMessage);}
#define SCREEN_LOG_COLOR(Key, DisplayColor, DebugMessage) if(GEngine)\
	{ GEngine->AddOnScreenDebugMessage(Key, 5, DisplayColor, DebugMessage); }

#define DRAW_SPHERE(Location) if(GetWorld())DrawDebugSphere(GetWorld(), Location, 25.f, 12, FColor::Cyan, false, 5.0f);
#define DRAW_SPHERE_COLOR(Location, Color) if(GetWorld())DrawDebugSphere(GetWorld(), Location, 8.f, 12, Color, false, 5.0f);
#define DRAW_SPHERE_SINGLE_FRAME(Location) if(GetWorld())DrawDebugSphere(GetWorld(), Location, 25.f, 12, FColor::Cyan, false, -1.f);
#define DRAW_LINE(StartLocation, EndLocation) if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, -1.f, 0, 1.f);
#define DRAW_LINE_COLOR(StartLocation, EndLocation, Color) if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, Color, true, -1.f, 0, 1.f);
#define DRAW_LINE_SINGLE_FRAME(StartLocation, EndLocation) if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.f, 0, 1.f);
#define DRAW_LINE_SINGLE_FRAME_COLOR(StartLocation, EndLocation, Color) if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, Color, false, -1.f, 0, 1.f);
#define DRAW_POINT(Location) if(GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Green, true);
#define DRAW_POINT_SingleFrame(Location) if(GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Green, false, -1.f);
#define DRAW_VECTOR(StartLocation, EndLocation) if (GetWorld())\
{\
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, -1.f, 0, 1.f);\
	DrawDebugPoint(GetWorld(), EndLocation, 15.f, FColor::Green, true);\
}
#define DRAW_VECTOR_SingleFrame(StartLocation, EndLocation) if (GetWorld())\
{\
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.f, 0, 1.f);\
	DrawDebugPoint(GetWorld(), EndLocation, 15.f, FColor::Green, false, -1.f);\
}