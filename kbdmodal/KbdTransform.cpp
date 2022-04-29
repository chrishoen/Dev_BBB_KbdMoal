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
   mSpecMode = 0;
   mCapsFlag = false;
   mLAltFlag = false;

   mSpecCtrl = false;
   mSpecAlt = false;
   mSpecGui = false;
   mSpecShift = false;
   memset(mSpecReport, 0, 8);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Transform an IN report. An IN report is an input to the host.
// Apply a function, ReportA->ReportB, that depends on the mode.
// The mode is changed by the caps lock key. 

void KbdTransform::doTransformINReport(const char* aReportA, char* aReportB)
{
   // Copy ReportA to SpecReport.
   memcpy(mSpecReport, aReportA, 8);

   // Examine SpecReport to determine the mode.
   doProcessINForSpecial();

   // If special mode then, transform the keycodes, remove zeros, 
   // and set the modifier bits.
   if (mSpecMode)
   {
      // Transform SpecReport keycodes and set modifier flags.
      for (int i = 2; i < 8; i++)
      {
         if (mSpecMode == 1)
         {
            doTransformINReportKey1(i);
         }
         else if (mSpecMode == 2)
         {
            doTransformINReportKey2(i);
         }
         else if (mSpecMode == 3)
         {
            doTransformINReportKey3(i);
         }
      }

      // Remove zero keycodes from SpecReport.
      doRemoveINReportZeroes();

      // Set SpecReport modifier bits.
      doTransformINReportModifier();
   }

   // Copy SpecReport to ReportB.
   memcpy(aReportB, mSpecReport, 8);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Examine SpecReport to determine the mode. If all of the bytes are
// zero then set the mode to normal. If one of the keycodes is capslock
// then set the mode to special. Reset the modifier flags.

void KbdTransform::doProcessINForSpecial()
{
   // Check for all zeroes.
   bool tAllZero = true;
   for (int i = 0; i < 8; i++)
   {
      if (mSpecReport[i]) tAllZero = false;
   }

   // If all zeroes then set the mode to normal.
   if (tAllZero)
   {
      mCapsFlag = false;
      mLAltFlag = false;
   }

   // Check the keycodes for capslock. If there is a capslock then
   // set the mode to special and zero the keycode to ignore it.
   for (int i = 2; i < 8; i++)
   {
      if (mSpecReport[i] == cKbdCode_Caps)
      {
         mCapsFlag = true;
         mSpecReport[i] = 0;
      }
   }

   // Check the first keycode for left alt. If there is a left alt
   // then set the mode to special and zero the keycode to ignore it.
   if (mSpecReport[0] & cKbdMod_LAlt)
   {
      mLAltFlag = true;
      mSpecReport[0] &= ~cKbdMod_LAlt;
   }

   mSpecMode = 0;
   if ( mCapsFlag && !mLAltFlag) mSpecMode = 1;
   if (!mCapsFlag && mLAltFlag)  mSpecMode = 2;
   if ( mCapsFlag && mLAltFlag)  mSpecMode = 3;

   // Reset the modifier flags.
   mSpecCtrl = false;
   mSpecAlt = false;
   mSpecGui = false;
   mSpecShift = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Transform a SpecReport keycode and set modifier flags.

void KbdTransform::doTransformINReportKey1(int aKeyIndex)
{
   char tKeyA = mSpecReport[aKeyIndex];
   char tKeyB = 0;

   // KeyA->KeyB.
   switch (tKeyA)
   {
   case cKbdCode_X: tKeyB = cKbdCode_Caps; break;

   case cKbdCode_F: mSpecCtrl = true; break;
   case cKbdCode_Space: mSpecShift = true; break;

   case cKbdCode_D: tKeyB = cKbdCode_A; mSpecCtrl = true; break;

   case cKbdCode_W: tKeyB = cKbdCode_X; mSpecCtrl = true; break;
   case cKbdCode_E: tKeyB = cKbdCode_C; mSpecCtrl = true; break;
   case cKbdCode_R: tKeyB = cKbdCode_V; mSpecCtrl = true; break;

   case cKbdCode_Y: tKeyB = cKbdCode_Esc; break;
   case cKbdCode_U: tKeyB = cKbdCode_UpArrow; break;
   case cKbdCode_I: tKeyB = cKbdCode_Delete; break;
   case cKbdCode_O: tKeyB = cKbdCode_8; mSpecShift = true; break;

   case cKbdCode_H: tKeyB = cKbdCode_Home; break;
   case cKbdCode_J: tKeyB = cKbdCode_LeftArrow; break;
   case cKbdCode_K: tKeyB = cKbdCode_RightArrow; break;
   case cKbdCode_L: tKeyB = cKbdCode_End; break;
   case cKbdCode_Semi: tKeyB = cKbdCode_Minus; break;
   case cKbdCode_Quote: tKeyB = cKbdCode_Minus; mSpecShift = true; break;

   case cKbdCode_N: tKeyB = cKbdCode_Backspace; break;
   case cKbdCode_M: tKeyB = cKbdCode_DownArrow; break;
   case cKbdCode_Comma: tKeyB = cKbdCode_PageUp; break;
   case cKbdCode_Period: tKeyB = cKbdCode_PageDown; break;

   case cKbdCode_LinSlash: tKeyB = cKbdCode_WinSlash; break;
   }

   // Store KeyB.
   mSpecReport[aKeyIndex] = tKeyB;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Transform a SpecReport keycode and set modifier flags.

void KbdTransform::doTransformINReportKey2(int aKeyIndex)
{
   char tKeyA = mSpecReport[aKeyIndex];
   char tKeyB = 0;

   // KeyA->KeyB.
   switch (tKeyA)
   {
   case cKbdCode_U: tKeyB = cKbdCode_1; break;
   case cKbdCode_I: tKeyB = cKbdCode_2; break;
   case cKbdCode_O: tKeyB = cKbdCode_3; break;
   case cKbdCode_P: tKeyB = cKbdCode_4; break;

   case cKbdCode_J: tKeyB = cKbdCode_5; break;
   case cKbdCode_K: tKeyB = cKbdCode_6; break;
   case cKbdCode_L: tKeyB = cKbdCode_7; break;
   case cKbdCode_Semi: tKeyB = cKbdCode_8; break;

   case cKbdCode_N: tKeyB = cKbdCode_0; break;
   case cKbdCode_M: tKeyB = cKbdCode_9; break;

   case cKbdCode_Y: tKeyB = cKbdCode_1; mSpecShift = true; break;
   case cKbdCode_H: tKeyB = cKbdCode_7; mSpecShift = true; break;
   case cKbdCode_LinSlash: tKeyB = cKbdCode_WinSlash; mSpecShift = true; break;
   }

   // Store KeyB.
   mSpecReport[aKeyIndex] = tKeyB;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Transform a SpecReport keycode and set modifier flags.

void KbdTransform::doTransformINReportKey3(int aKeyIndex)
{
   char tKeyA = mSpecReport[aKeyIndex];
   char tKeyB = 0;

   // KeyA->KeyB.
   switch (tKeyA)
   {
   case cKbdCode_U: tKeyB = cKbdCode_LArray;  mSpecShift = true; break;
   case cKbdCode_I: tKeyB = cKbdCode_RArray;  mSpecShift = true; break;

   case cKbdCode_J: tKeyB = cKbdCode_9; mSpecShift = true; break;
   case cKbdCode_K: tKeyB = cKbdCode_0; mSpecShift = true; break;

   case cKbdCode_M: tKeyB = cKbdCode_LArray; break;
   case cKbdCode_Comma: tKeyB = cKbdCode_RArray; break;

   case cKbdCode_L: tKeyB = cKbdCode_Equal; break;
   case cKbdCode_Semi: tKeyB = cKbdCode_Equal; mSpecShift = true; break;

   case cKbdCode_O: tKeyB = cKbdCode_5;   mSpecShift = true; break;
   case cKbdCode_P: tKeyB = cKbdCode_3;   mSpecShift = true; break;

   case cKbdCode_Period: tKeyB = cKbdCode_2;   mSpecShift = true; break;
   case cKbdCode_LinSlash: tKeyB = cKbdCode_4;   mSpecShift = true; break;
   }

   // Store KeyB.
   mSpecReport[aKeyIndex] = tKeyB;
}


//******************************************************************************
//******************************************************************************
//******************************************************************************
// Remove SpecReport zero key codes.

void KbdTransform::doRemoveINReportZeroes()
{
   // Copy SpecReport to temp report.
   char tReport[16];
   memcpy(tReport, mSpecReport, 8);

   // Set SpecReport keycodes to zero.
   for (int i = 2; i < 8; i++)
   {
      mSpecReport[i] = 0;
   }

   // Copy temp report nonzero keycodes to SpecReport.
   int j = 2;
   for (int i = 2; i < 8; i++)
   {
      if (tReport[i])
      {
         mSpecReport[j++] = tReport[i];
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// OR the modifier flags into the SpecReport modifier byte.

void KbdTransform::doTransformINReportModifier()
{
   char tModifierB = mSpecReport[0];

   if (mSpecAlt) tModifierB |= cKbdMod_LAlt;
   if (mSpecCtrl) tModifierB |= cKbdMod_LCtrl;
   if (mSpecGui) tModifierB |= cKbdMod_LGui;
   if (mSpecShift) tModifierB |= cKbdMod_LShift;

   mSpecReport[0] = tModifierB;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
