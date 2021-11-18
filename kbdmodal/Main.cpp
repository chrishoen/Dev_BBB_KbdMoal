#include "stdafx.h"

#include<signal.h>
#include<unistd.h>

#include "risThreadsProcess.h"
#include "risThreadsSynch.h"
#include "MainInit.h"

#include "KbdThread.h"
#include "GadgetThread.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Program signal handler. Posts to a semahore to terminate the program.

Ris::Threads::BinarySemaphore rTerminateSem;

void sig_handler(int signum)
{
   // Post a program termination request.
   printf("sig_handler %d\n", signum);
   rTerminateSem.put();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

int main(int argc,char** argv)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Begin program.
 
   // Register signal handler.
   //signal(SIGINT, sig_handler);
   //signal(SIGTERM, sig_handler);

   // Initialize program resources.
   main_initialize(argc, argv);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Launch program threads.

   if (true)
   {
      gGadgetThread = new GadgetThread;
      gGadgetThread->launchThread();
   }

   if (true)
   {
      gKbdThread = new KbdThread;
      gKbdThread->launchThread();
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Show program threads.

   Ris::Threads::showCurrentThreadInfo();
   if (gGadgetThread) gGadgetThread->showThreadInfo();
   if (gKbdThread) gKbdThread->showThreadInfo();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Wait for a program termination request.

   printf("wait for main terminate sem\n");
   rTerminateSem.get();
   printf("got main terminate sem\n");

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Shutdown program threads.

   if (gKbdThread) gKbdThread->shutdownThread();
   if (gGadgetThread) gGadgetThread->shutdownThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Delete program threads.

   if (gKbdThread)
   {
      delete gKbdThread;
      gKbdThread = 0;
   }

   if (gGadgetThread)
   {
      delete gGadgetThread;
      gGadgetThread = 0;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // End program.

   main_finalize();
   return 0;
}
