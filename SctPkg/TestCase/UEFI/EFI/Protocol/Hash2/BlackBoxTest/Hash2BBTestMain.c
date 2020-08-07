/** @file

  Copyright 2016 - 2017 Unified EFI, Inc.<BR>
  Copyright (c) 2016 - 2017, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at 
  http://opensource.org/licenses/bsd-license.php
 
  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
 
**/
/*++

Module Name:

  Hash2BBTestMain.c

Abstract:

  Test Driver of Hash2 Protocol

--*/

#include "SctLib.h"
#include "Hash2BBTestMain.h"


#define EFI_HASH2_TEST_GUID \
  { 0xda836f8d, 0x217f, 0x4ca0, {0x99, 0xc2, 0x1c, 0xa4, 0xe1, 0x60, 0x77, 0xea }}
//
// Global variables
//
EFI_HANDLE mImageHandle;

EFI_BB_TEST_PROTOCOL *gBBTestProtocolInterface;

EFI_BB_TEST_PROTOCOL_FIELD gBBTestProtocolField = {
  EFI_HASH2_PROTOCOL_TEST_REVISION,
  EFI_HASH2_TEST_GUID,
  L"Hash2 Protocol Test",
  L"UEFI Hash2 Protocol Black-Box Test"
};

EFI_GUID gSupportProtocolGuid[] = {
  EFI_STANDARD_TEST_LIBRARY_GUID,
  EFI_NULL_GUID
};

EFI_BB_TEST_ENTRY_FIELD gBBTestEntryField[] = {
  {
    EFI_HASH2_PROTOCOL_TEST_ENTRY_GUID0101,
    L"GetHashSizeFunction",
    L"Function auto test for Hash2 Protocol GetHashSize().",
    EFI_TEST_LEVEL_MINIMAL,
    gSupportProtocolGuid,
    EFI_TEST_CASE_AUTO,
    BBTestGetHashSizeFunctionTest
  },
  {
    EFI_HASH2_PROTOCOL_TEST_ENTRY_GUID0102,
    L"HashFunction",
    L"Function auto test for Hash2 Protocol Hash().",
    EFI_TEST_LEVEL_MINIMAL,
    gSupportProtocolGuid,
    EFI_TEST_CASE_AUTO,
    BBTestHashFunctionTest
  },
  {
    EFI_HASH2_PROTOCOL_TEST_ENTRY_GUID0103,
    L"HashMultiBlocksFunction",
    L"Function auto test for Hash2 Protocol HashInit()/HashUpdate()/HashFinal().",
    EFI_TEST_LEVEL_MINIMAL,
    gSupportProtocolGuid,
    EFI_TEST_CASE_AUTO,
    BBTestHashMultiBlocksFunctionTest
  },  
  {
    EFI_HASH2_PROTOCOL_TEST_ENTRY_GUID0201,
    L"GetHashSizeConformance",
    L"Conformance auto test for Hash2 Protocol GetHashSize().",
    EFI_TEST_LEVEL_MINIMAL,
    gSupportProtocolGuid,
    EFI_TEST_CASE_AUTO,
    BBTestGetHashSizeConformanceTest
  },
  {
    EFI_HASH2_PROTOCOL_TEST_ENTRY_GUID0202,
    L"HashConformance",
    L"Conformance auto test for Hash2 Protocol Hash().",
    EFI_TEST_LEVEL_MINIMAL,
    gSupportProtocolGuid,
    EFI_TEST_CASE_AUTO,
    BBTestHashConformanceTest
  },
  {
    EFI_HASH2_PROTOCOL_TEST_ENTRY_GUID0203,
    L"HashInitConformance",
    L"Conformance auto test for Hash2 Protocol HashInit().",
    EFI_TEST_LEVEL_MINIMAL,
    gSupportProtocolGuid,
    EFI_TEST_CASE_AUTO,
    BBTestHashInitConformanceTest
  },
  {
    EFI_HASH2_PROTOCOL_TEST_ENTRY_GUID0204,
    L"HashUpdateConformance",
    L"Conformance auto test for Hash2 Protocol HashUpdate().",
    EFI_TEST_LEVEL_MINIMAL,
    gSupportProtocolGuid,
    EFI_TEST_CASE_AUTO,
    BBTestHashUpdateConformanceTest
  },
  {
    EFI_HASH2_PROTOCOL_TEST_ENTRY_GUID0205,
    L"HashFinalConformance",
    L"Conformance auto test for Hash2 Protocol HashFinal().",
    EFI_TEST_LEVEL_MINIMAL,
    gSupportProtocolGuid,
    EFI_TEST_CASE_AUTO,
    BBTestHashFinalConformanceTest
  },
  
  0
};

#define HASH2_SHA1_ALGO_GUID    \
  { 0x2ae9d80f, 0x3fb2, 0x4095, {0xb7, 0xb1, 0xe9, 0x31, 0x57, 0xb9, 0x46, 0xb6 }}

#define HASH2_SHA224_ALGO_GUID    \
  { 0x8df01a06, 0x9bd5, 0x4bf7, {0xb0, 0x21, 0xdb, 0x4f, 0xd9, 0xcc, 0xf4, 0x5b }}

#define HASH2_SHA256_ALGO_GUID    \
  { 0x51aa59de, 0xfdf2, 0x4ea3, {0xbc, 0x63, 0x87, 0x5f, 0xb7, 0x84, 0x2e, 0xe9 }}

#define HASH2_SHA384_ALGO_GUID    \
  { 0xefa96432, 0xde33, 0x4dd2, {0xae, 0xe6, 0x32, 0x8c, 0x33, 0xdf, 0x77, 0x7a }}

#define HASH2_SHA512_ALGO_GUID    \
  { 0xcaa4381e, 0x750c, 0x4770, {0xb8, 0x70, 0x7a, 0x23, 0xb4, 0xe4, 0x21, 0x30 }}

#define HASH2_MD5_ALGO_GUID    \
  { 0xaf7c79c, 0x65b5, 0x4319, {0xb0, 0xae, 0x44, 0xec, 0x48, 0x4e, 0x4a, 0xd7 }}


EFI_GUID gHashAlgorithmGuids[6]={
	HASH2_SHA1_ALGO_GUID,
	HASH2_SHA224_ALGO_GUID,
	HASH2_SHA256_ALGO_GUID,
	HASH2_SHA384_ALGO_GUID,
	HASH2_SHA512_ALGO_GUID,
	HASH2_MD5_ALGO_GUID
};

//
//
//

EFI_STATUS
EFIAPI
InitializeHash2BBTest (
  IN EFI_HANDLE                   ImageHandle,
  IN EFI_SYSTEM_TABLE             *SystemTable
  )
{
  EfiInitializeTestLib (ImageHandle, SystemTable);

  //
  // initialize test utility lib
  //

  SctInitializeLib (ImageHandle, SystemTable);

  mImageHandle = ImageHandle;

  return EfiInitAndInstallBBTestInterface (
           &ImageHandle,
           &gBBTestProtocolField,
           gBBTestEntryField,
           UnloadHash2BBTest,
           &gBBTestProtocolInterface
           );
}


EFI_STATUS
EFIAPI
UnloadHash2BBTest (
  IN EFI_HANDLE                   ImageHandle
  )
{
  return EfiUninstallAndFreeBBTestInterface (
           ImageHandle,
           gBBTestProtocolInterface
           );
}

EFI_STATUS
Hash2ServiceBindingCreateChild (
  IN EFI_HASH2_SERVICE_BINDING_PROTOCOL    *This,
  EFI_HANDLE                               *ChildHandle,
  EFI_HASH2_PROTOCOL                       **Hash
)
{
  EFI_HASH2_SERVICE_BINDING_PROTOCOL *Hash2Sb = NULL;
  EFI_STATUS Status;
		
  //
  // Get the Hash2ServiceBinding Protocol
  //
  if(This == NULL)
  {
    Status = gtBS->LocateProtocol(
                     &gBlackBoxEfiHash2ServiceBindingProtocolGuid,
                     NULL,
                     (VOID **) &Hash2Sb
                     );
	
    if (EFI_ERROR (Status)) 
      return Status;

  } else {
    Hash2Sb = This;
  }

  //
  // Initialize a ChildHandle
  //
  *ChildHandle = NULL;
  
  //
  // Create a ChildHandle with the Hash Protocol
  //
  Status = Hash2Sb->CreateChild(Hash2Sb, ChildHandle);
  if (EFI_ERROR (Status))
    goto ErrorExit;


  //
  // Retrieve the Hash Protocol from ChildHandle
  //
  Status = gBS->HandleProtocol (
                  *ChildHandle,
                  &gBlackBoxEfiHash2ProtocolGuid,
                  (VOID **)Hash
                  );
  if (EFI_ERROR (Status)) 
    goto ErrorExit;
		
  return EFI_SUCCESS;
	
ErrorExit:

  return Status;
}


EFI_STATUS
Hash2ServiceBindingDestoryChild (
	IN EFI_HASH2_SERVICE_BINDING_PROTOCOL    *This,
	EFI_HANDLE                               ChildHandle
)
{
  EFI_HASH2_SERVICE_BINDING_PROTOCOL    *Hash2Sb;
  EFI_STATUS                            Status;
  HASH_PRIVATE_DATA *Private = HASH_PRIVATE_DATA_FROM_SERVICE_BINDING_THIS(This);

  //
  // Get the Hash Service Binding Protocol
  //
  if(This == NULL) {	
    Status = gBS->OpenProtocol (
                    ChildHandle,
                    &gBlackBoxEfiHash2ServiceBindingProtocolGuid,
                    (VOID **)&Hash2Sb,
                    Private->ImageHandle,
                    ChildHandle,
                    EFI_OPEN_PROTOCOL_GET_PROTOCOL
                    );
    if (EFI_ERROR (Status)) 
      return Status;
  } else {
      Hash2Sb = This;
  }

  //
  // Destroy the ChildHandle with the Hash Protocol
  //
  Status = Hash2Sb->DestroyChild (Hash2Sb, ChildHandle);
  return Status;
}
