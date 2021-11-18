/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include <linux/types.h>
#include <linux/input.h>
#include <linux/hidraw.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/eventfd.h>
#include <poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>

#include <errno.h>


#define  _GADGETTHREAD_CPP_
#include "GadgetThread.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

GadgetThread::GadgetThread()
{
   // Set base class thread services.
   BaseClass::setThreadName("Kbd");
   BaseClass::setThreadPriorityHigh();

   // Set base class thread priority.
   BaseClass::setThreadPriorityHigh();

   // Initialize variables.
   mGadgetFd = -1;
   mEventFd = -1;
   mErrorCount = 0;
   mReportCount = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immediately
// after the thread starts running. It initializes something.

void GadgetThread::threadInitFunction()
{
   printf("GadgetThread::threadInitFunction\n");

   // Open the event.
   mEventFd = eventfd(0, EFD_SEMAPHORE);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function. This is called by the base class immediately
// after the thread init function. It runs a loop that waits for the
// hid keyboard input.

void GadgetThread::threadRunFunction()
{
restart:
   // Guard.
   if (mTerminateFlag) return;

   // Sleep.
   BaseClass::threadSleep(250);

   // If the hidraw file is open then close it.
   if (mGadgetFd > 0)
   {
      close(mGadgetFd);
      mGadgetFd = -1;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Local variables.

   int tRet = 0;
   char tBuffer[256];

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Open hidraw file.

   mGadgetFd = open(cGadgetDev, O_RDWR, S_IRUSR | S_IWUSR);

   if (mGadgetFd < 0)
   {
      perror("open");
      goto restart;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read report.

   while (!BaseClass::mTerminateFlag)
   {
      printf("Write report******************************************* %d\n", mReportCount++);

      tBuffer[0] = 0;
      tBuffer[1] = 0;
      tBuffer[2] = 0x1d;
      tBuffer[3] = 0;
      tBuffer[4] = 0;
      tBuffer[5] = 0;
      tBuffer[6] = 0;
      tBuffer[7] = 0;

      // Write a report record. 
      tBuffer[2] = 0x1d;
      tRet = write(mGadgetFd, tBuffer, 8);
      if (tRet < 0)
      {
         perror("ERROR write ");
         goto restart;
      }
      // Sleep.
      threadSleep(200);

      // Write a report record. 
      tBuffer[2] = 0x00;
      tRet = write(mGadgetFd, tBuffer, 8);
      if (tRet < 0)
      {
         perror("ERROR write ");
         goto restart;
      }
      // Sleep.
      threadSleep(800);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immediately
// before the thread is terminated. It shuts down the hid api.

void GadgetThread::threadExitFunction()
{
   printf("GadgetThread::threadExitFunction\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread shutdown function. This posts to the close event to
// terminate the thread and it closes the files.

void GadgetThread::shutdownThread()
{
   printf("GadgetThread::shutdownThread\n");

   // Request thread run function return.
   mTerminateFlag = true;

   // Write bytes to the event semaphore to signal a close. This will
   // cause the thread to terminate.
   unsigned long long tCount = 1;
   write(mEventFd, &tCount, 8);

   // Wait for the thread to terminate.
   BaseClass::waitForThreadTerminate();

   // Close the hidraw file if it is open.
   if (mGadgetFd > 0)
   {
      close(mGadgetFd);
      mGadgetFd = -1;
   }

   // Close the event semaphore.
   close(mEventFd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
