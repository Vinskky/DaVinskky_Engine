#ifndef _MATH_GEO_INCL_H_
#define _MATH_GEO_INCL_H_

#include "External/MathGeoLib/include/Math/TransformOps.h"
#include "External/MathGeoLib/include/Math/float4x4.h"
#include "External/MathGeoLib/include/Math/float3x3.h"
#include "External/MathGeoLib/include/Math/float3.h"
#include "External/MathGeoLib/include/Math/Quat.h"
#include "External/MathGeoLib/include/Algorithm/Random/LCG.h"

struct mathTransform
{
	mathTransform() : position(float3::zero),rotation(Quat::identity), scale(float3::zero) {}
	mathTransform(const float3& position, const Quat& rotation, const float3& scale): position(position),rotation(rotation), scale(scale) {}
	mathTransform(float4x4 transformMatrix) { transformMatrix.Decompose(position, rotation, scale); }

	float3 position;
	Quat rotation;
	float3 scale;
};


#endif
