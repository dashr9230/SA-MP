
bool FUNC_1(PVECTOR vec); // unused
bool FUNC_2(PVECTOR vec);
bool FUNC_3(PVECTOR vec);
float GetLength(PVECTOR vec);
void CrossProduct(PVECTOR out, PVECTOR vec1, PVECTOR vec2); // unused
void Transform(PVECTOR out, PMATRIX4X4 mat, PVECTOR vec); // unused
void MatrixToQuaternion(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22, float *qw, float *qx, float *qy, float *qz);
void QuaternionToMatrix(float qw, float qx, float qy, float qz, float *m00, float *m01, float *m02, float *m10, float *m11, float *m12, float *m20, float *m21, float *m22);
void MatrixToQuaternion(PMATRIX4X4 mat, PQUATERNION quat);
void QuaternionToMatrix(PQUATERNION quat, PMATRIX4X4 mat);
float GetNormalisation(PVECTOR vec);
void ToQuaternion(PVECTOR vec, PQUATERNION quat); // unused
void ToEulerAngles(PQUATERNION quat, PVECTOR vec); // unused

