
#include "main.h"

#ifndef copysign
	#define copysign _copysign
#endif

#ifndef MAX
	#define MAX(x, y) ((x) < (y) ? (y) : (x))
#endif

//----------------------------------------------------

void CrossProduct(PVECTOR result, PVECTOR v1, PVECTOR v2)
{
	result->X = v1->Y * v2->Z - v1->Z * v2->Y;
	result->Y = v1->Z * v2->X - v1->X * v2->Z;
	result->Z = v1->X * v2->Y - v1->Y * v2->X;
}

//----------------------------------------------------

void TransformVector(PVECTOR result, PMATRIX4X4 m, PVECTOR v)
{
	result->X = m->at.X * v->Z + m->right.X * v->X + m->up.X * v->Y + m->pos.X;
	result->Y = m->at.Y * v->Z + m->right.Y * v->X + m->up.Y * v->Y + m->pos.Y;
	result->Z = m->at.Z * v->Z + m->right.Z * v->X + m->up.Z * v->Y + m->pos.Z;
}

//----------------------------------------------------

void MatrixToQuaternion(float m00, float m01, float m02,
						float m10, float m11, float m12,
						float m20, float m21, float m22,
						float &qw, float &qx, float &qy, float &qz)
{
	float tw = sqrt(MAX(m00 + 1.0f + m11 + m22, 0.0f)) / 2.0f;
	float tx = sqrt(MAX(m00 + 1.0f - m11 - m22, 0.0f)) / 2.0f;
	float ty = sqrt(MAX(1.0f - m00 + m11 - m22, 0.0f)) / 2.0f;
	float tz = sqrt(MAX(1.0f - m00 - m11 + m22, 0.0f)) / 2.0f;

	if(tw < 0.0f) tw = 0.0f;
	if(tx < 0.0f) tx = 0.0f;
	if(ty < 0.0f) ty = 0.0f;
	if(tz < 0.0f) tz = 0.0f;

	float x = (float)copysign(tx, m21 - m12);
	float y = (float)copysign(ty, m02 - m20);
	float z = (float)copysign(tz, m10 - m01);

	qw = tw;
	qx = x;
	qy = y;
	qz = z;
}

//----------------------------------------------------

void QuaternionToMatrix(float qw, float qx, float qy, float qz,
						float &m00, float &m01, float &m02,
						float &m10, float &m11, float &m12,
						float &m20, float &m21, float &m22)
{
	float ww = qw * qw;
	float xx = qx * qx;
	float yy = qy * qy;
	float zz = qz * qz;
	m00 = xx - yy - zz + ww;
	m11 = yy - xx - zz + ww;
	m22 = zz - (yy + xx) + ww;

	float xy = qx * qy;
	float wz = qw * qz;
	m10 = wz + xy + wz + xy;
	m01 = xy - wz + xy - wz;

	float xz = qx * qz;
	float wy = qw * qy;
	m20 = xz - wy + xz - wy;
	m02 = wy + xz + wy + xz;

	float yz = qy * qz;
	float wx = qw * qx;
	m21 = wx + yz + wx + yz;
	m12 = yz - wx + yz - wx;
}

//----------------------------------------------------

void QuaternionToMatrix(PQUATERNION q, PMATRIX4X4 m)
{
	QuaternionToMatrix(q->W, q->X, q->Y, q->Z,
					   m->right.X, m->right.Y, m->right.Z,
					   m->up.X, m->up.Y, m->up.Z,
					   m->at.X, m->at.Y, m->at.Z);
}

//----------------------------------------------------

void MatrixToQuaternion(PMATRIX4X4 m, PQUATERNION q)
{
	MatrixToQuaternion(m->right.X, m->right.Y, m->right.Z,
					   m->up.X, m->up.Y, m->up.Z,
					   m->at.X, m->at.Y, m->at.Z,
					   q->W, q->X, q->Y, q->Z);
}

//----------------------------------------------------

void EulerToQuaternion(PVECTOR v, PQUATERNION q)
{
	float cz = cosf(v->Z * 0.5f);
	float cy = cosf(v->Y * 0.5f);
	float cx = cosf(v->X * 0.5f);
	float sz = sinf(v->Z * 0.5f);
	float sy = sinf(v->Y * 0.5f);
	float sx = sinf(v->X * 0.5f);

	q->W = cz * cy * cx + sz * sy * sx;
	q->X = cz * cy * sx - sz * sy * cx;
	q->Y = cz * sy * cx + sz * cy * sx;
	q->Z = sz * cy * cx - cz * sy * sx;
}

//----------------------------------------------------

void QuaternionToEuler(PQUATERNION q, PVECTOR v)
{
	float sqw = q->W * q->W;
	float sqx = q->X * q->X;
	float sqy = q->Y * q->Y;
	float sqz = q->Z * q->Z;

	float t0 = q->X * q->Y + q->Z * q->W;
	v->X = atan2f(t0 + t0, sqx - sqy - sqz + sqw);

	v->Y = asinf((q->X * q->Z - q->Y * q->W) * -2.0f);

	float t1 = q->Y * q->Z + q->X * q->W;
	v->Z = atan2f(t1 + t1, -sqx - sqy + sqz + sqw);
}

//----------------------------------------------------
// EOF
