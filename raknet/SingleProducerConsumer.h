// TODO: Implement SingleProducerConsumer.h

#ifndef __SINGLE_PRODUCER_CONSUMER_H
#define __SINGLE_PRODUCER_CONSUMER_H

#include <assert.h>

static const int MINIMUM_LIST_SIZE=8;

#include "Export.h"

/// The namespace DataStructures was only added to avoid compiler errors for commonly named data structures
/// As these data structures are stand-alone, you can use them outside of RakNet for your own projects if you wish.
namespace DataStructures
{
	/// \brief A single producer consumer implementation without critical sections.
	template <class SingleProducerConsumerType>
	class RAK_DLL_EXPORT SingleProducerConsumer
	{
	public:
		/// Constructor
		SingleProducerConsumer();

		/// ReadLock must be immediately followed by ReadUnlock. These two functions must be called in the same thread.
		/// \retval 0 No data is availble to read
		/// \retval Non-zero The data previously written to, in another thread, by WriteLock followed by WriteUnlock.
		SingleProducerConsumerType* ReadLock(void);

		/// Signals that we are done reading the the data from the least recent call of ReadLock.
		/// At this point that pointer is no longer valid, and should no longer be read.
		void ReadUnlock(void);

	private:
		struct DataPlusPtr
		{
			SingleProducerConsumerType object;

			// Ready to read is so we can use an equality boolean comparison, in case the writePointer var is trashed while context switching.
			volatile bool readyToRead;
			volatile DataPlusPtr *next;
		};
		volatile DataPlusPtr *readAheadPointer;
		volatile DataPlusPtr *writeAheadPointer;
		volatile DataPlusPtr *readPointer;
		volatile DataPlusPtr *writePointer;
		unsigned readCount, writeCount;
	};

	template <class SingleProducerConsumerType>
		SingleProducerConsumer<SingleProducerConsumerType>::SingleProducerConsumer()
	{
		// Preallocate
		readPointer = new DataPlusPtr;
		writePointer=readPointer;
		readPointer->next = new DataPlusPtr;
		int listSize;
#ifdef _DEBUG
		assert(MINIMUM_LIST_SIZE>=3);
#endif
		for (listSize=2; listSize < MINIMUM_LIST_SIZE; listSize++)
		{
			readPointer=readPointer->next;
			readPointer->next = new DataPlusPtr;
		}
		readPointer->next->next=writePointer; // last to next = start
		readPointer=writePointer;
		readAheadPointer=readPointer;
		writeAheadPointer=writePointer;
		readCount=writeCount=0;
	}

	template <class SingleProducerConsumerType>
		SingleProducerConsumerType* SingleProducerConsumer<SingleProducerConsumerType>::ReadLock( void )
	{
		if (readAheadPointer==writePointer ||
			readAheadPointer->readyToRead==false)
		{
			return 0;
		}

		volatile DataPlusPtr *last;
		last=readAheadPointer;
		readAheadPointer=readAheadPointer->next;
		return (SingleProducerConsumerType*)last;
	}

	template <class SingleProducerConsumerType>
		void SingleProducerConsumer<SingleProducerConsumerType>::ReadUnlock( void )
	{
#ifdef _DEBUG
		assert(readAheadPointer!=readPointer); // If hits, then called ReadUnlock before ReadLock
		assert(readPointer!=writePointer); // If hits, then called ReadUnlock when Read returns 0
#endif
		readCount++;

		// Allow writes to this memory block
		readPointer->readyToRead=false;
		readPointer=readPointer->next;
	}



}

#endif
