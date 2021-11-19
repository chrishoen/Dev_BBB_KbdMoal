/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "KbdDefs.h"

#define  _KBDTRANSFORM_CPP_
#include "KbdTransform.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

KbdTransform::KbdTransform()
{
   reset();
}

void KbdTransform::reset()
{
   mSpecMode = false;
   mSpecCtrl = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Transform an IN report. An IN report is an input to the host.
// Apply a function, ReportA->ReportB, that depends on the mode.
// The mode is changed by the caps lock key. 

void KbdTransform::doTransformINReport(const char* aReportA, char* aReportB)
{
   doProcessINForSpecial(aReportA);

   if (!mSpecMode)
   {
      memcpy(aReportB, aReportA, 8);
      return;
   }

   for (int i = 2; i < 8; i++)
   {
      aReportB[i] = doTransformINReportKey(aReportB[i]);
   }

   aReportB[0] = doTransformINReportModifier(mSpecReport[0]);
   aReportB[1] = 0;

}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Transform an IN report. Copy ReportA to the member report and perform
// caps lock special logic. Examine the key codes for caps lock to
// determine the mode. Do not copy a caps lock key code to the member
// report.

void KbdTransform::doProcessINForSpecial(const char* aReportA)
{
   memset(mSpecReport, 0, 8);
   mSpecReport[0] = aReportA[0];

   mLastSpecMode = mSpecMode;
   mSpecMode = false;
   mSpecShift = false;
   int j = 2;
   for (int i = 2; i < 8; i++)
   {
      if (aReportA[i] == cKbdCode_Caps)
      {
         mSpecMode = true;
      }
      else if (aReportA[i] == cKbdCode_Space)
      {
         mSpecShift = true;
      }
      else
      {
         mSpecReport[j++] = aReportA[i];
      }
   }
   mSpecCtrl = false;
   mSpecAlt = false;
   mSpecShift = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Transform an IN report key code byte.
// Apply a function, KeyA->KeyB, that depends on the mode.
// Return KeyB.

char KbdTransform::doTransformINReportKey(char aKeyA)
{
   char tKeyB = 0;

   switch (aKeyA)
   {
   case cKbdCode_W: tKeyB = cKbdCode_X; mSpecCtrl = true; break;
   case cKbdCode_E: tKeyB = cKbdCode_C; mSpecCtrl = true; break;
   case cKbdCode_R: tKeyB = cKbdCode_V; mSpecCtrl = true; break;

   case cKbdCode_Y: tKeyB = cKbdCode_Esc; break;
   case cKbdCode_U: tKeyB = cKbdCode_UpArrow; break;
   case cKbdCode_I: tKeyB = cKbdCode_Delete; break;
   case cKbdCode_O: tKeyB = cKbdCode_Insert; break;
   case cKbdCode_P: tKeyB = cKbdCode_Dash; break;

   case cKbdCode_D: mSpecAlt = true; break;
   case cKbdCode_F: mSpecCtrl = true; break;

   case cKbdCode_H: tKeyB = cKbdCode_Home; break;
   case cKbdCode_J: tKeyB = cKbdCode_LeftArrow; break;
   case cKbdCode_K: tKeyB = cKbdCode_RightArrow; break;
   case cKbdCode_L: tKeyB = cKbdCode_End; break;

   case cKbdCode_N: tKeyB = cKbdCode_Backspace; break;
   case cKbdCode_M: tKeyB = cKbdCode_DownArrow; break;
   case cKbdCode_Comma: tKeyB = cKbdCode_PageUp; break;
   case cKbdCode_Period: tKeyB = cKbdCode_PageDown; break;
   }
   return tKeyB;
}


//******************************************************************************
//******************************************************************************
//******************************************************************************
// Transform an IN report modifier byte.
// Apply a function, ModifierA->ModifierB, that depends on the mode.
// Return ModiferB.

char KbdTransform::doTransformINReportModifier(char aModifierA)
{
   return 0;

}

//******************************************************************************
//******************************************************************************
//******************************************************************************
