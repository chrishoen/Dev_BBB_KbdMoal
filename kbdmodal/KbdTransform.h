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
// This class provides a modal keyboard transformation for key codes
// and modifier bits. It supplies a function that operates on hid keyboard
// reports (ReportA->ReportB).
//
// If the caps lock key is not pressed then the mode is normal and
// an identify transformation is applied, all key codes and modifier
// bits are returned with no changes.
//
// If the caps lock key is pressed then the mode is special and
// a different transformation is applied, some key codes and modifier
// bits are returned with changes and some are ignored.
// 
// An example of this is pressing caps lock and 'u' produces an
// up arrow and caps lock and 'm' produces a down arrow.
// 

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

   // If true then the caps lock is pressed.
   bool mCapsFlag;

   // If true then the left altis pressed.
   bool mLAltFlag;

   // If false then the mode is normal. If true then the mode is special.
   // The mode is special when the caps lock key is pressed.
   // 0 is normal, 1 is caps lock pressed, 2 is left alt presses.
   int mSpecMode;

   // If true then set the ReportB modifier control, alt, shift bits.
   bool mSpecCtrl;
   bool mSpecAlt;
   bool mSpecShift;

   // Previous values of the above flags.
   bool mLastSpecCtrl;
   bool mLastSpecAlt;
   bool mLastSpecShift;

   // If true then some of the special flags changed.
   bool mSpecChangeFlag;

   // If true then the ctrl will be added to the next key.
   bool mSpecCtrlNext;

   // Intermediate temporary report.
   char mSpecReport[16];

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   KbdTransform();
   void reset();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Transform an IN report. An IN report is an input to the host.
   // Apply a function, ReportA->ReportB, that depends on the mode.
   // The mode is changed by the caps lock key. 
   void doTransformINReport(const char* aReportA, char* aReportB);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Examine SpecReport to determine the mode. If all of the bytes are
   // zero then set the mode to normal. If one of the keycodes is capslock
   // then set the mode to special. Reset the modifier flags.
   void doProcessINForSpecial();

   // Transform a SpecReport keycode and set modifier flags.
   void doTransformINReportKey1(int aKeyIndex);
   void doTransformINReportKey2(int aKeyIndex);
   void doTransformINReportKey3(int aKeyIndex);

   // Remove SpecReport zero key codes.
   void doRemoveINReportZeroes();

   // Process SpecReport for special ctrl states.
   void doProcessReportForSpecialCtrl();

   // OR the modifier flags into the SpecReport modifier byte.
   void doTransformINReportModifier();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global instance.

#ifdef _KBDTRANSFORM_CPP_
           KbdTransform gKbdTransform;
#else
   extern  KbdTransform gKbdTransform;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************


