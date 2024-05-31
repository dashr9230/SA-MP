
#pragma once

//----------------------------------------------------

void CrossProduct(PVECTOR result, PVECTOR v1, PVECTOR v2);
void TransformVector(PVECTOR result, PMATRIX4X4 m, PVECTOR v);
void QuaternionToMatrix(PQUATERNION q, PMATRIX4X4 m);
void MatrixToQuaternion(PMATRIX4X4 m, PQUATERNION q);
void EulerToQuaternion(PVECTOR v, PQUATERNION q);
void QuaternionToEuler(PQUATERNION q, PVECTOR v);

//----------------------------------------------------
