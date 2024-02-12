// TODO: Implement BitStream.h

#ifndef __BITSTREAM_H
#define __BITSTREAM_H

#include "Export.h"
#include "NetworkTypes.h"
#include <assert.h>

/// Arbitrary size, just picking something likely to be larger than most packets
#define BITSTREAM_STACK_ALLOCATION_SIZE 256

/// The namespace RakNet is not consistently used.  It's only purpose is to avoid compiler errors for classes whose names are very common.
/// For the most part I've tried to avoid this simply by using names very likely to be unique for my classes.
namespace RakNet
{
	/// This class allows you to write and read native types as a string of bits.  BitStream is used extensively throughout RakNet and is designed to be used by users as well.	
	/// \sa BitStreamSample.txt
	class RAK_DLL_EXPORT BitStream
	{
	public:
		/// Default Constructor 
		BitStream();

		/// Create the bitstream, with some number of bytes to immediately allocate.
		/// There is no benefit to calling this, unless you know exactly how many bytes you need and it is greater than BITSTREAM_STACK_ALLOCATION_SIZE.
		/// In that case all it does is save you one or more realloc calls.
		/// \param[in] initialBytesToAllocate the number of bytes to pre-allocate. 
		BitStream( int initialBytesToAllocate );

		/// Initialize the BitStream, immediately setting the data it contains to a predefined pointer.
		/// Set \a _copyData to true if you want to make an internal copy of the data you are passing. Set it to false to just save a pointer to the data.
		/// You shouldn't call Write functions with \a _copyData as false, as this will write to unallocated memory
		/// 99% of the time you will use this function to cast Packet::data to a bitstream for reading, in which case you should write something as follows:
		/// \code
		/// RakNet::BitStream bs(packet->data, packet->length, false);
		/// \endcode
		/// \param[in] _data An array of bytes.
		/// \param[in] lengthInBytes Size of the \a _data.
		/// \param[in] _copyData true or false to make a copy of \a _data or not.
		BitStream( unsigned char* _data, unsigned int lengthInBytes, bool _copyData );

// SAMPSRV (adding this just as a tag for next RakNet upgrade)
		BitStream( char* _dataC, unsigned int lengthInBytes, bool _copyData );
// SAMPSRV end

		/// Destructor 
		~BitStream();

		/// Resets the bitstream for reuse.
		void Reset( void );

		/// Use this if you pass a pointer copy to the constructor
		/// *(_copyData==false) and want to overallocate to prevent
		/// *reallocation
		void SetNumberOfBitsAllocated( const unsigned int lengthInBits );


	private:

		int numberOfBitsUsed;

		int numberOfBitsAllocated;

		int readOffset;

		unsigned char *data;

		/// true if the internal buffer is copy of the data passed to the constructor
		bool copyData;

		/// BitStreams that use less than BITSTREAM_STACK_ALLOCATION_SIZE use the stack, rather than the heap to store data.  It switches over if BITSTREAM_STACK_ALLOCATION_SIZE is exceeded
		unsigned char stackData[BITSTREAM_STACK_ALLOCATION_SIZE];
	};
}

#endif
