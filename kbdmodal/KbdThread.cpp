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
   mHidrawFd = -1;

}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immediately 
// after the thread starts running. It initializes the hid api.

void KbdThread::threadInitFunction()
{
   printf("KbdThread::threadInitFunction\n");

   // Open the event.
   mEventFd = eventfd(0, EFD_SEMAPHORE);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function. This is called by the base class immediately
// after the thread init function. It runs a loop that waits for the
// hid keyboard input.

void KbdThread::threadRunFunction()
{
restart:
   // Guard.
   if (mTerminateFlag) return;

   // Sleep.
   BaseClass::threadSleep(250);

   // If the hidraw file is open then close it.
   if (mHidrawFd > 0)
   {
      close(mHidrawFd);
      mHidrawFd = -1;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Local variables.

   int tRet = 0;
   int tReportDescSize = 0;
   char tBuffer[256];
   struct hidraw_report_descriptor tReportDesc;
   struct hidraw_devinfo tDevInfo;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Open hidraw file.

   mHidrawFd = open(cHidrawDev, O_RDWR, S_IRUSR | S_IWUSR);

   if (mHidrawFd < 0)
   {
      perror("open");
      goto restart;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read descriptors.

   // Get Report Descriptor Size.
   tRet = ioctl(mHidrawFd, HIDIOCGRDESCSIZE, &tReportDescSize);
   if (tRet < 0)
   {
      perror("HIDIOCGRDESCSIZE");
      goto restart;
   }
   printf("Report Descriptor Size: %d\n", tReportDescSize);

   // Get Report Descriptor.
   tReportDesc.size = tReportDescSize;
   tRet = ioctl(mHidrawFd, HIDIOCGRDESC, &tReportDesc);
   if (tRet < 0)
   {
      perror("HIDIOCGRDESC");
      goto restart;
   }
   printf("Report Descriptor:\n");
   for (int i = 0; i < tReportDesc.size; i++) printf("%hhx ", tReportDesc.value[i]);
   printf("\n");

   // Get Raw Name.
   tRet = ioctl(mHidrawFd, HIDIOCGRAWNAME(256), tBuffer);
   if (tRet < 0)
   {
      perror("HIDIOCGRAWNAME");
      goto restart;
   }
   printf("Raw Name: %s\n", tBuffer);

   // Get Physical Location. 
   tRet = ioctl(mHidrawFd, HIDIOCGRAWPHYS(256), tBuffer);
   if (tRet < 0)
   {
      perror("HIDIOCGRAWPHYS");
      goto restart;
   }
   printf("Raw Phys: %s\n", tBuffer);

   // Get Raw Info.
   tRet = ioctl(mHidrawFd, HIDIOCGRAWINFO, &tDevInfo);
   if (tRet < 0)
   {
      perror("HIDIOCGRAWINFO");
      goto restart;
   }
   printf("Raw Info:\n");
   printf("\tbustype: %d\n", tDevInfo.bustype);
   printf("\tvendor:  0x%04hx\n", tDevInfo.vendor);
   printf("\tproduct: 0x%04hx\n", tDevInfo.product);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read report.

   int tReportCount = 0;
   while (!BaseClass::mTerminateFlag)
   {
      printf("Read report******************************************* %d\n", tReportCount++);

      // Blocking poll for read or close.
      struct pollfd tPollFd[2];
      tPollFd[0].fd = mHidrawFd;
      tPollFd[0].events = POLLIN;
      tPollFd[0].revents = 0;
      tPollFd[1].fd = mEventFd;
      tPollFd[1].events = POLLIN;
      tPollFd[1].revents = 0;

      tRet = poll(&tPollFd[0], 2, -1);
      if (tRet < 0)
      {
         perror("poll");
         goto restart;
      }

      // Test for close.
      if (tPollFd[1].revents & POLLIN)
      {
         printf("read report closed %d\n", tRet);
         goto restart;
      }

      // Read a record. 
      tRet = read(mHidrawFd, tBuffer, 32);
      if (tRet < 0)
      {
         perror("read");
         goto restart;
      }
      printf("read() read %d bytes:\n\t", tRet);
      for (int i = 0; i < tRet; i++) printf("%hhx ", tBuffer[i]);
      puts("\n");
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immediately
// before the thread is terminated. It shuts down the hid api.

void KbdThread::threadExitFunction()
{
   printf("KbdThread::threadExitFunction\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread shutdown function. It shuts down the hid api and terminates
// the thread.
void KbdThread::shutdownThread()
{
   printf("KbdThread::shutdownThread22\n");
   // Request thread run function return.
   mTerminateFlag = true;

   // Write bytes to the event semaphore to signal a close.
   unsigned long long tCount = 1;
   write(mEventFd, &tCount, 8);

   // If the hidraw file is open then close it. This will cause any
   // pending reads to fail and the thread run function will return.
   if (mHidrawFd > 0)
   {
      close(mHidrawFd);
      mHidrawFd = -1;
   }

   // Wait for the thread to terminate.
   BaseClass::waitForThreadTerminate();

   // Close the event semaphore.
   close(mEventFd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
