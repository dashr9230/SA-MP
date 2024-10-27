
#include "main.h"
#include <math.h>

#ifndef MAX
	#define MAX(x, y) ((x) < (y) ? (y) : (x))
#endif

//----------------------------------------------------

bool FUNC_1(PVECTOR vec)
{
	if( vec->X <= 1.0f && vec->X >= -1.0f &&
		vec->Y <= 1.0f && vec->Y >= -1.0f &&
		vec->Z <= 1.0f && vec->Z >= -1.0f )
	{
		return true;
	}
	return false;
}

//----------------------------------------------------

bool FUNC_2(PVECTOR vec)
{
	if( vec->X < 20000.0f && vec->X > -20000.0f &&
		vec->Y < 20000.0f && vec->Y > -20000.0f &&
		vec->Z < 200000.0f && vec->Z > -1000.0f )
	{
		return true;
	}
	return false;
}

//----------------------------------------------------

bool FUNC_3(PVECTOR vec)
{
	if( vec->X < 100.0f && vec->X > -100.0f &&
		vec->Y < 100.0f && vec->Y > -100.0f &&
		vec->Z < 100.0f && vec->Z > -100.0f )
	{
		return true;
	}
	return false;
}

//----------------------------------------------------

float GetLength(PVECTOR vec)
{
	return (float)sqrt((vec->X * vec->X) + (vec->Y * vec->Y) + (vec->Z * vec->Z));
}

//----------------------------------------------------

void CrossProduct(PVECTOR out, PVECTOR vec1, PVECTOR vec2)
{
	out->X = vec1->Y * vec2->Z - vec2->Y * vec1->Z;
	out->Y = vec1->Z * vec2->X - vec2->Z * vec1->X;
	out->Z = vec1->X * vec2->Y - vec2->X * vec1->Y;
}

//----------------------------------------------------

void Transform(PVECTOR out, PMATRIX4X4 mat, PVECTOR vec)
{
	out->X = vec->X * mat->right.X + vec->Y * mat->up.X + vec->Z * mat->at.X + mat->pos.X;
	out->Y = vec->X * mat->right.Y + vec->Y * mat->up.Y + vec->Z * mat->at.Y + mat->pos.Y;
	out->Z = vec->X * mat->right.Z + vec->Y * mat->up.Z + vec->Z * mat->at.Z + mat->pos.Z;
}

//----------------------------------------------------

void MatrixToQuaternion(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22, float *qw, float *qx, float *qy, float *qz)
{
	float w1 = (float)sqrt(MAX(1.0f + m00 + m11 + m22, 0.0f)) * 0.5f;
	float x1 = (float)sqrt(MAX(1.0f + m00 - m11 - m22, 0.0f)) * 0.5f;
	float y1 = (float)sqrt(MAX(1.0f - m00 + m11 - m22, 0.0f)) * 0.5f;
	float z1 = (float)sqrt(MAX(1.0f - m00 - m11 + m22, 0.0f)) * 0.5f;

	if(w1 < 0.0f) w1 = 0.0f;
	if(x1 < 0.0f) x1 = 0.0f;
	if(y1 < 0.0f) y1 = 0.0f;
	if(z1 < 0.0f) z1 = 0.0f;

	float x2 = _copysign(x1, m21 - m12);
	float y2 = _copysign(y1, m02 - m20);
	float z2 = _copysign(z1, m10 - m01);

	*qw = w1;
	*qx = x2;
	*qy = y2;
	*qz = z2;
}

//----------------------------------------------------

void QuaternionToMatrix(float qw, float qx, float qy, float qz, float *m00, float *m01, float *m02, float *m10, float *m11, float *m12, float *m20, float *m21, float *m22)
{
	float ww = qw * qw;
	float xx = qx * qx;
	float yy = qy * qy;
	float zz = qz * qz;

	*m00 = xx - yy - zz + ww;
	*m11 = yy - xx - zz + ww;
	*m22 = zz - (yy + xx) + ww;

	float xy = qx * qy;
	float zw = qz * qw;
	*m10 = zw + xy + zw + xy;
	*m01 = xy - zw + xy - zw;

	float xz = qx * qz;
	float yw = qw * qy;
	*m20 = xz - yw + xz - yw;
	*m02 = yw + xz + yw + xz;

	float yz = qy * qz;
	float xw = qx * qw;
	*m21 = xw + yz + xw + yz;
	*m12 = yz - xw + yz - xw;
}

//----------------------------------------------------

void MatrixToQuaternion(PMATRIX4X4 mat, PQUATERNION quat)
{
	MatrixToQuaternion(
		mat->right.X,
		mat->right.Y,
		mat->right.Z,
		mat->up.X,
		mat->up.Y,
		mat->up.Z,
		mat->at.X,
		mat->at.Y,
		mat->at.Z,
		&quat->W,
		&quat->X,
		&quat->Y,
		&quat->Z);
}

//----------------------------------------------------

void QuaternionToMatrix(PQUATERNION quat, PMATRIX4X4 mat)
{
	QuaternionToMatrix(
		quat->W,
		quat->X,
		quat->Y,
		quat->Z,
		&mat->right.X,
		&mat->right.Y,
		&mat->right.Z,
		&mat->up.X,
		&mat->up.Y,
		&mat->up.Z,
		&mat->at.X,
		&mat->at.Y,
		&mat->at.Z);
}

//----------------------------------------------------

float GetNormalisation(PVECTOR vec)
{
	return ((vec->X * vec->X) + (vec->Y * vec->Y) + (vec->Z * vec->Z));
}

//----------------------------------------------------

void ToQuaternion(PVECTOR vec, PQUATERNION quat)
{
	float cr = cosf(vec->X * 0.5f);
	float cp = cosf(vec->Y * 0.5f);
	float cy = cosf(vec->Z * 0.5f);
	float sr = sinf(vec->X * 0.5f);
	float sp = sinf(vec->Y * 0.5f);
	float sy = sinf(vec->Z * 0.5f);

	quat->W = cr * cp * cy + sr * sp * sy;
	quat->X = sr * cp * cy - cr * sp * sy;
	quat->Y = cr * sp * cy + sr * cp * sy;
	quat->Z = cr * cp * sy - sr * sp * cy;
}

//----------------------------------------------------

void ToEulerAngles(PQUATERNION quat, PVECTOR vec)
{

}

//----------------------------------------------------
