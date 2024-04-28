
#include "main.h"

RakServerInterface		*pRak=0;

void SvrStats(RPCParameters *rpcParams) {} // TODO: SvrStats
void SetInteriorId(RPCParameters *rpcParams) {} // TODO: SetInteriorId
void ScmEvent(RPCParameters *rpcParams) {} // TODO: ScmEvent
void ScriptCash(RPCParameters *rpcParams) {} // TODO: ScriptCash
void ClickMap(RPCParameters *rpcParams) {} // TODO: ClickMap
void VehicleDestroyed(RPCParameters *rpcParams) {} // TODO: VehicleDestroyed
void PickedUpWeapon(RPCParameters *rpcParams) {} // TODO: PickedUpWeapon
void PickedUpPickup(RPCParameters *rpcParams) {} // TODO: PickedUpPickup
void MenuSelect(RPCParameters *rpcParams) {} // TODO: MenuSelect
void MenuQuit(RPCParameters *rpcParams) {} // TODO: MenuQuit
void DialogResponse(RPCParameters *rpcParams) {} // TODO: DialogResponse
void ClickPlayer(RPCParameters *rpcParams) {} // TODO: ClickPlayer
void ClientJoin(RPCParameters *rpcParams) {} // TODO: ClientJoin
void NPCJoin(RPCParameters *rpcParams) {} // TODO: NPCJoin
void Chat(RPCParameters *rpcParams) {} // TODO: Chat
void RequestClass(RPCParameters *rpcParams) {} // TODO: RequestClass
void RequestSpawn(RPCParameters *rpcParams) {} // TODO: RequestSpawn
void Spawn(RPCParameters *rpcParams) {} // TODO: Spawn
void Death(RPCParameters *rpcParams) {} // TODO: Death
void DamageVehicle(RPCParameters *rpcParams) {} // TODO: DamageVehicle
void EnterVehicle(RPCParameters *rpcParams) {} // TODO: EnterVehicle
void ExitVehicle(RPCParameters *rpcParams) {} // TODO: ExitVehicle
void ServerCommand(RPCParameters *rpcParams) {} // TODO: ServerCommand
void UpdateScoresPingsIPs(RPCParameters *rpcParams) {} // TODO: UpdateScoresPingsIPs
void EditAttachedObject(RPCParameters *rpcParams) {} // TODO: EditAttachedObject
void EditObject(RPCParameters *rpcParams) {} // TODO: EditObject
void SelectObject(RPCParameters *rpcParams) {} // TODO: SelectObject
void ClickTextDraw(RPCParameters *rpcParams) {} // TODO: ClickTextDraw
void ClientCheck(RPCParameters *rpcParams) {} // TODO: ClientCheck
void CameraTarget(RPCParameters *rpcParams) {} // TODO: CameraTarget
void PlayerDamage(RPCParameters *rpcParams) {} // TODO: PlayerDamage
void ActorDamage(RPCParameters *rpcParams) {} // TODO: ActorDamage

//----------------------------------------------------

void RegisterRPCs(RakServerInterface * pRakServer)
{
	pRak = pRakServer;

	REGISTER_STATIC_RPC(pRakServer, SvrStats);
	REGISTER_STATIC_RPC(pRakServer, SetInteriorId);
	REGISTER_STATIC_RPC(pRakServer, ScmEvent);
	REGISTER_STATIC_RPC(pRakServer, ScriptCash);
	REGISTER_STATIC_RPC(pRakServer, ClickMap);
	REGISTER_STATIC_RPC(pRakServer, VehicleDestroyed);
	REGISTER_STATIC_RPC(pRakServer, PickedUpWeapon);
	REGISTER_STATIC_RPC(pRakServer, PickedUpPickup);
	REGISTER_STATIC_RPC(pRakServer, MenuSelect);
	REGISTER_STATIC_RPC(pRakServer, MenuQuit);
	REGISTER_STATIC_RPC(pRakServer, DialogResponse);
	REGISTER_STATIC_RPC(pRakServer, ClickPlayer);
	REGISTER_STATIC_RPC(pRakServer, ClientJoin);
	REGISTER_STATIC_RPC(pRakServer, NPCJoin);
	REGISTER_STATIC_RPC(pRakServer, Chat);
	REGISTER_STATIC_RPC(pRakServer, RequestClass);
	REGISTER_STATIC_RPC(pRakServer, RequestSpawn);
	REGISTER_STATIC_RPC(pRakServer, Spawn);
	REGISTER_STATIC_RPC(pRakServer, Death);
	REGISTER_STATIC_RPC(pRakServer, DamageVehicle);
	REGISTER_STATIC_RPC(pRakServer, EnterVehicle);
	REGISTER_STATIC_RPC(pRakServer, ExitVehicle);
	REGISTER_STATIC_RPC(pRakServer, ServerCommand);
	REGISTER_STATIC_RPC(pRakServer, UpdateScoresPingsIPs);
	REGISTER_STATIC_RPC(pRakServer, EditAttachedObject);
	REGISTER_STATIC_RPC(pRakServer, EditObject);
	REGISTER_STATIC_RPC(pRakServer, SelectObject);
	REGISTER_STATIC_RPC(pRakServer, ClickTextDraw);
	REGISTER_STATIC_RPC(pRakServer, ClientCheck);
	REGISTER_STATIC_RPC(pRakServer, CameraTarget);
	REGISTER_STATIC_RPC(pRakServer, PlayerDamage);
	REGISTER_STATIC_RPC(pRakServer, ActorDamage);
}

//----------------------------------------------------

void UnRegisterRPCs(RakServerInterface * pRakServer)
{
	UNREGISTER_STATIC_RPC(pRakServer, PlayerDamage);
	UNREGISTER_STATIC_RPC(pRakServer, SvrStats);
	UNREGISTER_STATIC_RPC(pRakServer, SetInteriorId);
	UNREGISTER_STATIC_RPC(pRakServer, ScmEvent);
	UNREGISTER_STATIC_RPC(pRakServer, ScriptCash);
	UNREGISTER_STATIC_RPC(pRakServer, ClickMap);
	UNREGISTER_STATIC_RPC(pRakServer, VehicleDestroyed);
	UNREGISTER_STATIC_RPC(pRakServer, PickedUpWeapon);
	UNREGISTER_STATIC_RPC(pRakServer, PickedUpPickup);
	UNREGISTER_STATIC_RPC(pRakServer, MenuSelect);
	UNREGISTER_STATIC_RPC(pRakServer, MenuQuit);
	UNREGISTER_STATIC_RPC(pRakServer, DialogResponse);
	UNREGISTER_STATIC_RPC(pRakServer, ClickPlayer);
	UNREGISTER_STATIC_RPC(pRakServer, ClientJoin);
	UNREGISTER_STATIC_RPC(pRakServer, NPCJoin);
	UNREGISTER_STATIC_RPC(pRakServer, Chat);
	UNREGISTER_STATIC_RPC(pRakServer, RequestClass);
	UNREGISTER_STATIC_RPC(pRakServer, RequestSpawn);
	UNREGISTER_STATIC_RPC(pRakServer, Spawn);
	UNREGISTER_STATIC_RPC(pRakServer, Death);
	UNREGISTER_STATIC_RPC(pRakServer, DamageVehicle);
	UNREGISTER_STATIC_RPC(pRakServer, EnterVehicle);
	UNREGISTER_STATIC_RPC(pRakServer, ExitVehicle);
	UNREGISTER_STATIC_RPC(pRakServer, ServerCommand);
	UNREGISTER_STATIC_RPC(pRakServer, UpdateScoresPingsIPs);
	UNREGISTER_STATIC_RPC(pRakServer, EditAttachedObject);
	UNREGISTER_STATIC_RPC(pRakServer, EditObject);
	UNREGISTER_STATIC_RPC(pRakServer, SelectObject);
	UNREGISTER_STATIC_RPC(pRakServer, ClickTextDraw);
	UNREGISTER_STATIC_RPC(pRakServer, ClientCheck);
	UNREGISTER_STATIC_RPC(pRakServer, CameraTarget);

	pRak = 0;
}

//----------------------------------------------------
