
#include "main.h"
#include "mathutils.h"

struct struc_72 {
	//char _gap0[71];
	VEHICLEID VehicleID;
	WORD lrAnalog;
	WORD udAnalog;
	WORD wKeys;
	VECTOR field_8; // vecRoll?
	VECTOR field_14; // vecDirection?
	VECTOR vecPos;
	VECTOR vecMoveSpeed;
	float fCarHealth;
	BYTE bytePlayerHealth;
	BYTE bytePlayerArmour;
	BYTE byteCurrentWeapon : 6;
	BYTE byteSpecialKey : 2;
	BYTE byteSirenOn;

	char _gap40[3];

	DWORD field_43;
};

struct struc_73 {
	char _gap0[52];
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

int FUNC_2(FILE *a1, DWORD a2)
{
	int x = 0;

	if(0x38 * (a2 / 0x38) != a2)
	{
		return 0;
	}

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

int FUNC_3(char *a1)
{
	FILE *fp = fopen(a1, "wb");
	if(!fp) return 0;

	DWORD v1 = 1000;
	DWORD v2 = 1;
	fwrite(&v1, 1, sizeof(DWORD), fp);
	fwrite(&v2, 1, sizeof(DWORD), fp);

	INCAR_SYNC_DATA icSync;
	VECTOR v, vec1, vec2;
	MATRIX4X4 m;
	QUATERNION q;

	memset(&icSync, 0, sizeof(INCAR_SYNC_DATA));

	for(int i = 0; i != GLOBAL_1; i++)
	{
		icSync.VehicleID = GLOBAL_2[i].VehicleID;
        icSync.lrAnalog = GLOBAL_2[i].lrAnalog;
        icSync.udAnalog = GLOBAL_2[i].udAnalog;
        icSync.wKeys = GLOBAL_2[i].wKeys;
        icSync.byteCurrentWeapon = GLOBAL_2[i].byteCurrentWeapon;
        icSync.byteLandingGearState = 0;
        icSync.bytePlayerHealth = GLOBAL_2[i].bytePlayerHealth;
        icSync.bytePlayerArmour = GLOBAL_2[i].bytePlayerArmour;
        icSync.byteSirenOn = GLOBAL_2[i].byteSirenOn;
        icSync.fCarHealth = GLOBAL_2[i].fCarHealth;
        icSync.dwHydraThrustAngle = GLOBAL_2[i].field_43;
		icSync.vecPos = GLOBAL_2[i].vecPos;
		icSync.vecMoveSpeed = GLOBAL_2[i].vecMoveSpeed;
        icSync.TrailerID = 0;

		vec2 = GLOBAL_2[i].field_8;
		vec1 = GLOBAL_2[i].field_14;

		CrossProduct(&v, &vec1, &vec2);

		v.X *= -1.0f;
		v.Y *= -1.0f;
		v.Z *= -1.0f;

		m.right = vec2;
		m.up = vec1;
		m.at = v;

		MatrixToQuaternion(&m, &q);

		icSync.quatRotation = q;

		fwrite(&GLOBAL_3[i], 1, sizeof(DWORD), fp);
		fwrite(&icSync, 1, sizeof(INCAR_SYNC_DATA), fp);
	}
	fclose(fp);
	return 1;
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
