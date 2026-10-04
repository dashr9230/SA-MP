#pragma once

// only SetKey is reconstructed; it is all `CNetGame::CNetGame` needs
class SocketDataEncryptor
{
public:
	static void SetKey(int iPort);
};
