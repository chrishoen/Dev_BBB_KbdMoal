#pragma once

/*==============================================================================
Exampleple timer thread.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risThreadsThreads.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is an example timer thread. It inherits from the timer thread base
// class. It executes a function periodically.

class KbdThread : public Ris::Threads::BaseThread
{
public:
   typedef Ris::Threads::BaseThread BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants.

   // Device path for hidraw.
   const char* cHidrawDev = "/dev/hidraw0";

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // File descriptor for hidraw.
   int mHidrawFd;

   // File descriptor for event semaphore used for close.
   int mEventFd;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   KbdThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Base class overloads.

   // Thread init function. This is called by the base class immediately 
   // after the thread starts running. It initializes the hid api.
   void threadInitFunction() override;

   // Thread run function. This is called by the base class immediately
   // after the thread init function. It runs a loop that waits for the
   // hid keyboard input.
   void threadRunFunction() override;

   // Thread exit function. This is called by the base class immediately
   // before the thread is terminated. It shuts down the hid api.
   void threadExitFunction() override;

   // Thread shutdown function. It shuts down the hid api and terminates
   // the thread.
   void shutdownThread() override;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global instance.

#ifdef _KBDTHREAD_CPP_
           KbdThread* gKbdThread = 0;
#else
   extern  KbdThread* gKbdThread;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************


