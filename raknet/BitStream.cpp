// TODO: Implement BitStream.cpp

#include "BitStream.h"
#include <stdlib.h>
#include <string.h>

using namespace RakNet;

BitStream::BitStream()
{
	numberOfBitsUsed = 0;
	//numberOfBitsAllocated = 32 * 8;
	numberOfBitsAllocated = BITSTREAM_STACK_ALLOCATION_SIZE * 8;
	readOffset = 0;
	//data = ( unsigned char* ) malloc( 32 );
	data = ( unsigned char* ) stackData;

#ifdef _DEBUG
//	assert( data );
#endif
	//memset(data, 0, 32);
	copyData = true;
}

BitStream::BitStream( int initialBytesToAllocate )
{
	numberOfBitsUsed = 0;
	readOffset = 0;
	if (initialBytesToAllocate <= BITSTREAM_STACK_ALLOCATION_SIZE)
	{
		data = ( unsigned char* ) stackData;
		numberOfBitsAllocated = BITSTREAM_STACK_ALLOCATION_SIZE * 8;
	}
	else
	{
		data = ( unsigned char* ) malloc( initialBytesToAllocate );
		numberOfBitsAllocated = initialBytesToAllocate << 3;
	}
#ifdef _DEBUG
	assert( data );
#endif
	// memset(data, 0, initialBytesToAllocate);
	copyData = true;
}

BitStream::BitStream( unsigned char* _data, unsigned int lengthInBytes, bool _copyData )
{
	numberOfBitsUsed = lengthInBytes << 3;
	readOffset = 0;
	copyData = _copyData;
	numberOfBitsAllocated = lengthInBytes << 3;

	if ( copyData )
	{
		if ( lengthInBytes > 0 )
		{
			if (lengthInBytes < BITSTREAM_STACK_ALLOCATION_SIZE)
			{
				data = ( unsigned char* ) stackData;
				numberOfBitsAllocated = BITSTREAM_STACK_ALLOCATION_SIZE << 3;
			}
			else
			{
				data = ( unsigned char* ) malloc( lengthInBytes );
			}
#ifdef _DEBUG
			assert( data );
#endif
			memcpy( data, _data, lengthInBytes );
		}
		else
			data = 0;
	}
	else
		data = ( unsigned char* ) _data;
}

// SAMPSRV (adding this just as a tag for next RakNet upgrade)
BitStream::BitStream( char* _dataC, unsigned int lengthInBytes, bool _copyData )
{
	unsigned char* _data = reinterpret_cast<unsigned char*>(_dataC);

	numberOfBitsUsed = lengthInBytes << 3;
	readOffset = 0;
	copyData = _copyData;
	numberOfBitsAllocated = lengthInBytes << 3;

	if ( copyData )
	{
		if ( lengthInBytes > 0 )
		{
			if (lengthInBytes < BITSTREAM_STACK_ALLOCATION_SIZE)
			{
				data = ( unsigned char* ) stackData;
				numberOfBitsAllocated = BITSTREAM_STACK_ALLOCATION_SIZE << 3;
			}
			else
			{
				data = ( unsigned char* ) malloc( lengthInBytes );
			}
#ifdef _DEBUG
			assert( data );
#endif
			memcpy( data, _data, lengthInBytes );
		}
		else
			data = 0;
	}
	else
		data = ( unsigned char* ) _data;
}

// Use this if you pass a pointer copy to the constructor (_copyData==false) and want to overallocate to prevent reallocation
void BitStream::SetNumberOfBitsAllocated( const unsigned int lengthInBits )
{
#ifdef _DEBUG
	assert( lengthInBits >= ( unsigned int ) numberOfBitsAllocated );
#endif
	numberOfBitsAllocated = lengthInBits;
}

BitStream::~BitStream()
{
	if ( copyData && numberOfBitsAllocated > BITSTREAM_STACK_ALLOCATION_SIZE << 3)
		free( data );  // Use realloc and free so we are more efficient than delete and new for resizing
}




