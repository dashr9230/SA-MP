
#include "../main.h"

using namespace RakNet;
extern CNetGame*	pNetGame;
extern CGame * pGame;

//----------------------------------------------------

// TODO: These script RPCs
void ScrUnkA7(RPCParameters *rpcParams) {}
void ScrUnk38(RPCParameters *rpcParams) {}
void ScrUnk90(RPCParameters *rpcParams) {}
void ScrUnk91(RPCParameters *rpcParams) {}
void ScrSetGravity(RPCParameters *rpcParams) {}
void ScrUnk93(RPCParameters *rpcParams) {}
void ScrUnk94(RPCParameters *rpcParams) {}
void ScrUnk95(RPCParameters *rpcParams) {}
void ScrUnk2C(RPCParameters *rpcParams) {}
void ScrUnk2D(RPCParameters *rpcParams) {}
void ScrUnk2E(RPCParameters *rpcParams) {}
void ScrUnk2F(RPCParameters *rpcParams) {}
void ScrUnk4F(RPCParameters *rpcParams) {}
void ScrUnk50(RPCParameters *rpcParams) {}
void ScrUnk63(RPCParameters *rpcParams) {}
void ScrUnk7A(RPCParameters *rpcParams) {}
void ScrUnk7B(RPCParameters *rpcParams) {}
void ScrUnk7C(RPCParameters *rpcParams) {}

void ScrUnk7D(RPCParameters *rpcParams)
{
	// nothing
}

void ScrUnk7E(RPCParameters *rpcParams) {}
void ScrUnk7F(RPCParameters *rpcParams) {}
void ScrUnk39(RPCParameters *rpcParams) {}
void ScrUnk4A(RPCParameters *rpcParams) {}
void ScrUnk4B(RPCParameters *rpcParams) {}
void ScrUnk85(RPCParameters *rpcParams) {}
void ScrUnk86(RPCParameters *rpcParams) {}
void ScrUnk87(RPCParameters *rpcParams) {}
void ScrUnk69(RPCParameters *rpcParams) {}
void ScrAddGangZone(RPCParameters *rpcParams) {}
void ScrRemoveGangZone(RPCParameters *rpcParams) {}
void ScrFlashGangZone(RPCParameters *rpcParams) {}
void ScrStopFlashGangZone(RPCParameters *rpcParams) {}
void ScrUnk56(RPCParameters *rpcParams) {}
void ScrUnk57(RPCParameters *rpcParams) {}
void ScrUnk58(RPCParameters *rpcParams) {}
void ScrUnk68(RPCParameters *rpcParams) {}
void ScrUnk59(RPCParameters *rpcParams) {}
void ScrUnk5A(RPCParameters *rpcParams) {}
void ScrUnk5B(RPCParameters *rpcParams) {}
void ScrUnk6F(RPCParameters *rpcParams) {}
void ScrUnk62(RPCParameters *rpcParams) {}
void ScrUnk5C(RPCParameters *rpcParams) {}
void ScrUnk96(RPCParameters *rpcParams) {}
void ScrUnk70(RPCParameters *rpcParams) {}
void ScrSetSpawnInfo(RPCParameters *rpcParams) {}
void ScrUnk45(RPCParameters *rpcParams) {}
void ScrUnk99(RPCParameters *rpcParams) {}
void ScrUnk0B(RPCParameters *rpcParams) {}
void ScrSetPlayerPos(RPCParameters *rpcParams) {}
void ScrUnk0D(RPCParameters *rpcParams) {}
void ScrUnk0E(RPCParameters *rpcParams) {}
void ScrPutPlayerInVehicle(RPCParameters *rpcParams) {}
void ScrRemovePlayerFromVehicle(RPCParameters *rpcParams) {}
void ScrSetPlayerColor(RPCParameters *rpcParams) {}
void ScrDisplayGameText(RPCParameters *rpcParams) {}
void ScrSetInterior(RPCParameters *rpcParams) {}
void ScrUnk9F(RPCParameters *rpcParams) {}
void ScrUnkA0(RPCParameters *rpcParams) {}
void ScrUnkA1(RPCParameters *rpcParams) {}
void ScrUnk0F(RPCParameters *rpcParams) {}
void ScrUnk10(RPCParameters *rpcParams) {}
void ScrUnk11(RPCParameters *rpcParams) {}
void ScrUnk12(RPCParameters *rpcParams) {}
void ScrSetPlayerFacingAngle(RPCParameters *rpcParams) {}
void ScrUnk14(RPCParameters *rpcParams) {}
void ScrUnk15(RPCParameters *rpcParams) {}
void ScrUnk16(RPCParameters *rpcParams) {}
void ScrUnk40(RPCParameters *rpcParams) {}
void ScrUnk41(RPCParameters *rpcParams) {}
void ScrUnk42(RPCParameters *rpcParams) {}
void ScrUnk37(RPCParameters *rpcParams) {}
void ScrUnk21(RPCParameters *rpcParams) {}
void ScrUnk23(RPCParameters *rpcParams) {}
void ScrUnk43(RPCParameters *rpcParams) {}
void ScrUnk71(RPCParameters *rpcParams) {}
void ScrUnk29(RPCParameters *rpcParams) {}
void ScrUnk2A(RPCParameters *rpcParams) {}
void ScrUnk2B(RPCParameters *rpcParams) {}
void ScrUnk51(RPCParameters *rpcParams) {}
void ScrUnk52(RPCParameters *rpcParams) {}
void ScrUnk53(RPCParameters *rpcParams) {}
void ScrUnk54(RPCParameters *rpcParams) {}
void ScrUnkA9(RPCParameters *rpcParams) {}
void ScrUnkAD(RPCParameters *rpcParams) {}
void ScrUnkAE(RPCParameters *rpcParams) {}
void ScrUnkAF(RPCParameters *rpcParams) {}
void ScrUnkB0(RPCParameters *rpcParams) {}
void ScrUnkB2(RPCParameters *rpcParams) {}
void ScrUnk30(RPCParameters *rpcParams) {}
void ScrInitMenu(RPCParameters *rpcParams) {}
void ScrShowMenu(RPCParameters *rpcParams) {}
void ScrHideMenu(RPCParameters *rpcParams) {}

//----------------------------------------------------

void ScrSetCameraPos(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	VECTOR vecPos;
	bsData.Read(vecPos.X);
	bsData.Read(vecPos.Y);
	bsData.Read(vecPos.Z);
	pGame->GetCamera()->SetPosition(vecPos.X,vecPos.Y,vecPos.Z,0.0f,0.0f,0.0f);
}

//----------------------------------------------------

void ScrSetCameraLookAt(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	VECTOR vecPos;
	BYTE byteType=0;
	bsData.Read(vecPos.X);
	bsData.Read(vecPos.Y);
	bsData.Read(vecPos.Z);
	bsData.Read(byteType);
	if(byteType < 1 || byteType > 2) {
		byteType = 2;
	}
	pGame->GetCamera()->LookAtPoint(vecPos.X,vecPos.Y,vecPos.Z,byteType);
}

//----------------------------------------------------

void ScrSetCameraBehindPlayer(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	pGame->GetCamera()->SetBehindPlayer();
}

//----------------------------------------------------

void RegisterScriptRPCs(RakClientInterface* pRakClient)
{
	REGISTER_STATIC_RPC(pRakClient, ScrUnkA7);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk38);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk90);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk91);
	REGISTER_STATIC_RPC(pRakClient, ScrSetGravity);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk93);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk94);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk95);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk2C);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk2D);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk2E);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk2F);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk4F);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk50);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk63);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk7A);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk7B);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk7C);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk7D);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk7E);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk7F);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk39);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk4A);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk4B);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk85);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk86);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk87);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk69);
	REGISTER_STATIC_RPC(pRakClient, ScrAddGangZone);
	REGISTER_STATIC_RPC(pRakClient, ScrRemoveGangZone);
	REGISTER_STATIC_RPC(pRakClient, ScrFlashGangZone);
	REGISTER_STATIC_RPC(pRakClient, ScrStopFlashGangZone);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk56);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk57);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk58);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk68);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk59);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk5A);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk5B);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk6F);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk62);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk5C);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk96);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk70);
	REGISTER_STATIC_RPC(pRakClient, ScrSetSpawnInfo);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk45);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk99);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk0B);
	REGISTER_STATIC_RPC(pRakClient, ScrSetPlayerPos);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk0D);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk0E);
	REGISTER_STATIC_RPC(pRakClient, ScrPutPlayerInVehicle);
	REGISTER_STATIC_RPC(pRakClient, ScrRemovePlayerFromVehicle);
	REGISTER_STATIC_RPC(pRakClient, ScrSetPlayerColor);
	REGISTER_STATIC_RPC(pRakClient, ScrDisplayGameText);
	REGISTER_STATIC_RPC(pRakClient, ScrSetInterior);
	REGISTER_STATIC_RPC(pRakClient, ScrSetCameraPos);
	REGISTER_STATIC_RPC(pRakClient, ScrSetCameraLookAt);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk9F);
	REGISTER_STATIC_RPC(pRakClient, ScrUnkA0);
	REGISTER_STATIC_RPC(pRakClient, ScrUnkA1);
	REGISTER_STATIC_RPC(pRakClient, ScrSetCameraBehindPlayer);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk0F);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk10);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk11);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk12);
	REGISTER_STATIC_RPC(pRakClient, ScrSetPlayerFacingAngle);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk14);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk15);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk16);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk40);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk41);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk42);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk37);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk21);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk23);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk43);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk71);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk29);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk2A);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk2B);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk51);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk52);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk53);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk54);
	REGISTER_STATIC_RPC(pRakClient, ScrUnkA9);
	REGISTER_STATIC_RPC(pRakClient, ScrUnkAD);
	REGISTER_STATIC_RPC(pRakClient, ScrUnkAE);
	REGISTER_STATIC_RPC(pRakClient, ScrUnkAF);
	REGISTER_STATIC_RPC(pRakClient, ScrUnkB0);
	REGISTER_STATIC_RPC(pRakClient, ScrUnkB2);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk30);
	REGISTER_STATIC_RPC(pRakClient, ScrInitMenu);
	REGISTER_STATIC_RPC(pRakClient, ScrShowMenu);
	REGISTER_STATIC_RPC(pRakClient, ScrHideMenu);
}

//----------------------------------------------------

void UnRegisterScriptRPCs(RakClientInterface* pRakClient)
{
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnkA7);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk4A);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk4B);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk85);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk86);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk87);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk69);
	UNREGISTER_STATIC_RPC(pRakClient, ScrAddGangZone);
	UNREGISTER_STATIC_RPC(pRakClient, ScrRemoveGangZone);
	UNREGISTER_STATIC_RPC(pRakClient, ScrFlashGangZone);
	UNREGISTER_STATIC_RPC(pRakClient, ScrStopFlashGangZone);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk56);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk57);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk58);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk68);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk59);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk5A);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk5B);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk6F);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk62);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk5C);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk96);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk70);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk38);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk90);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk91);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetGravity);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk93);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk94);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk95);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk2C);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk2D);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk2E);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk2F);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk4F);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk50);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk63);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk7A);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk7B);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk7C);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk7D);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk7E);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk7F);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk39);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk21);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk23);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk43);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk71);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk29);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk2A);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk2B);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetSpawnInfo);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk45);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk0B);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk99);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetPlayerPos);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk0D);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk0E);
	UNREGISTER_STATIC_RPC(pRakClient, ScrPutPlayerInVehicle);
	UNREGISTER_STATIC_RPC(pRakClient, ScrRemovePlayerFromVehicle);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetPlayerColor);
	UNREGISTER_STATIC_RPC(pRakClient, ScrDisplayGameText);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetInterior);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetCameraPos);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetCameraLookAt);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk9F);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnkA0);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnkA1);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetCameraBehindPlayer);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk0F);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk10);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk11);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk12);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetPlayerFacingAngle);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk14);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk15);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk16);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk40);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk41);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk37);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk51);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk52);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk53);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk54);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnkA9);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnkAD);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnkAE);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnkAF);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnkB0);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnkB2);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk30);
	UNREGISTER_STATIC_RPC(pRakClient, ScrInitMenu);
	UNREGISTER_STATIC_RPC(pRakClient, ScrShowMenu);
	UNREGISTER_STATIC_RPC(pRakClient, ScrHideMenu);
}

//----------------------------------------------------
