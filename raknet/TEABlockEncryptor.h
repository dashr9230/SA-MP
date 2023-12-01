// TODO: Implement TEABlockEncryptor.h

#ifndef __TEA_BLOCK_ENCRYPTOR_H
#define __TEA_BLOCK_ENCRYPTOR_H

#include "DataBlockEncryptor.h"

class TEABlockEncryptor :
	public DataBlockEncryptor
{

public:

	TEABlockEncryptor();

protected:

	unsigned int initSum;
	unsigned int initDelta;

	static unsigned int initObsDelta;

};

#endif
