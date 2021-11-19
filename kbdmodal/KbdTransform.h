#pragma once

/*==============================================================================
Kbd modal transformation.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This class provides a modal keyboard transformation.

class KbdTransform
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants.

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // File descriptor for gadget.
   int mGadgetFd;

   // File descriptor for event semaphore used for close.
   int mEventFd;

   // Status.
   int mErrorCount;
   int mReportCount;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   KbdTransform();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   void doSomething();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global instance.

#ifdef _KBDTRANSFORM_CPP_
           KbdTransform gKbdTransform;
#else
   extern  KbdTransform* gKbdTransform;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************


