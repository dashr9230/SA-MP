
#include "../main.h"

void EnterVehicle(RPCParameters *rpcParams) {}
void ExitVehicle(RPCParameters *rpcParams) {}
void Unk6B(RPCParameters *rpcParams) {}
void Unk25(RPCParameters *rpcParams) {}
void Unk26(RPCParameters *rpcParams) {}
void Unk27(RPCParameters *rpcParams) {}
void UpdateScoresPingsIPs(RPCParameters *rpcParams) {}

void SvrStats(RPCParameters *rpcParams)
{
	// TODO: SvrStats
}

void Unk28(RPCParameters *rpcParams)
{
	// TODO: Unk28
}

void Unk82(RPCParameters *rpcParams)
{
	// TODO: Unk82
}

void ClientMessage(RPCParameters *rpcParams)
{
	// TODO: ClientMessage
}

void WorldTime(RPCParameters *rpcParams) {}
void Unk5F(RPCParameters *rpcParams) {}
void Unk3F(RPCParameters *rpcParams) {}
void Unk97(RPCParameters *rpcParams) {}
void ScmEvent(RPCParameters *rpcParams) {}
void Weather(RPCParameters *rpcParams) {}
void Unk1D(RPCParameters *rpcParams) {}
void Unk1E(RPCParameters *rpcParams) {}

void Unk89(RPCParameters *rpcParams)
{
	// TODO: Unk89
}

void Unk8A(RPCParameters *rpcParams)
{
	// TODO: Unk8A
}

void Unk8B(RPCParameters *rpcParams)
{
	// TODO: Unk8B
}

void Chat(RPCParameters *rpcParams)
{
	// TODO: Chat
}

void RequestClass(RPCParameters *rpcParams)
{
	// TODO: RequestClass
}

void RequestSpawn(RPCParameters *rpcParams)
{
	// TODO: RequestSpawn
}

void Unk20(RPCParameters *rpcParams)
{
	// TODO: Unk20
}

void UnkA6(RPCParameters *rpcParams)
{
	// TODO: UnkA6
}

void UnkA3(RPCParameters *rpcParams)
{
	// TODO: UnkA3
}

void UnkA4(RPCParameters *rpcParams)
{
	// TODO: UnkA4
}

void UnkA5(RPCParameters *rpcParams)
{
	// TODO: UnkA5
}

//----------------------------------------------------

void RegisterRPCs(RakClientInterface * pRakClient)
{
	REGISTER_STATIC_RPC(pRakClient,EnterVehicle);
	REGISTER_STATIC_RPC(pRakClient,ExitVehicle);
	REGISTER_STATIC_RPC(pRakClient,Unk6B);
	REGISTER_STATIC_RPC(pRakClient,Unk25);
	REGISTER_STATIC_RPC(pRakClient,Unk26);
	REGISTER_STATIC_RPC(pRakClient,Unk27);
	REGISTER_STATIC_RPC(pRakClient,UpdateScoresPingsIPs);
	REGISTER_STATIC_RPC(pRakClient,SvrStats);
	REGISTER_STATIC_RPC(pRakClient,Unk28);
	REGISTER_STATIC_RPC(pRakClient,Unk82);
	REGISTER_STATIC_RPC(pRakClient,ClientMessage);
	REGISTER_STATIC_RPC(pRakClient,WorldTime);
	REGISTER_STATIC_RPC(pRakClient,Unk5F);
	REGISTER_STATIC_RPC(pRakClient,Unk3F);
	REGISTER_STATIC_RPC(pRakClient,Unk97);
	REGISTER_STATIC_RPC(pRakClient,ScmEvent);
	REGISTER_STATIC_RPC(pRakClient,Weather);
	REGISTER_STATIC_RPC(pRakClient,Unk1D);
	REGISTER_STATIC_RPC(pRakClient,Unk1E);
	REGISTER_STATIC_RPC(pRakClient,Unk89);
	REGISTER_STATIC_RPC(pRakClient,Unk8A);
	REGISTER_STATIC_RPC(pRakClient,Unk8B);
	REGISTER_STATIC_RPC(pRakClient,Chat);
	REGISTER_STATIC_RPC(pRakClient,RequestClass);
	REGISTER_STATIC_RPC(pRakClient,RequestSpawn);
	REGISTER_STATIC_RPC(pRakClient,Unk20);
	REGISTER_STATIC_RPC(pRakClient,UnkA6);
	REGISTER_STATIC_RPC(pRakClient,UnkA3);
	REGISTER_STATIC_RPC(pRakClient,UnkA4);
	REGISTER_STATIC_RPC(pRakClient,UnkA5);
}

//----------------------------------------------------

void UnRegisterRPCs(RakClientInterface * pRakClient)
{
	UNREGISTER_STATIC_RPC(pRakClient,EnterVehicle);
	UNREGISTER_STATIC_RPC(pRakClient,ExitVehicle);
	UNREGISTER_STATIC_RPC(pRakClient,Unk6B);
	UNREGISTER_STATIC_RPC(pRakClient,Unk25);
	UNREGISTER_STATIC_RPC(pRakClient,Unk26);
	UNREGISTER_STATIC_RPC(pRakClient,Unk27);
	UNREGISTER_STATIC_RPC(pRakClient,UpdateScoresPingsIPs);
	UNREGISTER_STATIC_RPC(pRakClient,SvrStats);
	UNREGISTER_STATIC_RPC(pRakClient,Unk28);
	UNREGISTER_STATIC_RPC(pRakClient,Unk82);
	UNREGISTER_STATIC_RPC(pRakClient,ClientMessage);
	UNREGISTER_STATIC_RPC(pRakClient,WorldTime);
	UNREGISTER_STATIC_RPC(pRakClient,Unk5F);
	UNREGISTER_STATIC_RPC(pRakClient,Unk3F);
	UNREGISTER_STATIC_RPC(pRakClient,Unk97);
	UNREGISTER_STATIC_RPC(pRakClient,ScmEvent);
	UNREGISTER_STATIC_RPC(pRakClient,Weather);
	UNREGISTER_STATIC_RPC(pRakClient,Unk1D);
	UNREGISTER_STATIC_RPC(pRakClient,Unk1E);
	UNREGISTER_STATIC_RPC(pRakClient,Unk20);
	UNREGISTER_STATIC_RPC(pRakClient,UnkA6);
	UNREGISTER_STATIC_RPC(pRakClient,UnkA3);
	UNREGISTER_STATIC_RPC(pRakClient,UnkA4);
	UNREGISTER_STATIC_RPC(pRakClient,UnkA5);
	UNREGISTER_STATIC_RPC(pRakClient,Unk89);
	UNREGISTER_STATIC_RPC(pRakClient,Unk8A);
	UNREGISTER_STATIC_RPC(pRakClient,Unk8B);
	UNREGISTER_STATIC_RPC(pRakClient,Chat);
	UNREGISTER_STATIC_RPC(pRakClient,RequestClass);
	UNREGISTER_STATIC_RPC(pRakClient,RequestSpawn);
}

//----------------------------------------------------
