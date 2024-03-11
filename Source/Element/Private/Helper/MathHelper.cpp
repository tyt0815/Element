#include "Helper/MathHelper.h"

double MathHelper::GetAngleBetweenTwoVectorsByRadians(FVector A, FVector B)
{
	A.Normalize();
	B.Normalize();
	return FMath::Acos(A.Dot(B));
}

double MathHelper::GetAngleBetweenTwoVectorsByDegree(FVector A, FVector B)
{
	return FMath::RadiansToDegrees(GetAngleBetweenTwoVectorsByRadians(A, B));
}
