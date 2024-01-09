
#include "../main.h"

GAME_SCRIPT_THREAD* gst;
FARPROC ProcessOneCommand = (FARPROC)0x469EB0;
BYTE ScriptBuf[255];

DWORD dwScmOpcodeDebug=0;
BOOL bScmLocalDebug=FALSE;

int __declspec(naked) ExecuteScriptBuf()
{
	__asm
	{
		mov bScmLocalDebug, 1;

		mov eax, OFFSET ScriptBuf	// Move our script buffer base into eax.

		mov ecx, gst				// Move GAME_SCRIPT_THREAD structure into ecx.
		mov [ecx+0x14], eax			// Move eax into the gst->dwScriptIP.

		call ProcessOneCommand		// Call the game's script opcode processor.

		mov ecx, gst				// Move game script thread into ecx again.
		mov eax, [ecx+0xC5]			// Move the dwIfFlag into eax (return value).

		mov bScmLocalDebug, 0;

		ret							// return.
	}
}

int ScriptCommand(const SCRIPT_COMMAND* pScriptCommand, ...)
{
	// TODO: ScriptCommand
	return 0;
}

void InitScripting()
{
	gst = new GAME_SCRIPT_THREAD;
	ZeroMemory(gst, sizeof(GAME_SCRIPT_THREAD));
}
