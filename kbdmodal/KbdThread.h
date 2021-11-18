#pragma once

/*==============================================================================
Kbd hidraw thread.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risThreadsThreads.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a thread that processes the hidraw inputs from the keyboard.

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
   // after the thread starts running. It initializes something.
   void threadInitFunction() override;

   // Thread run function. This is called by the base class immediately
   // after the thread init function. It runs a loop that waits for the
   // hid keyboard input.
   void threadRunFunction() override;

   // Thread exit function. This is called by the base class immediately
   // before the thread is terminated. It is a placeholder.
   void threadExitFunction() override;

   // Thread shutdown function. This posts to the close event to
   // terminate the thread and it closes the files.
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


