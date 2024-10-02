
#include "main.h"
#include "mathutils.h"

struct struc_72 {
	//char _gap0[71];
	WORD field_0;
	WORD field_2;
	WORD field_4;
	WORD field_6;
	VECTOR field_8;
	VECTOR field_14;
	DWORD field_20;
	DWORD field_24;
	DWORD field_28;
	DWORD field_2C;
	DWORD field_30;
	DWORD field_34;
	DWORD field_38;
	BYTE field_3C;
	BYTE field_3D;
	BYTE field_3E;
	BYTE field_3F;

	char _gap40[3];

	DWORD field_43;
};

struct struc_73 {
	char _gap0[52];
};

struct struc_74 {
	//char _gap0[63];
	WORD field_0;
	WORD field_2;
	WORD field_4;
	WORD field_6;
	QUATERNION field_8;
	DWORD field_18;
	DWORD field_1C;
	DWORD field_20;
	DWORD field_24;
	DWORD field_28;
	DWORD field_2C;
	DWORD field_30;
	BYTE field_34;
	BYTE field_35;
	BYTE field_36;
	BYTE field_37;
	BYTE field_38;
	WORD field_39;
	DWORD field_3B;
};

DWORD *GLOBAL_3 = NULL;
struc_73 *GLOBAL_4 = NULL;
struc_72 *GLOBAL_2 = NULL;
int GLOBAL_1;

int FUNC_1(FILE *a1, DWORD a2)
{
	int x = 0;

	if(0x4B * (a2 / 0x4B) != a2)
	{
		return 0;
	}

	GLOBAL_1 = a2 / 0x4B;
	if(GLOBAL_2)
		free(GLOBAL_2);
	if(GLOBAL_3)
		free(GLOBAL_3);
	GLOBAL_2 = (struc_72 *)calloc(1, sizeof(struc_72) * GLOBAL_1);
	GLOBAL_3 = (DWORD *)calloc(1, sizeof(DWORD) * GLOBAL_1);
	while(!feof(a1))
	{
		fread(&GLOBAL_3[x], 1, sizeof(DWORD), a1);
		fread(&GLOBAL_2[x], 1, sizeof(struc_72), a1);
		x++;
	}
	return 1;
}

int FUNC_2(FILE *a1, int a2)
{
	int x = 0;

	if(0x38 * (a2 / 0x38) == a2)
	{
		GLOBAL_1 = a2 / 0x38;
		if(GLOBAL_4)
			free(GLOBAL_4);
		if(GLOBAL_3)
			free(GLOBAL_3);
		GLOBAL_4 = (struc_73 *)calloc(1, sizeof(struc_73) * GLOBAL_1);
		GLOBAL_3 = (DWORD *)calloc(1, sizeof(DWORD) * GLOBAL_1);
		while(!feof(a1))
		{
			fread(&GLOBAL_3[x], 1, sizeof(DWORD), a1);
			fread(&GLOBAL_4[x], 1, sizeof(struc_73), a1);
			x++;
		}
		return 1;
	}
	else
	{
		return 0;
	}
}

int FUNC_3(char *a1)
{
	FILE *fp = fopen(a1, "wb");
	if ( fp )
	{
		DWORD v1 = 1000;
		DWORD v2 = 1;
		fwrite(&v1, 1, sizeof(DWORD), fp);
		fwrite(&v2, 1, sizeof(DWORD), fp);

		struc_74 s;
		VECTOR v, vec1, vec2;
		MATRIX4X4 m;
		QUATERNION q;

		memset(&s, 0, sizeof(struc_74));

		for(int i = 0; i != GLOBAL_1; i++)
		{
			s.field_0 = GLOBAL_2[i].field_0;
			s.field_2 = GLOBAL_2[i].field_2;
			s.field_4 = GLOBAL_2[i].field_4;
			s.field_6 = GLOBAL_2[i].field_6;
			s.field_36 = GLOBAL_2[i].field_3E & 0x3F | s.field_36 & 0xC0;
			s.field_38 = 0;
			s.field_34 = GLOBAL_2[i].field_3C;
			s.field_35 = GLOBAL_2[i].field_3D;
			s.field_37 = GLOBAL_2[i].field_3F;
			s.field_30 = GLOBAL_2[i].field_38;
			s.field_3B = GLOBAL_2[i].field_43;
			s.field_18 = GLOBAL_2[i].field_20;
			s.field_1C = GLOBAL_2[i].field_24;
			s.field_20 = GLOBAL_2[i].field_28;
			s.field_24 = GLOBAL_2[i].field_2C;
			s.field_28 = GLOBAL_2[i].field_30;
			s.field_2C = GLOBAL_2[i].field_34;
			s.field_39 = 0;

			vec2.X = GLOBAL_2[i].field_8.X;
			vec2.Y = GLOBAL_2[i].field_8.Y;
			vec2.Z = GLOBAL_2[i].field_8.Z;
			vec1.X = GLOBAL_2[i].field_14.X;
			vec1.Y = GLOBAL_2[i].field_14.Y;
			vec1.Z = GLOBAL_2[i].field_14.Z;
			CrossProduct(&v, &vec1, &vec2);

			v.X *= -1.0f;
			v.Y *= -1.0f;
			v.Z *= -1.0f;

			m.right.X = vec2.X;
			m.right.Y = vec2.Y;
			m.right.Z = vec2.Z;
			m.up.X = vec1.X;
			m.up.Y = vec1.Y;
			m.up.Z = vec1.Z;
			m.at.X = v.X;
			m.at.Y = v.Y;
			m.at.Z = v.Z;

			MatrixToQuaternion(&m, &q);

			s.field_8.X = q.X;
			s.field_8.Y = q.Y;
			s.field_8.Z = q.Z;
			s.field_8.W = q.W;

			fwrite(&GLOBAL_3[i], 1, sizeof(DWORD), fp);
			fwrite(&s, 1, sizeof(struc_74), fp);
		}

		fclose(fp);
		return 1;
	}
	else
	{
		return 0;
	}
}

int FUNC_4(char *a1)
{
	FILE *fp = fopen(a1, "wb");
	if(!fp) return 0;

	DWORD v3 = 1000;
	DWORD v4 = 2;
	fwrite(&v3, 1, sizeof(DWORD), fp);
	fwrite(&v4, 1, sizeof(DWORD), fp);
	for(int i = 0; i != GLOBAL_1; i++)
	{
		fwrite(&GLOBAL_3[i], 1, 4, fp);
		fwrite(&GLOBAL_4[i], 1, 68, fp);
	}
	fclose(fp);
	return 1;
}

int UpgradeRecordFile(char *a1, int a2, int a3)
{
	FILE *fp = fopen(a1, "rb");
	if(!fp) return 0;

	if(a3 == 1)
	{
		if(FUNC_1(fp, a2))
		{
			fclose(fp);
			if(FUNC_3(a1))
				return 1;
			else
				return 0;
		}
		else
		{
			fclose(fp);
			return  0;
		}
	}
	else if (a3 == 2)
	{
		if(!FUNC_2(fp, a2))
		{
			fclose(fp);
			return 0;
		}
		else
		{
			fclose(fp);
			if(FUNC_4(a1))
				return 1;
			else
				return 0;
		}
	}
	else
	{
		return 0;
	}
}
