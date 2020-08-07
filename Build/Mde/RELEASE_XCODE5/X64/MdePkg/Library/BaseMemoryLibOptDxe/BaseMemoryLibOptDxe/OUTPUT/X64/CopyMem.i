# 1 "/Users/g5/Dev/SctWorkspace/MdePkg/Library/BaseMemoryLibOptDxe/X64/CopyMem.nasm"
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
# 1 "/Users/g5/Dev/SctWorkspace/MdePkg/Library/BaseMemoryLibOptDxe/X64/CopyMem.nasm" 2
;------------------------------------------------------------------------------
;
; Copyright (c) 2006, Intel Corporation. All rights reserved.<BR>
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
; CopyMem.nasm
;
; Abstract:
;
; CopyMem function
;
; Notes:
;
;------------------------------------------------------------------------------

    DEFAULT REL
    SECTION .text

;------------------------------------------------------------------------------
; void *
;
; InternalMemCopyMem (
; void *Destination,
; void *Source,
; UINTN Count
; );
;------------------------------------------------------------------------------
global _InternalMemCopyMem
_InternalMemCopyMem:
    push rsi
    push rdi
    mov rsi, rdx ; rsi <- Source
    mov rdi, rcx ; rdi <- Destination
    lea r9, [rsi + r8 - 1] ; r9 <- Last byte of Source
    cmp rsi, rdi
    mov rax, rdi ; rax <- Destination as return value
    jae .0 ; Copy forward if Source > Destination
    cmp r9, rdi ; Overlapped?
    jae @CopyBackward ; Copy backward if overlapped
.0:
    xor rcx, rcx
    sub rcx, rdi ; rcx <- -rdi
    and rcx, 15 ; rcx + rsi should be 16 bytes aligned
    jz .1 ; skip if rcx == 0
    cmp rcx, r8
    cmova rcx, r8
    sub r8, rcx
    rep movsb
.1:
    mov rcx, r8
    and r8, 15
    shr rcx, 4 ; rcx <- # of DQwords to copy
    jz @CopyBytes
    movdqa [rsp + 0x18], xmm0 ; save xmm0 on stack
.2:
    movdqu xmm0, [rsi] ; rsi may not be 16-byte aligned
    movntdq [rdi], xmm0 ; rdi should be 16-byte aligned
    add rsi, 16
    add rdi, 16
    loop .2
    mfence
    movdqa xmm0, [rsp + 0x18] ; restore xmm0
    jmp @CopyBytes ; copy remaining bytes
@CopyBackward:
    mov rsi, r9 ; rsi <- Last byte of Source
    lea rdi, [rdi + r8 - 1] ; rdi <- Last byte of Destination
    std
@CopyBytes:
    mov rcx, r8
    rep movsb
    cld
    pop rdi
    pop rsi
    ret
