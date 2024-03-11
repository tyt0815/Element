// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/*
 * FMath, UKismetMathLibrary���� �������� �ʴ� �Լ��� ����.
 */
class ELEMENT_API MathHelper
{
public:
	static double GetAngleBetweenTwoVectorsByRadians(FVector A, FVector B);
	static double GetAngleBetweenTwoVectorsByDegree(FVector A, FVector B);
};
