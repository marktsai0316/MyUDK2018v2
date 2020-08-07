# 1 "/Users/g5/Dev/SctWorkspace/MdePkg/Library/BaseMemoryLibOptDxe/X64/ScanMem8.nasm"
# 1 "<built-in>" 1
# 361 "<built-in>"
# 1 "/Users/g5/Dev/SctWorkspace/Build/Mde/RELEASE_XCODE5/X64/MdePkg/Library/BaseMemoryLibOptDxe/BaseMemoryLibOptDxe/DEBUG/AutoGen.h" 1
# 16 "/Users/g5/Dev/SctWorkspace/Build/Mde/RELEASE_XCODE5/X64/MdePkg/Library/BaseMemoryLibOptDxe/BaseMemoryLibOptDxe/DEBUG/AutoGen.h"
# 1 "/Users/g5/Dev/SctWorkspace/MdePkg/Include/Base.h" 1
# 28 "/Users/g5/Dev/SctWorkspace/MdePkg/Include/Base.h"
# 1 "/Users/g5/Dev/SctWorkspace/MdePkg/Include/X64/ProcessorBind.h" 1
# 41 "/Users/g5/Dev/SctWorkspace/MdePkg/Include/X64/ProcessorBind.h"
#pragma GCC visibility push (hidden)
# 188 "/Users/g5/Dev/SctWorkspace/MdePkg/Include/X64/ProcessorBind.h"
 typedef unsigned long long UINT64;



  typedef long long INT64;



  typedef unsigned int UINT32;



  typedef int INT32;



  typedef unsigned short UINT16;




  typedef unsigned short CHAR16;



  typedef short INT16;




  typedef unsigned char BOOLEAN;



  typedef unsigned char UINT8;



  typedef char CHAR8;



  typedef signed char INT8;






typedef UINT64 UINTN;




typedef INT64 INTN;
# 29 "/Users/g5/Dev/SctWorkspace/MdePkg/Include/Base.h" 2
# 54 "/Users/g5/Dev/SctWorkspace/MdePkg/Include/Base.h"
extern UINT8 _VerifySizeofBOOLEAN[(sizeof(BOOLEAN) == (1)) / (sizeof(BOOLEAN) == (1))];
extern UINT8 _VerifySizeofINT8[(sizeof(INT8) == (1)) / (sizeof(INT8) == (1))];
extern UINT8 _VerifySizeofUINT8[(sizeof(UINT8) == (1)) / (sizeof(UINT8) == (1))];
extern UINT8 _VerifySizeofINT16[(sizeof(INT16) == (2)) / (sizeof(INT16) == (2))];
extern UINT8 _VerifySizeofUINT16[(sizeof(UINT16) == (2)) / (sizeof(UINT16) == (2))];
extern UINT8 _VerifySizeofINT32[(sizeof(INT32) == (4)) / (sizeof(INT32) == (4))];
extern UINT8 _VerifySizeofUINT32[(sizeof(UINT32) == (4)) / (sizeof(UINT32) == (4))];
extern UINT8 _VerifySizeofINT64[(sizeof(INT64) == (8)) / (sizeof(INT64) == (8))];
extern UINT8 _VerifySizeofUINT64[(sizeof(UINT64) == (8)) / (sizeof(UINT64) == (8))];
extern UINT8 _VerifySizeofCHAR8[(sizeof(CHAR8) == (1)) / (sizeof(CHAR8) == (1))];
extern UINT8 _VerifySizeofCHAR16[(sizeof(CHAR16) == (2)) / (sizeof(CHAR16) == (2))];
# 73 "/Users/g5/Dev/SctWorkspace/MdePkg/Include/Base.h"
typedef enum {
  __VerifyUint8EnumValue = 0xff
} __VERIFY_UINT8_ENUM_SIZE;

typedef enum {
  __VerifyUint16EnumValue = 0xffff
} __VERIFY_UINT16_ENUM_SIZE;

typedef enum {
  __VerifyUint32EnumValue = 0xffffffff
} __VERIFY_UINT32_ENUM_SIZE;

extern UINT8 _VerifySizeof__VERIFY_UINT8_ENUM_SIZE[(sizeof(__VERIFY_UINT8_ENUM_SIZE) == (4)) / (sizeof(__VERIFY_UINT8_ENUM_SIZE) == (4))];
extern UINT8 _VerifySizeof__VERIFY_UINT16_ENUM_SIZE[(sizeof(__VERIFY_UINT16_ENUM_SIZE) == (4)) / (sizeof(__VERIFY_UINT16_ENUM_SIZE) == (4))];
extern UINT8 _VerifySizeof__VERIFY_UINT32_ENUM_SIZE[(sizeof(__VERIFY_UINT32_ENUM_SIZE) == (4)) / (sizeof(__VERIFY_UINT32_ENUM_SIZE) == (4))];
# 261 "/Users/g5/Dev/SctWorkspace/MdePkg/Include/Base.h"
typedef struct {
  UINT32 Data1;
  UINT16 Data2;
  UINT16 Data3;
  UINT8 Data4[8];
} GUID;




typedef struct {
  UINT8 Addr[4];
} IPv4_ADDRESS;




typedef struct {
  UINT8 Addr[16];
} IPv6_ADDRESS;




typedef UINT64 PHYSICAL_ADDRESS;




typedef struct _LIST_ENTRY LIST_ENTRY;




struct _LIST_ENTRY {
  LIST_ENTRY *ForwardLink;
  LIST_ENTRY *BackLink;
};
# 657 "/Users/g5/Dev/SctWorkspace/MdePkg/Include/Base.h"
typedef __builtin_ms_va_list VA_LIST;
# 761 "/Users/g5/Dev/SctWorkspace/MdePkg/Include/Base.h"
typedef UINTN *BASE_LIST;
# 925 "/Users/g5/Dev/SctWorkspace/MdePkg/Include/Base.h"
typedef UINTN RETURN_STATUS;
# 1241 "/Users/g5/Dev/SctWorkspace/MdePkg/Include/Base.h"
  void * __builtin_return_address (unsigned int level);
# 17 "/Users/g5/Dev/SctWorkspace/Build/Mde/RELEASE_XCODE5/X64/MdePkg/Library/BaseMemoryLibOptDxe/BaseMemoryLibOptDxe/DEBUG/AutoGen.h" 2

extern GUID gEfiCallerIdGuid;
extern CHAR8 *gEfiCallerBaseName;



extern UINT64 _gPcd_SkuId_Array[];
# 362 "<built-in>" 2
# 1 "/Users/g5/Dev/SctWorkspace/MdePkg/Library/BaseMemoryLibOptDxe/X64/ScanMem8.nasm" 2
;------------------------------------------------------------------------------
;
; Copyright (c) 2006 - 2008, Intel Corporation. All rights reserved.<BR>
; This program and the accompanying materials
; are licensed and made available under the terms and conditions of the BSD License
; which accompanies this distribution. The full text of the license may be found at
; http:
;
; THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
; WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
;
; Module Name:
;
; ScanMem8.Asm
;
; Abstract:
;
; ScanMem8 function
;
; Notes:
;
; The following BaseMemoryLib instances contain the same copy of this file:
;
; BaseMemoryLibRepStr
; BaseMemoryLibMmx
; BaseMemoryLibSse2
; BaseMemoryLibOptDxe
; BaseMemoryLibOptPei
;
;------------------------------------------------------------------------------

    DEFAULT REL
    SECTION .text

;------------------------------------------------------------------------------
; const void *
;
; InternalMemScanMem8 (
; const void *Buffer,
; UINTN Length,
; UINT8 Value
; );
;------------------------------------------------------------------------------
global _InternalMemScanMem8
_InternalMemScanMem8:
    push rdi
    mov rdi, rcx
    mov rcx, rdx
    mov rax, r8
    repne scasb
    lea rax, [rdi - 1]
    cmovnz rax, rcx ; set rax to 0 if not found
    pop rdi
    ret
