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

   // If false then the mode is normal. If true then the mode is special.
   // The mode is special when the caps lock key is pressed.
   bool mSpecMode;
   bool mLastSpecMode;

   // If true then set the ReportB modifier control, alt, shift bits.
   bool mSpecCtrl;
   bool mSpecAlt;
   bool mSpecShift;

   // Intermediate temporary report.
   char mSpecReport[8];

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

   // Transform an IN report. Copy ReportA to the member report and perform
   // caps lock special logic. 
   void doProcessINForSpecial(const char* aReportA);

   // Transform an IN report key code byte.
   // Apply a function, KeyA->KeyB, that depends on the mode.
   // Return KeyB.
   char doTransformINReportKey(int aKeyIndex);

   // Remove empty zero key codes from member report.
   void doRemoveINReportZeroes();

   // Transform an IN report modifier byte.
   // Apply a function, ModifierA->ModifierB, that depends on the mode.
   // Return ModiferB.
   char doTransformINReportModifier();

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


