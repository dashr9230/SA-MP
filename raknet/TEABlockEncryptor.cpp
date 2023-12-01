// TODO: Implement TEABlockEncryptor.cpp

#include "TEABlockEncryptor.h"

#define TEA_ROUNDS		32
#define TEA_XOR_MASK	0x5E94A3CF

unsigned int TEABlockEncryptor::initObsDelta = (0x9E3779B9 ^ TEA_XOR_MASK);

TEABlockEncryptor::TEABlockEncryptor()
{
	initDelta = initObsDelta ^ TEA_XOR_MASK;
	initSum = initDelta * TEA_ROUNDS;

	keySet = false;
}
