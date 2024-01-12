
#include "../main.h"

// TODO: these RPCs
void Unk22(RPCParameters *rpcParams) {}
void Unk24(RPCParameters *rpcParams) {}
void Unk3A(RPCParameters *rpcParams) {}
void Unk3B(RPCParameters *rpcParams) {}
void Unk3D(RPCParameters *rpcParams) {}
void Unk6B(RPCParameters *rpcParams) {}
void Unk25(RPCParameters *rpcParams) {}
void Unk26(RPCParameters *rpcParams) {}
void Unk27(RPCParameters *rpcParams) {}
void Unk9B(RPCParameters *rpcParams) {}
void Unk66(RPCParameters *rpcParams) {}
void Unk28(RPCParameters *rpcParams) {}
void Unk82(RPCParameters *rpcParams) {}
void Unk5D(RPCParameters *rpcParams) {}
void Unk5E(RPCParameters *rpcParams) {}
void Unk5F(RPCParameters *rpcParams) {}
void Unk3F(RPCParameters *rpcParams) {}
void Unk97(RPCParameters *rpcParams) {}
void Unk60(RPCParameters *rpcParams) {}
void Unk98(RPCParameters *rpcParams) {}
void Unk1D(RPCParameters *rpcParams) {}
void Unk1E(RPCParameters *rpcParams) {}
void Unk3C(RPCParameters *rpcParams) {}
void Unk20(RPCParameters *rpcParams) {}
void UnkA6(RPCParameters *rpcParams) {}
void UnkA3(RPCParameters *rpcParams) {}
void UnkA4(RPCParameters *rpcParams) {}
void UnkA5(RPCParameters *rpcParams) {}
void Unk6A(RPCParameters *rpcParams) {}
void Unk18(RPCParameters *rpcParams) {}
void Unk1A(RPCParameters *rpcParams) {}
void Unk9A(RPCParameters *rpcParams) {}
void Unk89(RPCParameters *rpcParams) {}
void Unk8A(RPCParameters *rpcParams) {}
void Unk8B(RPCParameters *rpcParams) {}
void Unk65(RPCParameters *rpcParams) {}
void Unk80(RPCParameters *rpcParams) {}
void Unk81(RPCParameters *rpcParams) {}
void Unk74(RPCParameters *rpcParams) {}
void Unk75(RPCParameters *rpcParams) {}
void Unk1B(RPCParameters *rpcParams) {}
void Unk1C(RPCParameters *rpcParams) {}
void UnkAA(RPCParameters *rpcParams) {}
void Unk67(RPCParameters *rpcParams) {}
void UnkAB(RPCParameters *rpcParams) {}
void UnkAC(RPCParameters *rpcParams) {}

//----------------------------------------------------

void RegisterRPCs(RakClientInterface * pRakClient)
{
	REGISTER_STATIC_RPC(pRakClient,Unk22);
	REGISTER_STATIC_RPC(pRakClient,Unk24);
	REGISTER_STATIC_RPC(pRakClient,Unk3A);
	REGISTER_STATIC_RPC(pRakClient,Unk3B);
	REGISTER_STATIC_RPC(pRakClient,Unk3D);
	REGISTER_STATIC_RPC(pRakClient,Unk6B);
	REGISTER_STATIC_RPC(pRakClient,Unk25);
	REGISTER_STATIC_RPC(pRakClient,Unk26);
	REGISTER_STATIC_RPC(pRakClient,Unk27);
	REGISTER_STATIC_RPC(pRakClient,Unk9B);
	REGISTER_STATIC_RPC(pRakClient,Unk66);
	REGISTER_STATIC_RPC(pRakClient,Unk28);
	REGISTER_STATIC_RPC(pRakClient,Unk82);
	REGISTER_STATIC_RPC(pRakClient,Unk5D);
	REGISTER_STATIC_RPC(pRakClient,Unk5E);
	REGISTER_STATIC_RPC(pRakClient,Unk5F);
	REGISTER_STATIC_RPC(pRakClient,Unk3F);
	REGISTER_STATIC_RPC(pRakClient,Unk97);
	REGISTER_STATIC_RPC(pRakClient,Unk60);
	REGISTER_STATIC_RPC(pRakClient,Unk98);
	REGISTER_STATIC_RPC(pRakClient,Unk1D);
	REGISTER_STATIC_RPC(pRakClient,Unk1E);
	REGISTER_STATIC_RPC(pRakClient,Unk3C);
	REGISTER_STATIC_RPC(pRakClient,Unk20);
	REGISTER_STATIC_RPC(pRakClient,UnkA6);
	REGISTER_STATIC_RPC(pRakClient,UnkA3);
	REGISTER_STATIC_RPC(pRakClient,UnkA4);
	REGISTER_STATIC_RPC(pRakClient,UnkA5);
	REGISTER_STATIC_RPC(pRakClient,Unk6A);
	REGISTER_STATIC_RPC(pRakClient,Unk18);
	REGISTER_STATIC_RPC(pRakClient,Unk1A);
	REGISTER_STATIC_RPC(pRakClient,Unk9A);
	REGISTER_STATIC_RPC(pRakClient,Unk89);
	REGISTER_STATIC_RPC(pRakClient,Unk8A);
	REGISTER_STATIC_RPC(pRakClient,Unk8B);
	REGISTER_STATIC_RPC(pRakClient,Unk65);
	REGISTER_STATIC_RPC(pRakClient,Unk80);
	REGISTER_STATIC_RPC(pRakClient,Unk81);
	REGISTER_STATIC_RPC(pRakClient,Unk74);
	REGISTER_STATIC_RPC(pRakClient,Unk75);
	REGISTER_STATIC_RPC(pRakClient,Unk1B);
	REGISTER_STATIC_RPC(pRakClient,Unk1C);
	REGISTER_STATIC_RPC(pRakClient,UnkAA);
	REGISTER_STATIC_RPC(pRakClient,Unk67);
	REGISTER_STATIC_RPC(pRakClient,UnkAB);
	REGISTER_STATIC_RPC(pRakClient,UnkAC);
}

//----------------------------------------------------

void UnRegisterRPCs(RakClientInterface * pRakClient)
{
	UNREGISTER_STATIC_RPC(pRakClient,Unk3C);
	UNREGISTER_STATIC_RPC(pRakClient,Unk20);
	UNREGISTER_STATIC_RPC(pRakClient,UnkA6);
	UNREGISTER_STATIC_RPC(pRakClient,UnkA3);
	UNREGISTER_STATIC_RPC(pRakClient,UnkA4);
	UNREGISTER_STATIC_RPC(pRakClient,UnkA5);
	UNREGISTER_STATIC_RPC(pRakClient,Unk18);
	UNREGISTER_STATIC_RPC(pRakClient,Unk80);
	UNREGISTER_STATIC_RPC(pRakClient,Unk81);
	UNREGISTER_STATIC_RPC(pRakClient,Unk6A);
	UNREGISTER_STATIC_RPC(pRakClient,Unk1A);
	UNREGISTER_STATIC_RPC(pRakClient,Unk9A);
	UNREGISTER_STATIC_RPC(pRakClient,Unk22);
	UNREGISTER_STATIC_RPC(pRakClient,Unk24);
	UNREGISTER_STATIC_RPC(pRakClient,Unk3A);
	UNREGISTER_STATIC_RPC(pRakClient,Unk3B);
	UNREGISTER_STATIC_RPC(pRakClient,Unk3D);
	UNREGISTER_STATIC_RPC(pRakClient,Unk89);
	UNREGISTER_STATIC_RPC(pRakClient,Unk8A);
	UNREGISTER_STATIC_RPC(pRakClient,Unk8B);
	UNREGISTER_STATIC_RPC(pRakClient,Unk65);
	UNREGISTER_STATIC_RPC(pRakClient,Unk6B);
	UNREGISTER_STATIC_RPC(pRakClient,Unk25);
	UNREGISTER_STATIC_RPC(pRakClient,Unk26);
	UNREGISTER_STATIC_RPC(pRakClient,Unk27);
	UNREGISTER_STATIC_RPC(pRakClient,Unk9B);
	UNREGISTER_STATIC_RPC(pRakClient,Unk66);
	UNREGISTER_STATIC_RPC(pRakClient,Unk28);
	UNREGISTER_STATIC_RPC(pRakClient,Unk82);
	UNREGISTER_STATIC_RPC(pRakClient,Unk5D);
	UNREGISTER_STATIC_RPC(pRakClient,Unk5E);
	UNREGISTER_STATIC_RPC(pRakClient,Unk5F);
	UNREGISTER_STATIC_RPC(pRakClient,Unk3F);
	UNREGISTER_STATIC_RPC(pRakClient,Unk97);
	UNREGISTER_STATIC_RPC(pRakClient,Unk60);
	UNREGISTER_STATIC_RPC(pRakClient,Unk98);
	UNREGISTER_STATIC_RPC(pRakClient,Unk1D);
	UNREGISTER_STATIC_RPC(pRakClient,Unk1E);
	UNREGISTER_STATIC_RPC(pRakClient,Unk74);
	UNREGISTER_STATIC_RPC(pRakClient,Unk75);
	UNREGISTER_STATIC_RPC(pRakClient,Unk1B);
	UNREGISTER_STATIC_RPC(pRakClient,Unk1C);
	UNREGISTER_STATIC_RPC(pRakClient,UnkAA);
	UNREGISTER_STATIC_RPC(pRakClient,Unk67);
	UNREGISTER_STATIC_RPC(pRakClient,UnkAB);
	UNREGISTER_STATIC_RPC(pRakClient,UnkAC);
}

//----------------------------------------------------
