/*++

Copyright (c) 2005, Intel Corporation                                                         
All rights reserved. This program and the accompanying materials                          
are licensed and made available under the terms and conditions of the BSD License         
which accompanies this distribution. The full text of the license may be found at         
http://opensource.org/licenses/bsd-license.php                                            
                                                                                          
THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.             

Module Name:

     EventLogInfo.h
    
Abstract:

    Module to clarify system event log of smbios structure.

Revision History

--*/

#ifndef _SMBIOS_EVENT_LOG_INFO_H
#define _SMBIOS_EVENT_LOG_INFO_H

#define END_OF_LOG  0xFF

#pragma pack(1)

typedef struct {
  UINT8 Type;
  UINT8 Length;
  UINT8 Year;
  UINT8 Month;
  UINT8 Day;
  UINT8 Hour;
  UINT8 Minute;
  UINT8 Second;
  UINT8 LogVariableData[1];
} LOG_RECORD_FORMAT;

typedef struct {
  UINT8 OEMReserved[5];
  UINT8 METW;           // Multiple Event Time Window
  UINT8 MECI;           // Multiple Event Count Increment
  UINT8 CMOSAddress;    // Pre-boot Event Log Reset - CMOS Address
  UINT8 CMOSBitIndex;   // Pre-boot Event Log Reset - CMOS Bit Index
  UINT8 StartingOffset; // CMOS Checksum - Starting Offset
  UINT8 ByteCount;      // CMOS Checksum - Byte Count
  UINT8 ChecksumOffset; // CMOS Checksum - Checksum Offset
  UINT8 Reserved[3];
  UINT8 HeaderRevision;
} LOG_HEADER_TYPE1_FORMAT;

#pragma pack()
//
// System Event Log (Type 15)
//
VOID
DisplaySELAccessMethod (
  UINT8 Key,
  UINT8 Option
  );
VOID
DisplaySELLogStatus (
  UINT8 Key,
  UINT8 Option
  );
VOID
DisplaySysEventLogHeaderFormat (
  UINT8 Key,
  UINT8 Option
  );

VOID
DisplaySysEventLogHeader (
  UINT8 LogHeaderFormat,
  UINT8 *LogHeader
  );
VOID
DisplaySysEventLogData (
  UINT8   *LogData,
  UINT16  LogAreaLength
  );

#endif
