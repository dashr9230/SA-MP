#include "TEABlockEncryptor.h"
#include "CheckSum.h"
#include "Rand.h"
#include <assert.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#define OutputDebugString NULL
#endif

#define TEA_ROUNDS		32
#define TEA_XOR_MASK	0x5E94A3CF

unsigned int TEABlockEncryptor::initObsDelta = (0x9E3779B9 ^ TEA_XOR_MASK);

#ifdef _DEBUG
void DumpMemory(const char *preText, const unsigned char *data, unsigned int count)
{
	return;

	char buffer[2000];
	char temp[5];
	sprintf(buffer, "%s: ", preText);
	for(unsigned int i=0; i<count; i++)
	{
		sprintf(temp, "%02x ", data[i]);
		strcat(buffer, temp);
	}
	strcat(buffer, "\n");
	OutputDebugString(buffer);
}
#endif

TEABlockEncryptor::TEABlockEncryptor()
{
	initDelta = initObsDelta ^ TEA_XOR_MASK;
	initSum = initDelta * TEA_ROUNDS;

	keySet = false;
}

TEABlockEncryptor::~TEABlockEncryptor()
{
	// Note: This is not virtual
	//       Make it cascaded virtual before adding any code here!
}
	
bool TEABlockEncryptor::IsKeySet( void ) const
{
	return keySet;
}

void TEABlockEncryptor::SetKey( const unsigned char key[ 16 ] )
{
#ifdef _DEBUG
#ifdef SAMPSRV
	DumpMemory("SvKey", &key[0], 16);
#else
	DumpMemory("ClKey", &key[0], 16);
#endif
#endif

	key = key;
	keySet = true;
}

void TEABlockEncryptor::UnsetKey( void )
{
	keySet = false;
}

void TEABlockEncryptor::Encrypt( unsigned char *input, int inputLength, unsigned char *output, int *outputLength )
{
	unsigned char checkSum;
	unsigned int checkSumInt;
	unsigned char paddingBytes;
	unsigned char encodedPad;
	unsigned int totalLength;
	CheckSum checkSumCalculator;

#ifdef _DEBUG
	assert( keySet );
#endif

	assert( input && inputLength );

	// # of padding bytes
	totalLength = inputLength + sizeof( checkSum ) + sizeof( encodedPad );
	paddingBytes = 0;

	/*if (totalLength < 16)
	{
		paddingBytes = 16 - totalLength;
	}
	else 
	*/
	if ((totalLength % 8) != 0)
	{
		paddingBytes = 8 - (totalLength % 8);
	}

	*outputLength = totalLength + paddingBytes;

	// Randomize the pad size variable
	encodedPad = (unsigned char) randomMT();
	encodedPad <<= 4;
	encodedPad |= paddingBytes;

	// Write the data first, in case we are overwriting ourselves
	if ( input == output )
		memmove( output + sizeof( checkSum ) + sizeof( encodedPad ) + paddingBytes, input, inputLength );
	else
		memcpy( output + sizeof( checkSum ) + sizeof( encodedPad ) + paddingBytes, input, inputLength );

	// Write the pad size variable
	*(output + sizeof( checkSum )) = encodedPad;

	// Write the padding
	for (unsigned int index = 0; index < paddingBytes; index++ )
		*( output + sizeof( checkSum ) + sizeof( encodedPad ) + index ) = (unsigned char) randomMT();

	// Calculate the checksum on the data
	checkSumCalculator.Add( output + sizeof( checkSum ), inputLength + sizeof( encodedPad ) + paddingBytes );

	checkSumInt = checkSumCalculator.Get();
	checkSum = (unsigned char) ( (checkSumInt << 4) ^ (checkSumInt) );

	// Write checksum
	*(output) = checkSum;

#ifdef _DEBUG
	DumpMemory("Pre Encrypt", output, *outputLength);
#endif

	for(int i=0; i < (*outputLength); i+=8)
	{
		unsigned int &V0 = *(reinterpret_cast<unsigned int*>(output+i));
		unsigned int &V1 = *(reinterpret_cast<unsigned int*>(output+i+sizeof(unsigned int)));
		EncryptBlock(V0, V1);
		//*(reinterpret_cast<unsigned int*>(output+i)+0) = V0;
		//*(reinterpret_cast<unsigned int*>(output+i)+1) = V1;
	}

#ifdef _DEBUG
	DumpMemory("PostEncrypt", output, *outputLength);
#endif

}

bool TEABlockEncryptor::Decrypt( unsigned char *input, int inputLength, unsigned char *output, int *outputLength )
{
	unsigned char checkSum;
	unsigned int checkSumInt;
	unsigned char paddingBytes;
	unsigned char encodedPad;
	CheckSum checkSumCalculator;

#ifdef _DEBUG
	assert( keySet );
#endif

	if ( input == 0 || inputLength < 8 || ( inputLength % 8 ) != 0 )
	{
		return false;
	}

#ifdef _DEBUG
	DumpMemory("Pre Decrypt", input, inputLength);
#endif

	for(int i=0; i<inputLength; i+=8)
	{
		unsigned int &V0 = *(reinterpret_cast<unsigned int*>(input+i));
		unsigned int &V1 = *(reinterpret_cast<unsigned int*>(input+i+sizeof(unsigned int)));
		DecryptBlock(V0, V1);
		//*(reinterpret_cast<unsigned int*>(input+i)+0) = V0;
		//*(reinterpret_cast<unsigned int*>(input+i)+1) = V1;
	}

#ifdef _DEBUG
	DumpMemory("PostDecrypt", input, inputLength);
#endif

	// Read checksum
	checkSum = *(output);

	// Read the pad size variable
	encodedPad = *(output + sizeof( checkSum ));

	// Only care about the last 4 bits
	paddingBytes = encodedPad & 0x0F;

	// Get the data length
	*outputLength = inputLength - sizeof( checkSum ) - sizeof( encodedPad ) - paddingBytes;

	// Calculate the checksum on the data.
	checkSumCalculator.Add( input + sizeof( checkSum ), *outputLength + sizeof( encodedPad ) + paddingBytes );
	
	checkSumInt = checkSumCalculator.Get();
	checkSumInt = ( (checkSumInt << 4) ^ (checkSumInt) ) & 0xFF;

	if ( (unsigned char)checkSumInt != checkSum )
		return false;

	// Read the data
	if ( input == output )
		memmove( output, input + sizeof( checkSum ) + sizeof( encodedPad ) + paddingBytes, *outputLength );
	else
		memcpy( output, input + sizeof( checkSum ) + sizeof( encodedPad ) + paddingBytes, *outputLength );

	return true;

}

void TEABlockEncryptor::EncryptBlock(unsigned int &V0, unsigned int &V1)
{
	unsigned int sum = 0;

	for(unsigned int i=0; i<TEA_ROUNDS; i++) 
	{
		V0 += ((V1 << 4 ^ V1 >> 5) + V1) ^ (sum + key[sum & 3]);
		sum += initDelta;
		V1 += ((V0 << 4 ^ V0 >> 5) + V0) ^ (sum + key[sum>>11 & 3]);
	}
}

void TEABlockEncryptor::DecryptBlock(unsigned int &V0, unsigned int &V1)
{
	unsigned int sum = initSum;

    for(unsigned int i=0; i<TEA_ROUNDS; i++) 
	{
        V1 -= ((V0 << 4 ^ V0 >> 5) + V0) ^ (sum + key[sum>>11 & 3]);
        sum -= initDelta;
        V0 -= ((V1 << 4 ^ V1 >> 5) + V1) ^ (sum + key[sum & 3]);
    }
}
