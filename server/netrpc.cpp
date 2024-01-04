
#include "main.h"

RakServerInterface		*pRak=0;

void Unk66(RPCParameters *rpcParams) {} // TODO: Unk66
void Unk76(RPCParameters *rpcParams) {} // TODO: Unk76
void Unk60(RPCParameters *rpcParams) {} // TODO: Unk60
void Unk1F(RPCParameters *rpcParams) {} // TODO: Unk1F
void Unk77(RPCParameters *rpcParams) {} // TODO: Unk77
void Unk88(RPCParameters *rpcParams) {} // TODO: Unk88
void Unk61(RPCParameters *rpcParams) {} // TODO: Unk61
void Unk83(RPCParameters *rpcParams) {} // TODO: Unk83
void Unk84(RPCParameters *rpcParams) {} // TODO: Unk84
void Unk8C(RPCParameters *rpcParams) {} // TODO: Unk8C
void Unk3E(RPCParameters *rpcParams) {} // TODO: Unk3E
void Unk17(RPCParameters *rpcParams) {} // TODO: Unk17
void Unk19(RPCParameters *rpcParams) {} // TODO: Unk19
void Unk36(RPCParameters *rpcParams) {} // TODO: Unk36
void Unk65(RPCParameters *rpcParams) {} // TODO: Unk65
void Unk80(RPCParameters *rpcParams) {} // TODO: Unk80
void Unk81(RPCParameters *rpcParams) {} // TODO: Unk81
void Unk34(RPCParameters *rpcParams) {} // TODO: Unk34
void Unk35(RPCParameters *rpcParams) {} // TODO: Unk35
void Unk6A(RPCParameters *rpcParams) {} // TODO: Unk6A
void Unk1A(RPCParameters *rpcParams) {} // TODO: Unk1A
void Unk9A(RPCParameters *rpcParams) {} // TODO: Unk9A
void Unk32(RPCParameters *rpcParams) {} // TODO: Unk32
void Unk9B(RPCParameters *rpcParams) {} // TODO: Unk9B
void Unk74(RPCParameters *rpcParams) {} // TODO: Unk74
void Unk75(RPCParameters *rpcParams) {} // TODO: Unk75
void Unk1B(RPCParameters *rpcParams) {} // TODO: Unk1B
void Unk53(RPCParameters *rpcParams) {} // TODO: Unk53
void Unk67(RPCParameters *rpcParams) {} // TODO: Unk67
void UnkA8(RPCParameters *rpcParams) {} // TODO: UnkA8
void Unk73(RPCParameters *rpcParams) {} // TODO: Unk73
void UnkB1(RPCParameters *rpcParams) {} // TODO: UnkB1

//----------------------------------------------------

void RegisterRPCs(RakServerInterface * pRakServer)
{
	pRak = pRakServer;

	REGISTER_STATIC_RPC(pRakServer, Unk66);
	REGISTER_STATIC_RPC(pRakServer, Unk76);
	REGISTER_STATIC_RPC(pRakServer, Unk60);
	REGISTER_STATIC_RPC(pRakServer, Unk1F);
	REGISTER_STATIC_RPC(pRakServer, Unk77);
	REGISTER_STATIC_RPC(pRakServer, Unk88);
	REGISTER_STATIC_RPC(pRakServer, Unk61);
	REGISTER_STATIC_RPC(pRakServer, Unk83);
	REGISTER_STATIC_RPC(pRakServer, Unk84);
	REGISTER_STATIC_RPC(pRakServer, Unk8C);
	REGISTER_STATIC_RPC(pRakServer, Unk3E);
	REGISTER_STATIC_RPC(pRakServer, Unk17);
	REGISTER_STATIC_RPC(pRakServer, Unk19);
	REGISTER_STATIC_RPC(pRakServer, Unk36);
	REGISTER_STATIC_RPC(pRakServer, Unk65);
	REGISTER_STATIC_RPC(pRakServer, Unk80);
	REGISTER_STATIC_RPC(pRakServer, Unk81);
	REGISTER_STATIC_RPC(pRakServer, Unk34);
	REGISTER_STATIC_RPC(pRakServer, Unk35);
	REGISTER_STATIC_RPC(pRakServer, Unk6A);
	REGISTER_STATIC_RPC(pRakServer, Unk1A);
	REGISTER_STATIC_RPC(pRakServer, Unk9A);
	REGISTER_STATIC_RPC(pRakServer, Unk32);
	REGISTER_STATIC_RPC(pRakServer, Unk9B);
	REGISTER_STATIC_RPC(pRakServer, Unk74);
	REGISTER_STATIC_RPC(pRakServer, Unk75);
	REGISTER_STATIC_RPC(pRakServer, Unk1B);
	REGISTER_STATIC_RPC(pRakServer, Unk53);
	REGISTER_STATIC_RPC(pRakServer, Unk67);
	REGISTER_STATIC_RPC(pRakServer, UnkA8);
	REGISTER_STATIC_RPC(pRakServer, Unk73);
	REGISTER_STATIC_RPC(pRakServer, UnkB1);
}

//----------------------------------------------------

void UnRegisterRPCs(RakServerInterface * pRakServer)
{
	UNREGISTER_STATIC_RPC(pRakServer, Unk73);
	UNREGISTER_STATIC_RPC(pRakServer, Unk66);
	UNREGISTER_STATIC_RPC(pRakServer, Unk76);
	UNREGISTER_STATIC_RPC(pRakServer, Unk60);
	UNREGISTER_STATIC_RPC(pRakServer, Unk1F);
	UNREGISTER_STATIC_RPC(pRakServer, Unk77);
	UNREGISTER_STATIC_RPC(pRakServer, Unk88);
	UNREGISTER_STATIC_RPC(pRakServer, Unk61);
	UNREGISTER_STATIC_RPC(pRakServer, Unk83);
	UNREGISTER_STATIC_RPC(pRakServer, Unk84);
	UNREGISTER_STATIC_RPC(pRakServer, Unk8C);
	UNREGISTER_STATIC_RPC(pRakServer, Unk3E);
	UNREGISTER_STATIC_RPC(pRakServer, Unk17);
	UNREGISTER_STATIC_RPC(pRakServer, Unk19);
	UNREGISTER_STATIC_RPC(pRakServer, Unk36);
	UNREGISTER_STATIC_RPC(pRakServer, Unk65);
	UNREGISTER_STATIC_RPC(pRakServer, Unk80);
	UNREGISTER_STATIC_RPC(pRakServer, Unk81);
	UNREGISTER_STATIC_RPC(pRakServer, Unk34);
	UNREGISTER_STATIC_RPC(pRakServer, Unk35);
	UNREGISTER_STATIC_RPC(pRakServer, Unk6A);
	UNREGISTER_STATIC_RPC(pRakServer, Unk1A);
	UNREGISTER_STATIC_RPC(pRakServer, Unk9A);
	UNREGISTER_STATIC_RPC(pRakServer, Unk32);
	UNREGISTER_STATIC_RPC(pRakServer, Unk9B);
	UNREGISTER_STATIC_RPC(pRakServer, Unk74);
	UNREGISTER_STATIC_RPC(pRakServer, Unk75);
	UNREGISTER_STATIC_RPC(pRakServer, Unk1B);
	UNREGISTER_STATIC_RPC(pRakServer, Unk53);
	UNREGISTER_STATIC_RPC(pRakServer, Unk67);
	UNREGISTER_STATIC_RPC(pRakServer, UnkA8);

	pRak = 0;
}

//----------------------------------------------------
