
#ifndef __SOCKET_DATA_ENCRYPTOR_H
#define __SOCKET_DATA_ENCRYPTOR_H

class SocketDataEncryptor
{
private:
	static unsigned char GetChecksum(unsigned char *data, int length);
	static void Process(unsigned char key1, unsigned char key2, unsigned char *data, int length);

	static unsigned short session_key;

public:
	static void Encrypt(unsigned char *destination, unsigned char *source, int *length);
	static int Decrypt(unsigned char *destination, unsigned char *source, int *length);

	static void SetKey(int key);

};

#endif
