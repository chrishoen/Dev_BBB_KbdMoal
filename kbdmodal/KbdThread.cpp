/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"


#define  _KBDTHREAD_CPP_
#include "KbdThread.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

KbdThread::KbdThread()
{
   // Set base class thread services.
   BaseClass::setThreadName("Kbd");
   BaseClass::setThreadPriorityHigh();

   // Set base class thread priority.
   BaseClass::setThreadPriorityHigh();

   // Initialize variables.
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immediately 
// after the thread starts running. It initializes the hid api.

void KbdThread::threadInitFunction()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function. This is called by the base class immediately
// after the thread init function. It runs a loop that waits for the
// hid keyboard input.

void KbdThread::threadRunFunction()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immediately
// before the thread is terminated. It shuts down the hid api.

void KbdThread::threadExitFunction()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************