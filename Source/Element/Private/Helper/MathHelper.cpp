#include "Helper/MathHelper.h"

double MathHelper::GetAngleBetweenTwoVectors(FVector A, FVector B)
{
	A.Normalize();
	B.Normalize();
	return FMath::Acos(A.Dot(B));
}