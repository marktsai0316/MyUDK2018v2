/** @file

  Copyright 2006 - 2017 Unified EFI, Inc.<BR>
  Copyright (c) 2010 - 2017, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at 
  http://opensource.org/licenses/bsd-license.php
 
  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
 
**/
/*++

Module Name:
  Usb2HcTestMain.c

Abstract:
  The main source file for USB Host controller testing.

--*/

//
// Includes
//
#include "SctLib.h"
#include "Usb2HcTest.h"

//
// Declarations
//
EFI_EVENT                   TimerEvent;

EFI_BB_TEST_PROTOCOL_FIELD  gBBTestProtocolField = {
  USB2_HC_TEST_REVISION,
  EFI_USB2_HC_PROTOCOL_GUID,
  L"Usb2.0 Host Controller Protocol Interface Tests",
  L"UEFI Usb2.0 Host Controller Protocol Interface Test"
};

EFI_GUID                    gSupportProtocolGuid1[] = {
  EFI_STANDARD_TEST_LIBRARY_GUID,
  EFI_NULL_GUID
};

EFI_GUID                    gSupportProtocolGuid2[] = {
  EFI_STANDARD_TEST_LIBRARY_GUID,
  EFI_TEST_RECOVERY_LIBRARY_GUID,
  EFI_NULL_GUID
};

EFI_BB_TEST_ENTRY_FIELD     gBBTestEntryField[] = {
  /*
  {
    USB2_HC_GETCAPABILITY_BASIC_MANUAL_GUID,
    L"GetCapability_Func_Manual",
    L"Test the functionality of Usb2Hc->GetCapability.",
    EFI_TEST_LEVEL_DEFAULT,
    gSupportProtocolGuid1,
    EFI_TEST_CASE_MANUAL,
    Usb2HcGetCapabilityBasicManualTest
  },
  {
    USB2_HC_RESET_BASIC_AUTO_GUID,
    L"Reset_Func",
    L"Test the functionality of Usb2Hc->Reset.",
    EFI_TEST_LEVEL_DEFAULT,
    gSupportProtocolGuid1,
    EFI_TEST_CASE_AUTO,
    Usb2HcResetBasicAutoTest
  },
  {
    USB2_HC_GETSTATE_BASIC_AUTO_GUID,
    L"GetState_Func",
    L"Test the functionality of Usb2Hc->GetState.",
    EFI_TEST_LEVEL_DEFAULT,
    gSupportProtocolGuid1,
    EFI_TEST_CASE_AUTO,
    Usb2HcGetStateBasicAutoTest
  },
  {
    USB2_HC_SETSTATE_BASIC_AUTO_GUID,
    L"SetState_Func",
    L"Test the functionality of Usb2Hc->SetState.",
    EFI_TEST_LEVEL_DEFAULT,
    gSupportProtocolGuid1,
    EFI_TEST_CASE_AUTO,
    Usb2HcSetStateBasicAutoTest
  },
  {
    USB2_HC_GETROOTHUBPORTSTATUS_BASIC_AUTO_GUID,
    L"GetRootHubPortStatus_Func",
    L"Test the functionality of Usb2Hc->GetRootHubPortStatus.",
    EFI_TEST_LEVEL_DEFAULT,
    gSupportProtocolGuid1,
    EFI_TEST_CASE_AUTO,
    Usb2HcGetRootHubPortStatusBasicAutoTest
  },
  {
    USB2_HC_GETROOTHUBPORTSTATUS_BASIC_MANUAL_GUID,
    L"GetRootHubPortStatus_Func_Manual",
    L"Test the functionality of Usb2Hc->GetRootHubPortStatus.",
    EFI_TEST_LEVEL_DEFAULT,
    gSupportProtocolGuid1,
    EFI_TEST_CASE_MANUAL,
    Usb2HcGetRootHubPortStatusBasicManualTest
  },
  {
    USB2_HC_SETROOTHUBPORTFEATURE_BASIC_AUTO_GUID,
    L"SetRootHubPortFeature_Func",
    L"Test the functionality of Usb2Hc->SetRootHubPortFeature.",
    EFI_TEST_LEVEL_DEFAULT,
    gSupportProtocolGuid1,
    EFI_TEST_CASE_AUTO,
    Usb2HcSetRootHubPortFeatureBasicAutoTest
  },
  {
    USB2_HC_CLEARROOTHUBPORTFEATURE_BASIC_AUTO_GUID,
    L"ClearRootHubPortFeature_Func",
    L"Test the functionality of Usb2Hc->ClearRootHubPortFeature.",
    EFI_TEST_LEVEL_DEFAULT,
    gSupportProtocolGuid1,
    EFI_TEST_CASE_AUTO,
    Usb2HcClearRootHubPortFeatureBasicAutoTest
  },
  {
    USB2_HC_CLEARROOTHUBPORTFEATURE_BASIC_MANUAL_GUID,
    L"ClearRootHubPortFeature_Func_Manual",
    L"Test the functionality of Usb2Hc->ClearRootHubPortFeature.",
    EFI_TEST_LEVEL_DEFAULT,
    gSupportProtocolGuid1,
    EFI_TEST_CASE_MANUAL,
    Usb2HcClearRootHubPortFeatureBasicManualTest
  },
*/
  {
    USB2_HC_GETCAPABILITY_CONFORMANCE_AUTO_GUID,
    L"GetCapability_Conf",
    L"Perform conformance checks on the Usb2Hc->GetCapability",
    EFI_TEST_LEVEL_MINIMAL,
    gSupportProtocolGuid1,
    EFI_TEST_CASE_AUTO,
    Usb2HcGetCapabilityConformanceAutoTest
  },
  {
    USB2_HC_RESET_CONFORMANCE_AUTO_GUID,
    L"Reset_Conf",
    L"Perform conformance checks on the Usb2Hc->Reset",
    EFI_TEST_LEVEL_MINIMAL,
    gSupportProtocolGuid1,
    EFI_TEST_CASE_AUTO,
    Usb2HcResetConformanceAutoTest
  },
  {
    USB2_HC_GETSTATE_CONFORMANCE_AUTO_GUID,
    L"GetState_Conf",
    L"Perform conformance checks on the Usb2Hc->GetState",
    EFI_TEST_LEVEL_MINIMAL,
    gSupportProtocolGuid1,
    EFI_TEST_CASE_AUTO,
    Usb2HcGetStateConformanceAutoTest
  },
  {
    USB2_HC_SETSTATE_CONFORMANCE_AUTO_GUID,
    L"SetState_Conf",
    L"Perform conformance checks on the Usb2Hc->SetState",
    EFI_TEST_LEVEL_MINIMAL,
    gSupportProtocolGuid1,
    EFI_TEST_CASE_AUTO,
    Usb2HcSetStateConformanceAutoTest
  },
  {
    USB2_HC_CONTROLTRANSFER_CONFORMANCE_AUTO_GUID,
    L"ControlTransfer_Conf_Auto",
    L"Perform Automatic conformance checks on the Usb2Hc->ControlTransfer",
    EFI_TEST_LEVEL_MINIMAL,
    gSupportProtocolGuid1,
    EFI_TEST_CASE_AUTO,
    Usb2HcControlTransferConformanceAutoTest
  },

  /*{
    USB2_HC_CONTROLTRANSFER_CONFORMANCE_MANUAL_GUID,
    L"ControlTransfer_Conf_Manual",
    L"Perform manual conformance checks on the Usb2Hc->ControlTransfer",
    EFI_TEST_LEVEL_MINIMAL,
    gSupportProtocolGuid1,
    EFI_TEST_CASE_MANUAL,
    Usb2HcControlTransferConformanceManualTest
  },*/
  {
    USB2_HC_BULKTRANSFER_CONFORMANCE_AUTO_GUID,
    L"BulkTransfer_Conf",
    L"Perform conformance checks on the Usb2Hc->BulkTransfer",
    EFI_TEST_LEVEL_MINIMAL,
    gSupportProtocolGuid1,
    EFI_TEST_CASE_AUTO,
    Usb2HcBulkTransferConformanceAutoTest
  },
  {
    USB2_HC_ASYNCINTERRUPTTRANSFER_CONFORMANCE_AUTO_GUID,
    L"AsyncInterruptTransfer_Conf",
    L"Perform conformance checks on the Usb2Hc->AsyncInterruptTransfer",
    EFI_TEST_LEVEL_MINIMAL,
    gSupportProtocolGuid1,
    EFI_TEST_CASE_AUTO,
    Usb2HcAsyncInterruptTransferConformanceAutoTest
  },
  {
    USB2_HC_SYNCINTERRUPTTRANSFER_CONFORMANCE_AUTO_GUID,
    L"SyncInterruptTransfer_Conf",
    L"Perform conformance checks on the Usb2Hc->SyncInterruptTransfer",
    EFI_TEST_LEVEL_MINIMAL,
    gSupportProtocolGuid1,
    EFI_TEST_CASE_AUTO,
    Usb2HcSyncInterruptTransferConformanceAutoTest
  },
  {
    USB2_HC_ASYNCISOCHRONOUSTRANSFER_CONFORMANCE_AUTO_GUID,
    L"AsyncIsochronousTransfer_Conf",
    L"Perform conformance checks on the Usb2Hc->AsyncIsochronousTransfer",
    EFI_TEST_LEVEL_MINIMAL,
    gSupportProtocolGuid1,
    EFI_TEST_CASE_AUTO,
    Usb2HcAsyncIsochronousTransferConformanceAutoTest
  },
  {
    USB2_HC_ISOCHRONOUSTRANSFER_CONFORMANCE_AUTO_GUID,
    L"IsochronousTransfer_Conf",
    L"Perform conformance checks on the Usb2Hc->IsochronousTransfer",
    EFI_TEST_LEVEL_MINIMAL,
    gSupportProtocolGuid1,
    EFI_TEST_CASE_AUTO,
    Usb2HcIsochronousTransferConformanceAutoTest
  },
  {
    USB2_HC_GETROOTHUBPORTSTATUS_CONFORMANCE_AUTO_GUID,
    L"GetRootHubPortStatus_Conf",
    L"Perform conformance checks on the Usb2Hc->GetRootHubPortStatus",
    EFI_TEST_LEVEL_MINIMAL,
    gSupportProtocolGuid1,
    EFI_TEST_CASE_AUTO,
    Usb2HcGetRootHubPortStatusConformanceAutoTest
  },
  {
    USB2_HC_SETROOTHUBPORTFEATURE_CONFORMANCE_AUTO_GUID,
    L"SetRootHubPortFeature_Conf",
    L"Perform conformance checks on the Usb2Hc->SetRootHubPortFeature",
    EFI_TEST_LEVEL_MINIMAL,
    gSupportProtocolGuid1,
    EFI_TEST_CASE_AUTO,
    Usb2HcSetRootHubPortFeatureConformanceAutoTest
  },
  {
    USB2_HC_CLEARROOTHUBPORTFEATURE_CONFORMANCE_AUTO_GUID,
    L"ClearRootHubPortFeature_Conf",
    L"Perform conformance checks on the Usb2Hc->ClearRootHubPortFeature",
    EFI_TEST_LEVEL_MINIMAL,
    gSupportProtocolGuid1,
    EFI_TEST_CASE_AUTO,
    Usb2HcClearRootHubPortFeatureConformanceAutoTest
  },
  0
};

EFI_BB_TEST_PROTOCOL        *gBBTestProtocolInterface;

//
// Functions
//

EFI_STATUS
EFIAPI
InitializeBBTestUsb2Hc (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
/*++

Routine Description:

  Creates/installs the BlackBox Interface and the Entry Point list.

Arguments:

  ImageHandle              - The image handle.
  SystemTable              -The system table.

Returns:

 EFI_SUCCESS               - The interface was installed successfully.
 EFI_OUT_OF_RESOURCES      - Failed due to the lack of resources.
 EFI_INVALID_PARAMETER     - One of parameters is invalid.

--*/
{
  EFI_STATUS  status;
  EfiInitializeTestLib (ImageHandle, SystemTable);
  SctInitializeLib (ImageHandle, SystemTable);

  gtBS->CreateEvent (EVT_TIMER, 0, (EFI_EVENT_NOTIFY) NULL, NULL, &TimerEvent);
  status = EfiInitAndInstallBBTestInterface (
             &ImageHandle,
             &gBBTestProtocolField,
             gBBTestEntryField,
             BBTestUsb2HcUnload,
             &gBBTestProtocolInterface
             );

  return status;
}

EFI_STATUS
EFIAPI
BBTestUsb2HcUnload (
  IN EFI_HANDLE       ImageHandle
  )
/*++

Routine Description:

  Unloads the BlackBox Interface and the Entry Point list.

Arguments:

  ImageHandle   - The image handle.

Returns:

  EFI_SUCCESS   -The interface was uninstalled successfully.

--*/
{
  gtBS->CloseEvent (TimerEvent);
  return EfiUninstallAndFreeBBTestInterface (
           ImageHandle,
           gBBTestProtocolInterface
           );
}

VOID
StallForChangeDevice (
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL   *StandardLib,
  IN UINTN                                Time
  )
/*++

Routine Description:

  Stall a while to change device

Arguments:

  StandardLib     - A point to EFI_STANDARD_TEST_LIBRARY_PROTOCOL
  Time            - time to stall

Returns:

  VOID

--*/
{
  UINTN i;
  for (i = 0; i < Time; i++) {
    //
    // SctPrint(L"%d s left \r", Time - i);
    //
    StandardLib->RecordMessage (StandardLib, EFI_VERBOSE_LEVEL_DEFAULT, L"%d s left", Time - i);
    gtBS->Stall (1000000);
  }
}

BOOLEAN
AutoJudge (
  IN  UINTN         Seconds,
  IN  BOOLEAN       Default,
  IN  CHAR16        *Message
  )
/*++

Routine Description:

  Assistant function to auto judge the user's choice, correct or uncorrect.

Arguments:

  Seconds    - Time inteval
  Default    - Default value to judge
  Message    - String to desplay on screen

Returns:

  TRUE       - correct choice respectively
  FALSE      - uncorrect choice respectively

--*/
{
  EFI_STATUS    Status;
  EFI_EVENT     WaitList[2];
  UINTN         NoWait;
  UINTN         WaitIndex;
  EFI_INPUT_KEY Key;
  BOOLEAN       PressYes;

  //
  // Set 1 second periodic timer
  //
  Status = gtBS->SetTimer (TimerEvent, TimerPeriodic, 10000000);

  //
  // Set up a wait list for a key and the timer
  //
  NoWait              = 0;
  WaitList[NoWait++]  = TimerEvent;
  WaitList[NoWait++]  = gtST->ConIn->WaitForKey;

  PressYes            = Default;
  //
  // Wait for either
  //
  SctPrint (Message);
  while (NoWait) {
    if (Default == TRUE) {
      SctPrint (L"\rYes/No?(Auto judge as Yes in %d seconds)", Seconds);
    } else {
      SctPrint (L"\rYes/No?(Auto judge as No in %d seconds)", Seconds);
    }

    Status = gtBS->WaitForEvent (NoWait, WaitList, &WaitIndex);

    switch (WaitIndex) {
    case 0:
      Seconds -= 1;
      if (Seconds <= 0) {
        //
        // To stop loop
        //
        NoWait = 0;
      }
      break;

    case 1:
      //
      // Read the key
      //
      Status = gtST->ConIn->ReadKeyStroke (gtST->ConIn, &Key);
      SctAPrint ((char *) (&Key.UnicodeChar));
      switch (Key.UnicodeChar) {
      case 'Y':
      case 'y':
        NoWait    = 0;
        PressYes  = TRUE;
        break;

      case 'N':
      case 'n':
        NoWait    = 0;
        PressYes  = FALSE;
        break;

      default:
        SctPrint (L"\n");
        break;
      }
      break;

    default:
      break;
      //
      // bad WaitIndex response
      //
    }
  }

  SctPrint (L"\r\n");

  //
  // Done, cancle periodic timer
  //
  Status = gtBS->SetTimer (TimerEvent, TimerCancel, 0);

  return PressYes;
}
