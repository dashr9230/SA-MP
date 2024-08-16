#ifndef __THREAD_POOL_H
#define __THREAD_POOL_H

#include "DS_Queue.h"
#include "SimpleMutex.h"

/// A simple class to create work threads that processes a queue of functions with data.
/// This class does not allocate or deallocate memory.  It is up to the user to handle memory management.
/// InputType and OutputType are stored directly in a queue.  For large structures, if you plan to delete from the middle of the queue,
/// you might wish to store pointers rather than the structures themselves so the array can shift efficiently.
template <class InputType, class OutputType>
struct ThreadPool
{
	ThreadPool();
	~ThreadPool();

	/// Start the specified number of threads.
	/// \param[in] numThreads The number of threads to start
	/// \param[in] stackSize 0 for default (except on consoles).
	/// \return True on success, false on failure.
	bool StartThreads(int numThreads, int stackSize);

	/// Stops all threads
	void StopThreads(void);

	/// Adds a function to a queue with data to pass to that function.  This function will be called from the thread
	/// Memory management is your responsibility!  This class does not allocate or deallocate memory.
	/// The best way to deallocate \a inputData is in userCallback.  If you call EndThreads such that callbacks were not called, you
	/// can iterate through the inputDataQueue and deallocate all pending input data there
	/// The best way to deallocate output is as it is returned to you from GetOutput.  Similarly, if you end the threads such that
	/// not all output was returned, you can iterate through outputQueue and deallocate it there.
	/// \param[in] workerThreadCallback The function to call from the thread
	/// \param[in] inputData The parameter to pass to \a userCallback
	void AddInput(OutputType (*workerThreadCallback)(InputType, bool *returnOutput), InputType inputData);

	/// Returns true if output from GetOutput is waiting.
	/// \return true if output is waiting, false otherwise
	bool HasOutput(void);

	/// Inaccurate but fast version of HasOutput.  If this returns true, you should still check HasOutput for the real value.
	/// \return true if output is probably waiting, false otherwise
	bool HasOutputFast(void);

	/// Gets the output of a call to \a userCallback
	/// HasOutput must return true before you call this function.  Otherwise it will assert.
	/// \return The output of \a userCallback.  If you have different output signatures, it is up to you to encode the data to indicate this
	OutputType GetOutput(void);

	/// Clears internal buffers
	void Clear(void);

	/// Lock the input buffer before calling the functions InputSize, InputAtIndex, and RemoveInputAtIndex
	/// It is only necessary to lock the input or output while the threads are running
	void LockInput(void);

	/// Unlock the input buffer after you are done with the functions InputSize, GetInputAtIndex, and RemoveInputAtIndex
	void UnlockInput(void);

	/// Length of the input queue
	unsigned InputSize(void);

	/// Get the input at a specified index
	InputType GetInputAtIndex(unsigned index);

	/// Remove input from a specific index.  This does NOT do memory deallocation - it only removes the item from the queue
	void RemoveInputAtIndex(unsigned index);

	/// Lock the output buffer before calling the functions OutputSize, OutputAtIndex, and RemoveOutputAtIndex
	/// It is only necessary to lock the input or output while the threads are running
	void LockOutput(void);
	
	/// Unlock the output buffer after you are done with the functions OutputSize, GetOutputAtIndex, and RemoveOutputAtIndex
	void UnlockOutput(void);

	/// Length of the output queue
	unsigned OutputSize(void);

	/// Get the output at a specified index
	OutputType GetOutputAtIndex(unsigned index);

	/// Remove output from a specific index.  This does NOT do memory deallocation - it only removes the item from the queue
	void RemoveOutputAtIndex(unsigned index);

	/// Removes all items from the input queue
	void ClearInput(void);

	/// Removes all items from the output queue
	void ClearOutput(void);

protected:
	// It is valid to cancel input before it is processed.  To do so, lock the inputQueue with inputQueueMutex,
	// Scan the list, and remove the item you don't want.
	SimpleMutex inputQueueMutex, outputQueueMutex;

	// inputFunctionQueue & inputDataQueue are paired arrays so if you delete from one at a particular index you must delete from the other
	// at the same index
	DataStructures::Queue<OutputType (*)(InputType, bool *returnOutput)> inputFunctionQueue;
	DataStructures::Queue<InputType> inputDataQueue;

	DataStructures::Queue<OutputType> outputQueue;

	template <class InputType, class OutputType>
#ifdef _WIN32
	friend unsigned __stdcall WorkerThread( LPVOID arguments );
#else
	friend void* WorkerThread( void* arguments );
#endif

	/// \internal
	bool threadsRunning;
	/// \internal
	int numThreadsRunning;
	/// \internal
	SimpleMutex numThreadsRunningMutex;
#ifdef _WIN32
	/// \internal
	HANDLE quitAndIncomingDataEvents[2];
#endif
};

#include "ThreadPool.h"
#include "RakSleep.h"
#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
#include <pthread.h>
#endif

template <class InputType, class OutputType>
#ifdef _WIN32
unsigned __stdcall WorkerThread( LPVOID arguments )
#else
void* WorkerThread( void* arguments )
#endif
{
	bool returnOutput;
	ThreadPool<InputType, OutputType> *threadPool = (ThreadPool<InputType, OutputType>*) arguments;
	OutputType (*userCallback)(InputType, bool *returnOutput);
	InputType inputData;
	OutputType callbackOutput;

	userCallback=0;

	// Increase numThreadsRunning
	threadPool->numThreadsRunningMutex.Lock();
	++threadPool->numThreadsRunning;
	threadPool->numThreadsRunningMutex.Unlock();

	while (1)
	{
#ifdef _WIN32
		if (userCallback==0)
		{
			// Wait for signaled event
			WaitForMultipleObjects(
				2,
				threadPool->quitAndIncomingDataEvents,
				false,
				INFINITE);
		}		
#endif

		if (threadPool->threadsRunning==false)
			break;

		// Read input data
		userCallback=0;
		threadPool->inputQueueMutex.Lock();
		if (threadPool->inputFunctionQueue.Size())
		{
			userCallback=threadPool->inputFunctionQueue.Pop();
			inputData=threadPool->inputDataQueue.Pop();
		}
		threadPool->inputQueueMutex.Unlock();

		if (userCallback)
		{
			callbackOutput=userCallback(inputData, &returnOutput);
			if (returnOutput)
			{
				threadPool->outputQueueMutex.Lock();
				threadPool->outputQueue.Push(callbackOutput);
				threadPool->outputQueueMutex.Unlock();
			}			
		}

#ifndef _WIN32
		// If no input data available, and GCC, then sleep.
		if (userCallback==0)
			RakSleep(1000);
#endif
	}

	// Decrease numThreadsRunning
	threadPool->numThreadsRunningMutex.Lock();
	--threadPool->numThreadsRunning;
	threadPool->numThreadsRunningMutex.Unlock();

	return 0;
}
template <class InputType, class OutputType>
ThreadPool<InputType, OutputType>::ThreadPool()
{
	threadsRunning=false;
	numThreadsRunning=0;
}
template <class InputType, class OutputType>
ThreadPool<InputType, OutputType>::~ThreadPool()
{
	StopThreads();
	Clear();
}

template <class InputType, class OutputType>
bool ThreadPool<InputType, OutputType>::StartThreads(int numThreads, int stackSize)
{
	if (threadsRunning==true)
		return false;

#ifdef _WIN32
	quitAndIncomingDataEvents[0]=CreateEvent(0, true, false, 0);
	quitAndIncomingDataEvents[1]=CreateEvent(0, false, false, 0);
#endif

	unsigned threadId = 0;
	int i;
	for (i=0; i < numThreads; i++)
	{
#ifdef _WIN32
		HANDLE threadHandle;
#ifdef _COMPATIBILITY_1
		threadHandle = ( HANDLE ) _beginthreadex( NULL, 0, WorkerThread<InputType, OutputType>, this, 0, &threadId );
#else
		threadHandle = ( HANDLE ) _beginthreadex( NULL, stackSize, WorkerThread<InputType, OutputType>, this, 0, &threadId );
#endif

		if (threadHandle==0)
		{
			StopThreads();
			return false;
		}
		CloseHandle(threadHandle);
#else
		pthread_t threadHandle;
		pthread_attr_t attr;
		pthread_attr_init( &attr );
		pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_DETACHED );

		//  sched_param sp;
		//  sp.sched_priority = sched_get_priority_max(SCHED_OTHER);
		//  pthread_attr_setschedparam(&attr, &sp);

		int error;
		error = pthread_create( &threadHandle, &attr, &WorkerThread, this );

		if ( error )
		{
			StopThreads();
			return false;
		}
#endif
	}

	threadsRunning=true;

	// Wait for number of threads running to increase to numThreads
	bool done=false;
	while (done==false)
	{
		RakSleep(50);
		numThreadsRunningMutex.Lock();
		if (numThreadsRunning==numThreads)
			done=true;
		numThreadsRunningMutex.Unlock();
	}

	return true;
}
template <class InputType, class OutputType>
void ThreadPool<InputType, OutputType>::StopThreads(void)
{
	if (threadsRunning==false)
		return;

	threadsRunning=false;

#ifdef _WIN32
	// Quit event
	SetEvent(quitAndIncomingDataEvents[0]);
#endif

	// Wait for number of threads running to decrease to 0
	bool done=false;
	while (done==false)
	{
		RakSleep(50);
		numThreadsRunningMutex.Lock();
		if (numThreadsRunning==0)
			done=true;
		numThreadsRunningMutex.Unlock();
	}

#ifdef _WIN32
	CloseHandle(quitAndIncomingDataEvents[0]);
	CloseHandle(quitAndIncomingDataEvents[1]);
#endif
}
template <class InputType, class OutputType>
void ThreadPool<InputType, OutputType>::AddInput(OutputType (*workerThreadCallback)(InputType, bool *returnOutput), InputType inputData)
{
	inputQueueMutex.Lock();
	inputDataQueue.Push(inputData);
	inputFunctionQueue.Push(workerThreadCallback);
	inputQueueMutex.Unlock();

#ifdef _WIN32
	// Input data event
	SetEvent(quitAndIncomingDataEvents[1]);
#endif
}
template <class InputType, class OutputType>
bool ThreadPool<InputType, OutputType>::HasOutputFast(void)
{
	return outputQueue.IsEmpty()==false;
}
template <class InputType, class OutputType>
bool ThreadPool<InputType, OutputType>::HasOutput(void)
{
	bool res;
	outputQueueMutex.Lock();
	res=outputQueue.IsEmpty()==false;
	outputQueueMutex.Unlock();
	return res;
}
template <class InputType, class OutputType>
OutputType ThreadPool<InputType, OutputType>::GetOutput(void)
{
	// Real output check
	OutputType output;
	outputQueueMutex.Lock();
	output=outputQueue.Pop();
	outputQueueMutex.Unlock();
	return output;
}
template <class InputType, class OutputType>
void ThreadPool<InputType, OutputType>::Clear(void)
{
	if (threadsRunning)
	{
		inputQueueMutex.Lock();
		inputFunctionQueue.Clear();
		inputDataQueue.Clear();
		inputQueueMutex.Unlock();

		outputQueueMutex.Lock();
		outputQueue.Clear();
		outputQueueMutex.Unlock();
	}
	else
	{
		inputFunctionQueue.Clear();
		inputDataQueue.Clear();
		outputQueue.Clear();
	}
}
template <class InputType, class OutputType>
void ThreadPool<InputType, OutputType>::LockInput(void)
{
	inputQueueMutex.Lock();
}
template <class InputType, class OutputType>
void ThreadPool<InputType, OutputType>::UnlockInput(void)
{
	inputQueueMutex.Unlock();
}
template <class InputType, class OutputType>
unsigned ThreadPool<InputType, OutputType>::InputSize(void)
{
	return inputDataQueue.Size();
}
template <class InputType, class OutputType>
InputType ThreadPool<InputType, OutputType>::GetInputAtIndex(unsigned index)
{
	return inputDataQueue[index];
}
template <class InputType, class OutputType>
void ThreadPool<InputType, OutputType>::RemoveInputAtIndex(unsigned index)
{
	inputDataQueue.Del(index);
	inputFunctionQueue.Del(index);
}
template <class InputType, class OutputType>
void ThreadPool<InputType, OutputType>::LockOutput(void)
{
	outputQueueMutex.Lock();
}
template <class InputType, class OutputType>
void ThreadPool<InputType, OutputType>::UnlockOutput(void)
{
	outputQueueMutex.Unlock();
}
template <class InputType, class OutputType>
unsigned ThreadPool<InputType, OutputType>::OutputSize(void)
{
	return outputQueue.Size();
}
template <class InputType, class OutputType>
OutputType ThreadPool<InputType, OutputType>::GetOutputAtIndex(unsigned index)
{
	return outputQueue[index];
}
template <class InputType, class OutputType>
void ThreadPool<InputType, OutputType>::RemoveOutputAtIndex(unsigned index)
{
	outputQueue.Del(index);
}
template <class InputType, class OutputType>
void ThreadPool<InputType, OutputType>::ClearInput(void)
{
	inputDataQueue.Clear();
	inputFunctionQueue.Clear();
}

template <class InputType, class OutputType>
void ThreadPool<InputType, OutputType>::ClearOutput(void)
{
	outputQueue.Clear();
}


#endif
