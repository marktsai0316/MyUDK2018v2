# 1 "/Users/g5/Dev/SctWorkspace/MdePkg/Library/BaseIoLibIntrinsic/X64/IoFifoSev.nasm"
# 1 "<built-in>" 1
# 361 "<built-in>"
# 1 "/Users/g5/Dev/SctWorkspace/Build/Mde/RELEASE_XCODE5/X64/MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsicSev/DEBUG/AutoGen.h" 1
# 16 "/Users/g5/Dev/SctWorkspace/Build/Mde/RELEASE_XCODE5/X64/MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsicSev/DEBUG/AutoGen.h"
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
# 17 "/Users/g5/Dev/SctWorkspace/Build/Mde/RELEASE_XCODE5/X64/MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsicSev/DEBUG/AutoGen.h" 2

extern GUID gEfiCallerIdGuid;
extern CHAR8 *gEfiCallerBaseName;



extern UINT64 _gPcd_SkuId_Array[];
# 362 "<built-in>" 2
# 1 "/Users/g5/Dev/SctWorkspace/MdePkg/Library/BaseIoLibIntrinsic/X64/IoFifoSev.nasm" 2
;------------------------------------------------------------------------------
;
; Copyright (c) 2006 - 2012, Intel Corporation. All rights reserved.<BR>
; Copyright (c) 2017, AMD Incorporated. All rights reserved.<BR>
;
; This program and the accompanying materials are licensed and made available
; under the terms and conditions of the BSD License which accompanies this
; distribution. The full text of the license may be found at
; http:
;
; THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
; WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
;
;------------------------------------------------------------------------------

    DEFAULT REL
    SECTION .text

;------------------------------------------------------------------------------
; Check whether we need to unroll the String I/O in SEV guest
;
; Return
;------------------------------------------------------------------------------
global _SevNoRepIo
_SevNoRepIo:

  ; CPUID clobbers ebx, ecx and edx
  push rbx
  push rcx
  push rdx

  ; Check if we are runing under hypervisor
  ; CPUID(1).ECX Bit 31
  mov eax, 1
  cpuid
  bt ecx, 31
  jnc @UseRepIo

  ; Check if we have Memory encryption CPUID leaf
  mov eax, 0x80000000
  cpuid
  cmp eax, 0x8000001f
  jl @UseRepIo

  ; Check for memory encryption feature:
  ; CPUID Fn8000_001F[EAX] - Bit 1
  ;
  mov eax, 0x8000001f
  cpuid
  bt eax, 1
  jnc @UseRepIo

  ; Check if memory encryption is enabled
  ; MSR_0xC0010131 - Bit 0 (SEV enabled)
  ; MSR_0xC0010131 - Bit 1 (SEV-ES enabled)
  mov ecx, 0xc0010131
  rdmsr

  ; Check for (SevEsEnabled == 0 && SevEnabled == 1)
  and eax, 3
  cmp eax, 1
  je @SevNoRepIo_Done

@UseRepIo:
  xor eax, eax

@SevNoRepIo_Done:
  pop rdx
  pop rcx
  pop rbx
  ret

;------------------------------------------------------------------------------
; void
;
; IoReadFifo8 (
; UINTN Port,
; UINTN Size,
; void *Buffer
; );
;------------------------------------------------------------------------------
global _IoReadFifo8
_IoReadFifo8:
    xchg rcx, rdx
    xchg rdi, r8 ; rdi: buffer address; r8: save rdi

    ; Check if we need to unroll String I/O
    call _SevNoRepIo
    test eax, eax
    jnz @IoReadFifo8_NoRep

    cld
    rep insb
    jmp @IoReadFifo8_Done

@IoReadFifo8_NoRep:
    jrcxz @IoReadFifo8_Done

@IoReadFifo8_Loop:
    in al, dx
    mov byte [rdi], al
    inc rdi
    loop @IoReadFifo8_Loop

@IoReadFifo8_Done:
    mov rdi, r8 ; restore rdi
    ret

;------------------------------------------------------------------------------
; void
;
; IoReadFifo16 (
; UINTN Port,
; UINTN Size,
; void *Buffer
; );
;------------------------------------------------------------------------------
global _IoReadFifo16
_IoReadFifo16:
    xchg rcx, rdx
    xchg rdi, r8 ; rdi: buffer address; r8: save rdi

    ; Check if we need to unroll String I/O
    call _SevNoRepIo
    test eax, eax
    jnz @IoReadFifo16_NoRep

    cld
    rep insw
    jmp @IoReadFifo16_Done

@IoReadFifo16_NoRep:
    jrcxz @IoReadFifo16_Done

@IoReadFifo16_Loop:
    in ax, dx
    mov word [rdi], ax
    add rdi, 2
    loop @IoReadFifo16_Loop

@IoReadFifo16_Done:
    mov rdi, r8 ; restore rdi
    ret

;------------------------------------------------------------------------------
; void
;
; IoReadFifo32 (
; UINTN Port,
; UINTN Size,
; void *Buffer
; );
;------------------------------------------------------------------------------
global _IoReadFifo32
_IoReadFifo32:
    xchg rcx, rdx
    xchg rdi, r8 ; rdi: buffer address; r8: save rdi

    ; Check if we need to unroll String I/O
    call _SevNoRepIo
    test eax, eax
    jnz @IoReadFifo32_NoRep

    cld
    rep insd
    jmp @IoReadFifo32_Done

@IoReadFifo32_NoRep:
    jrcxz @IoReadFifo32_Done

@IoReadFifo32_Loop:
    in eax, dx
    mov dword [rdi], eax
    add rdi, 4
    loop @IoReadFifo32_Loop

@IoReadFifo32_Done:
    mov rdi, r8 ; restore rdi
    ret

;------------------------------------------------------------------------------
; void
;
; IoWriteFifo8 (
; UINTN Port,
; UINTN Size,
; void *Buffer
; );
;------------------------------------------------------------------------------
global _IoWriteFifo8
_IoWriteFifo8:
    xchg rcx, rdx
    xchg rsi, r8 ; rsi: buffer address; r8: save rsi

    ; Check if we need to unroll String I/O
    call _SevNoRepIo
    test eax, eax
    jnz @IoWriteFifo8_NoRep

    cld
    rep outsb
    jmp @IoWriteFifo8_Done

@IoWriteFifo8_NoRep:
    jrcxz @IoWriteFifo8_Done

@IoWriteFifo8_Loop:
    mov al, byte [rsi]
    out dx, al
    inc rsi
    loop @IoWriteFifo8_Loop

@IoWriteFifo8_Done:
    mov rsi, r8 ; restore rsi
    ret

;------------------------------------------------------------------------------
; void
;
; IoWriteFifo16 (
; UINTN Port,
; UINTN Size,
; void *Buffer
; );
;------------------------------------------------------------------------------
global _IoWriteFifo16
_IoWriteFifo16:
    xchg rcx, rdx
    xchg rsi, r8 ; rsi: buffer address; r8: save rsi

    ; Check if we need to unroll String I/O
    call _SevNoRepIo
    test eax, eax
    jnz @IoWriteFifo16_NoRep

    cld
    rep outsw
    jmp @IoWriteFifo16_Done

@IoWriteFifo16_NoRep:
    jrcxz @IoWriteFifo16_Done

@IoWriteFifo16_Loop:
    mov ax, word [rsi]
    out dx, ax
    add rsi, 2
    loop @IoWriteFifo16_Loop

@IoWriteFifo16_Done:
    mov rsi, r8 ; restore rsi
    ret

;------------------------------------------------------------------------------
; void
;
; IoWriteFifo32 (
; UINTN Port,
; UINTN Size,
; void *Buffer
; );
;------------------------------------------------------------------------------
global _IoWriteFifo32
_IoWriteFifo32:
    xchg rcx, rdx
    xchg rsi, r8 ; rsi: buffer address; r8: save rsi

    ; Check if we need to unroll String I/O
    call _SevNoRepIo
    test eax, eax
    jnz @IoWriteFifo32_NoRep

    cld
    rep outsd
    jmp @IoWriteFifo32_Done

@IoWriteFifo32_NoRep:
    jrcxz @IoWriteFifo32_Done

@IoWriteFifo32_Loop:
    mov eax, dword [rsi]
    out dx, eax
    add rsi, 4
    loop @IoWriteFifo32_Loop

@IoWriteFifo32_Done:
    mov rsi, r8 ; restore rsi
    ret
