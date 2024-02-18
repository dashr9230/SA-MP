
#pragma once

#define MAX_SCRIPT_VARS	16	// Size of our variable saving array

// Super Simplified SA Version
struct GAME_SCRIPT_THREAD	// 0xE0 bytes total.
{
	BYTE _gap0[224];
};

struct SCRIPT_COMMAND		//	Params
{							//		i = integer
	WORD OpCode;			//		f = float
	char Params[MAX_SCRIPT_VARS]; //	v = variable
};							//		s = string

int ScriptCommand(const SCRIPT_COMMAND* pScriptCommand, ...);	// The main scripting function. See notes.


const SCRIPT_COMMAND request_model						= { 0x0247, "i" };		// (CAR_*|BIKE_*|BOAT_*|WEAPON_*|OBJECT_*)
const SCRIPT_COMMAND load_requested_models				= { 0x038B, "" };		// -/-

const SCRIPT_COMMAND fade								= { 0x016A, "ii" };		// (time in ms), FADE_*
const SCRIPT_COMMAND is_model_available					= { 0x0248, "i" };		// #MODEL

const SCRIPT_COMMAND set_camera_behind_player			= { 0x0373, "" };		// -/-
const SCRIPT_COMMAND point_camera						= { 0x0160, "fffi" };	// x, y, z, type
const SCRIPT_COMMAND restore_camera_jumpcut				= { 0x02EB, "" };		// -/-
const SCRIPT_COMMAND set_camera_position				= { 0x015F, "ffffff" }; // x, y, z, vx, vy, vz

const SCRIPT_COMMAND set_actor_can_be_decapitated		= { 0x0446, "ii" };

const SCRIPT_COMMAND enable_train_traffic				= { 0x06d7, "i" };

const SCRIPT_COMMAND refresh_streaming_at				= { 0x04E4, "ff" };

const SCRIPT_COMMAND request_animation					= { 0x04ED, "s" };
const SCRIPT_COMMAND is_animation_loaded				= { 0x04EE, "s" };
const SCRIPT_COMMAND release_animation					= { 0x04EF, "s" };

const SCRIPT_COMMAND set_actor_weapon_droppable 		= { 0x087e, "ii" };

const SCRIPT_COMMAND restore_camera_to_user_defined = { 0x925, "" };
const SCRIPT_COMMAND set_camera_position_to = { 0x936, "ffffffii" };
const SCRIPT_COMMAND point_camera_transverse_to = { 0x920, "ffffffii" };
const SCRIPT_COMMAND lock_camera_target_point = { 0x92F, "i" };
const SCRIPT_COMMAND lock_camera_position = { 0x930, "i" };


