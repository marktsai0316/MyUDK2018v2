/** @file

  Copyright 2006 - 2015 Unified EFI, Inc.<BR>
  Copyright (c) 2013 - 2014, ARM Ltd. All rights reserved.<BR>
  Copyright (c) 2014 - 2015, Intel Corporation. All rights reserved.<BR>   

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at 
  http://opensource.org/licenses/bsd-license.php
 
  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
 
**/

#include "SctLibInternal.h"

#include <Protocol/EfiShell.h>
#include <Protocol/EfiShellInterface.h>
#include <Protocol/EfiShellEnvironment2.h>
#include <Protocol/EfiShellParameters.h>

typedef struct  {
  EFI_SHELL_GET_FILE_INFO                   GetFileInfo;
  EFI_SHELL_SET_FILE_INFO                   SetFileInfo;
  EFI_SHELL_READ_FILE                       ReadFile;
  EFI_SHELL_WRITE_FILE                      WriteFile;
  EFI_SHELL_CLOSE_FILE                      CloseFile;
  EFI_SHELL_DELETE_FILE                     DeleteFile;
  EFI_SHELL_GET_FILE_POSITION               GetFilePosition;
  EFI_SHELL_SET_FILE_POSITION               SetFilePosition;
  EFI_SHELL_FLUSH_FILE                      FlushFile;
  EFI_SHELL_GET_FILE_SIZE                   GetFileSize;
} FILE_HANDLE_FUNCTION_MAP;

// If we're running in the UEFI shell environment, ShellInitialize (in
// UefiShellLib.c) will populate gEfiShellProtocol. Otherwise, it will populate
// mEfiShellInterface.
#define USING_UEFI_SHELL (gEfiShellProtocol != NULL)

EFI_SHELL_ENVIRONMENT2        *mEfiShellEnvironment2;
EFI_SHELL_INTERFACE           *mEfiShellInterface;
EFI_SHELL_PROTOCOL            *gEfiShellProtocol;
EFI_SHELL_PARAMETERS_PROTOCOL *gEfiShellParametersProtocol;
EFI_HANDLE                    mEfiShellEnvironment2Handle;
FILE_HANDLE_FUNCTION_MAP      FileFunctionMap;

// Shadowed Arguments to workaround difference Argv[0] on the two shells
// Only populated when using the old shell
CHAR16 **mArgv = NULL;

EFI_GUID gEfiShellProtocolGuid           = {0x6302d008, 0x7f9b, 0x4f30, {0x87, 0xac, 0x60, 0xc9, 0xfe, 0xf5, 0xda, 0x4e}};
EFI_GUID gEfiShellParametersProtocolGuid = {0x752f3136, 0x4e16, 0x4fdc, {0xa2, 0x2a, 0xe5, 0xf4, 0x68, 0x12, 0xf4, 0xca}};
EFI_GUID gEfiShellEnvironment2Guid       = {0x47c7b221, 0xc42a, 0x11d2, {0x8e, 0x57, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};
EFI_GUID gEfiShellInterfaceGuid          = {0x47c7b223, 0xc42a, 0x11d2, {0x8e, 0x57, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};
EFI_GUID gEfiShellEnvironment2ExtGuid    = {0xd2c18636, 0x40e5, 0x4eb5, {0xa3, 0x1b, 0x36, 0x69, 0x5f, 0xd4, 0x2c, 0x87}};

STATIC
/**
  Destructor for the library.  free any resources.

  @param[in] ImageHandle  A copy of the ImageHandle.
  @param[in] SystemTable  A pointer to the SystemTable for the application.

  @retval EFI_SUCCESS   The operation was successful.
  @return               An error from the CloseProtocol function.
**/
EFI_STATUS
EFIAPI
ShellLibDestructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  if (mEfiShellEnvironment2 != NULL) {
    tBS->CloseProtocol(mEfiShellEnvironment2Handle==NULL?ImageHandle:mEfiShellEnvironment2Handle,
                       &gEfiShellEnvironment2Guid,
                       ImageHandle,
                       NULL);
    mEfiShellEnvironment2 = NULL;
  }
  if (mEfiShellInterface != NULL) {
    tBS->CloseProtocol(ImageHandle,
                       &gEfiShellInterfaceGuid,
                       ImageHandle,
                       NULL);
    mEfiShellInterface = NULL;
  }
  if (gEfiShellProtocol != NULL) {
    tBS->CloseProtocol(ImageHandle,
                       &gEfiShellProtocolGuid,
                       ImageHandle,
                       NULL);
    gEfiShellProtocol = NULL;
  }
  if (gEfiShellParametersProtocol != NULL) {
    tBS->CloseProtocol(ImageHandle,
                       &gEfiShellParametersProtocolGuid,
                       ImageHandle,
                       NULL);
    gEfiShellParametersProtocol = NULL;
  }
  mEfiShellEnvironment2Handle = NULL;

  if (mArgv != NULL) {
    SctFreePool (mArgv);
    SctFreePool (mArgv[0]);
  }
  mArgv = NULL;

  return (EFI_SUCCESS);
}

STATIC
/**
  Helper function to find ShellEnvironment2 for constructor.

  @param[in] ImageHandle    A copy of the calling image's handle.

  @retval EFI_OUT_OF_RESOURCES    Memory allocation failed.
**/
EFI_STATUS
EFIAPI
ShellFindSE2 (
  IN EFI_HANDLE        ImageHandle
  )
{
  EFI_STATUS  Status;
  EFI_HANDLE  *Buffer;
  UINTN       BufferSize;
  UINTN       HandleIndex;

  BufferSize = 0;
  Buffer = NULL;
  Status = tBS->OpenProtocol(ImageHandle,
                             &gEfiShellEnvironment2Guid,
                             (VOID **)&mEfiShellEnvironment2,
                             ImageHandle,
                             NULL,
                             EFI_OPEN_PROTOCOL_GET_PROTOCOL
                            );
  //
  // look for the mEfiShellEnvironment2 protocol at a higher level
  //
  if (EFI_ERROR (Status) || !(SctCompareGuid (&mEfiShellEnvironment2->SESGuid, &gEfiShellEnvironment2ExtGuid))){
    //
    // figure out how big of a buffer we need.
    //
    Status = tBS->LocateHandle (ByProtocol,
                                &gEfiShellEnvironment2Guid,
                                NULL, // ignored for ByProtocol
                                &BufferSize,
                                Buffer
                               );
    //
    // maybe it's not there???
    //
    if (Status == EFI_BUFFER_TOO_SMALL) {
      Buffer = (EFI_HANDLE*)SctAllocateZeroPool (BufferSize);
      if (Buffer == NULL) {
        return (EFI_OUT_OF_RESOURCES);
      }
      Status = tBS->LocateHandle (ByProtocol,
                                  &gEfiShellEnvironment2Guid,
                                  NULL, // ignored for ByProtocol
                                  &BufferSize,
                                  Buffer
                                 );
    }
    if (!EFI_ERROR (Status) && Buffer != NULL) {
      //
      // now parse the list of returned handles
      //
      Status = EFI_NOT_FOUND;
      for (HandleIndex = 0; HandleIndex < (BufferSize/sizeof(Buffer[0])); HandleIndex++) {
        Status = tBS->OpenProtocol(Buffer[HandleIndex],
                                   &gEfiShellEnvironment2Guid,
                                   (VOID **)&mEfiShellEnvironment2,
                                   ImageHandle,
                                   NULL,
                                   EFI_OPEN_PROTOCOL_GET_PROTOCOL
                                  );
         if (SctCompareGuid (&mEfiShellEnvironment2->SESGuid, &gEfiShellEnvironment2ExtGuid) == 0) {
          mEfiShellEnvironment2Handle = Buffer[HandleIndex];
          Status = EFI_SUCCESS;
          break;
        }
      }
    }
  }
  if (Buffer != NULL) {
    SctFreePool (Buffer);
  }
  return (Status);
}

STATIC
/**
  Function to do most of the work of the constructor.  Allows for calling
  multiple times without complete re-initialization.

  @param[in] ImageHandle  A copy of the ImageHandle.
  @param[in] SystemTable  A pointer to the SystemTable for the application.

  @retval EFI_SUCCESS   The operationw as successful.
**/
EFI_STATUS
EFIAPI
ShellLibConstructorWorker (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;
  CHAR16     *Cwd;
  CHAR16     *BinaryPath;

  gEfiShellProtocol = NULL;
  gEfiShellParametersProtocol = NULL;
  mEfiShellInterface = NULL;
  mEfiShellEnvironment2 = NULL;

  //
  // UEFI 2.0 shell interfaces (used preferentially)
  //
  Status = tBS->OpenProtocol(
    ImageHandle,
    &gEfiShellProtocolGuid,
    (VOID **)&gEfiShellProtocol,
    ImageHandle,
    NULL,
    EFI_OPEN_PROTOCOL_GET_PROTOCOL
   );
  if (EFI_ERROR(Status)) {
    //
    // Search for the shell protocol
    //
    Status = tBS->LocateProtocol(
      &gEfiShellProtocolGuid,
      NULL,
      (VOID **)&gEfiShellProtocol
     );
    if (EFI_ERROR(Status)) {
      gEfiShellProtocol = NULL;
    }
  }
  Status = tBS->OpenProtocol(
    ImageHandle,
    &gEfiShellParametersProtocolGuid,
    (VOID **)&gEfiShellParametersProtocol,
    ImageHandle,
    NULL,
    EFI_OPEN_PROTOCOL_GET_PROTOCOL
   );
  if (EFI_ERROR(Status)) {
    gEfiShellParametersProtocol = NULL;
  }

  if (gEfiShellParametersProtocol == NULL || gEfiShellProtocol == NULL) {
    //
    // Moved to separate function due to complexity
    //
    Status = ShellFindSE2(ImageHandle);

    if (EFI_ERROR(Status)) {
      DEBUG((EFI_D_ERROR, "Status: 0x%08x\r\n", Status));
      mEfiShellEnvironment2 = NULL;
    }
    Status = tBS->OpenProtocol(ImageHandle,
                               &gEfiShellInterfaceGuid,
                               (VOID **)&mEfiShellInterface,
                               ImageHandle,
                               NULL,
                               EFI_OPEN_PROTOCOL_GET_PROTOCOL
                              );
    if (EFI_ERROR(Status)) {
      mEfiShellInterface = NULL;
    }

    // EDK Shell's Argv[0] is the first word of the command line.
    // UEFI Shell's Argv[0] is the full path of the executable.
    // Convert former to latter.
    Cwd = mEfiShellEnvironment2->CurDir (NULL);
    ASSERT (Cwd != NULL);

    BinaryPath = SctPoolPrint (L"%s\\%s", Cwd, mEfiShellInterface->Argv[0]);
    if (BinaryPath == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    mArgv = SctAllocateCopyPool (
              mEfiShellInterface->Argc * sizeof (CHAR16 *),
              mEfiShellInterface->Argv
              );
    if (mArgv == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    mArgv[0] = BinaryPath;
  }

  //
  // only success getting 2 of either the old or new, but no 1/2 and 1/2
  //
  if ((mEfiShellEnvironment2 != NULL && mEfiShellInterface          != NULL) ||
      (gEfiShellProtocol     != NULL && gEfiShellParametersProtocol != NULL)   ) {
    return (EFI_SUCCESS);
  }
  return (EFI_NOT_FOUND);
}

EFI_STATUS
SctShellApplicationInit (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  SctInitializeLib (ImageHandle, SystemTable);

  //
  // deinit the current stuff
  //
  ASSERT_EFI_ERROR(ShellLibDestructor(ImageHandle, tST));

  //
  // init the new stuff
  //
  return (ShellLibConstructorWorker(ImageHandle, tST));
}

EFI_STATUS
SctShellGetArguments (
  OUT UINTN           *Argc,
  OUT CHAR16          ***Argv
  )
{
  if (USING_UEFI_SHELL) {
    if (Argv != NULL) {
      *Argv = gEfiShellParametersProtocol->Argv;
    }
    if (Argc != NULL) {
      *Argc = gEfiShellParametersProtocol->Argc;
    }
  } else {
    if (mArgv != NULL) {
      *Argv = mArgv;
    }
    if (Argc != NULL) {
      *Argc = mEfiShellInterface->Argc;
    }
  }
  return EFI_SUCCESS;
}

EFI_STATUS
SctShellMapToDevicePath (
  IN CHAR16                     *Name,
  OUT EFI_DEVICE_PATH_PROTOCOL **DevicePath
  )
{
  EFI_STATUS                Status;
  UINTN                     NameLength;
  CHAR16                   *InternalName = NULL;

  if (gEfiShellProtocol != NULL) {
    *DevicePath = (EFI_DEVICE_PATH_PROTOCOL *) gEfiShellProtocol->GetDevicePathFromMap (Name);
    if (*DevicePath != NULL) {
      return EFI_SUCCESS;
    } else {
      return EFI_NOT_FOUND;
    }
  } else {
    // The new shell's GetDevicePathFromMap allows map names with or without the
    // ':' at the end. The old shell's GetFsDevicePath does not allow it.
    // Create a version without it.
    NameLength = SctStrLen (Name);
    if (Name[NameLength - 1] == L':') {
      InternalName = SctAllocateCopyPool (NameLength * sizeof (CHAR16), Name);
      InternalName[NameLength - 1] = L'\0';
      *DevicePath = (EFI_DEVICE_PATH_PROTOCOL *) mEfiShellEnvironment2->GetMap (InternalName);
      if (*DevicePath == NULL) {
        Status = EFI_NOT_FOUND;
      }else {
        Status = EFI_SUCCESS;
      }
      SctFreePool (InternalName);
      return Status;
    } else {
      *DevicePath = (EFI_DEVICE_PATH_PROTOCOL *) mEfiShellEnvironment2->GetMap (Name);
      if (*DevicePath == NULL) {
        return EFI_NOT_FOUND;
      }else {
        return EFI_SUCCESS;
      }
    }
  }
}

BOOLEAN
SctShellGetExecutionBreak (
  IN VOID
  )
{
  //
  // Check for UEFI Shell 2.0 protocols
  //
  if (gEfiShellProtocol != NULL) {

    //
    // We are using UEFI Shell 2.0; see if the event has been triggered
    //
    if (tBS->CheckEvent(gEfiShellProtocol->ExecutionBreak) != EFI_SUCCESS) {
      return (FALSE);
    }
    return (TRUE);
  }

  //
  // using EFI Shell; call the function to check
  //
  if (mEfiShellEnvironment2 != NULL) {
    return (mEfiShellEnvironment2->GetExecutionBreak());
  }

  return (FALSE);
}

VOID
SctShellFreeArgInfo (
  IN EFI_SHELL_ARG_INFO       *ArgInfo
  )
{
  //
  // in case there gonna be changes of EFI_SHELL_ARG_INFO
  //
  return ;
}

EFI_STATUS
SctShellFilterNullArgs (
  VOID
  )
{
  CHAR16              **Argv;
  EFI_SHELL_ARG_INFO  *ArgInfo = NULL;
  UINTN               Argc;
  UINTN               Index;
  UINTN               Index2;
  EFI_STATUS          Status;

  Status  = EFI_SUCCESS;

  if (USING_UEFI_SHELL) {
    Argc = gEfiShellParametersProtocol->Argc;
    
    Index2  = 0;
    for (Index = 0; Index < gEfiShellParametersProtocol->Argc; Index++) {
      if (!gEfiShellParametersProtocol->Argv[Index][0]) {
        Argc--;
      }
    }

    Argv    = SctAllocateZeroPool (Argc * sizeof (CHAR16 *));
    if (NULL == Argv) {
      Status = EFI_OUT_OF_RESOURCES;
      goto Done;
    }

    for (Index = 0; Index < gEfiShellParametersProtocol->Argc; Index++) {
      if (!gEfiShellParametersProtocol->Argv[Index][0]) {
        continue;
      }

      Argv[Index2] = SctStrDuplicate (gEfiShellParametersProtocol->Argv[Index]);
      if (NULL == Argv[Index2]) {
        Status = EFI_OUT_OF_RESOURCES;
        goto Done;
      }

      Index2++;
    }

    for (Index = 0; Index < gEfiShellParametersProtocol->Argc; Index++) {
      SctFreePool (gEfiShellParametersProtocol->Argv[Index]);
    }

    SctFreePool (gEfiShellParametersProtocol->Argv);
    gEfiShellParametersProtocol->Argv = Argv;
    gEfiShellParametersProtocol->Argc = Argc;
    Argv        = NULL;
  } else {
    Argc = mEfiShellInterface->Argc;

    Index2  = 0;
    for (Index = 0; Index < mEfiShellInterface->Argc; Index++) {
      if (!mArgv[Index][0]) {
        Argc--;
      }
    }
    
    Argv    = SctAllocateZeroPool (Argc * sizeof (CHAR16 *));
    ArgInfo = SctAllocateZeroPool (Argc * sizeof (EFI_SHELL_ARG_INFO));
    if (NULL == Argv || NULL == ArgInfo) {
      Status = EFI_OUT_OF_RESOURCES;
      goto Done;
    }

    for (Index = 0; Index < mEfiShellInterface->Argc; Index++) {
      if (!mArgv[Index][0]) {
        continue;
      }

      Argv[Index2] = SctStrDuplicate (mArgv[Index]);
      if (NULL == Argv[Index2]) {
        Status = EFI_OUT_OF_RESOURCES;
        goto Done;
      }

      ArgInfo[Index2].Attributes = mEfiShellInterface->ArgInfo[Index].Attributes;
      Index2++;
    }

    for (Index = 0; Index < mEfiShellInterface->Argc; Index++) {
      SctShellFreeArgInfo (&mEfiShellInterface->ArgInfo[Index]);

      //SctFreePool (mArgv[Index]);
    }

    SctFreePool (mEfiShellInterface->ArgInfo);
    SctFreePool (mArgv);
    mEfiShellInterface->ArgInfo = ArgInfo;
    mArgv    = Argv;
    mEfiShellInterface->Argc    = Argc;
    ArgInfo     = NULL;
    Argv        = NULL;
  }

Done:
  if (ArgInfo) {
    for (Index = 0; Index < Index2; Index++) {
      SctShellFreeArgInfo (&ArgInfo[Index]);
    }

    SctFreePool (ArgInfo);
  }

  if (Argv) {
    for (Index = 0; Index < Index2; Index++) {
      SctFreePool (&Argv[Index]);
    }
    
    SctFreePool (Argv);
  }

  return Status;
}

EFI_DEVICE_PATH_PROTOCOL *
SctShellGetDevicePathFromFilePath (
  IN CONST CHAR16   *Path
  )
{
  if (gEfiShellProtocol != NULL) {
    return gEfiShellProtocol->GetDevicePathFromFilePath (Path);
  } else {
    return mEfiShellEnvironment2->NameToPath(Path);
  }
}


/**
  Constructor for the Shell library.

  Initialize the library and determine if the underlying is a UEFI Shell 2.0 or an EFI shell.

  @param ImageHandle    the image handle of the process
  @param SystemTable    the EFI System Table pointer

  @retval EFI_SUCCESS   the initialization was complete sucessfully
  @return others        an error ocurred during initialization
**/
EFI_STATUS
EFIAPI
ShellLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  mEfiShellEnvironment2       = NULL;
  gEfiShellProtocol           = NULL;
  gEfiShellParametersProtocol = NULL;
  mEfiShellInterface          = NULL;
  mEfiShellEnvironment2Handle = NULL;

  return (ShellLibConstructorWorker(ImageHandle, SystemTable));
}

/**
  Cause the shell to parse and execute a command line.

  This function creates a nested instance of the shell and executes the specified
  command (CommandLine) with the specified environment (Environment). Upon return,
  the status code returned by the specified command is placed in StatusCode.
  If Environment is NULL, then the current environment is used and all changes made
  by the commands executed will be reflected in the current environment. If the
  Environment is non-NULL, then the changes made will be discarded.
  The CommandLine is executed from the current working directory on the current
  device.

  The EnvironmentVariables pararemeter is ignored in a pre-UEFI Shell 2.0
  environment.  The values pointed to by the parameters will be unchanged by the
  ShellExecute() function.

  @param[in] ParentHandle         The parent image starting the operation.
  @param[in] CommandLine          The pointer to a NULL terminated command line.
  @param[in] Output               True to display debug output.  False to hide it.
  @param[in] EnvironmentVariables Optional pointer to array of environment variables
                                  in the form "x=y".  If NULL, the current set is used.
  @param[out] Status              The status of the run command line.

  @retval EFI_SUCCESS             The operation completed sucessfully.  Status
                                  contains the status code returned.
  @retval EFI_INVALID_PARAMETER   A parameter contains an invalid value.
  @retval EFI_OUT_OF_RESOURCES    Out of resources.
  @retval EFI_UNSUPPORTED         The operation is not allowed.
**/
EFI_STATUS
SctShellExecute (
  IN EFI_HANDLE                 *ParentHandle,
  IN CHAR16                     *CommandLine OPTIONAL,
  IN BOOLEAN                    Output OPTIONAL,
  IN CHAR16                     **EnvironmentVariables OPTIONAL,
  OUT EFI_STATUS                *Status OPTIONAL
  )
{
  EFI_STATUS                CmdStatus;
  //
  // Check for UEFI Shell 2.0 protocols
  //
  if (gEfiShellProtocol != NULL) {
    //
    // Call UEFI Shell 2.0 version (not using Output parameter)
    //
    return (gEfiShellProtocol->Execute(ParentHandle,
                                      CommandLine,
                                      EnvironmentVariables,
                                      Status));
  }

  //
  // Check for EFI shell
  //
  if (mEfiShellEnvironment2 != NULL) {
    //
    // Call EFI Shell version (not using EnvironmentVariables or Status parameters)
    // Due to oddity in the EFI shell we want to dereference the ParentHandle here
    //
    CmdStatus = (mEfiShellEnvironment2->Execute(*ParentHandle,
                                          CommandLine,
                                          Output));
    //
    // No Status output parameter so just use the returned status
    //
    if (Status != NULL) {
      *Status = CmdStatus;
    }
    //
    // If there was an error, we can't tell if it was from the command or from
    // the Execute() function, so we'll just assume the shell ran successfully
    // and the error came from the command.
    //
    return EFI_SUCCESS;
  }

  return (EFI_UNSUPPORTED);
}

///
/// version of EFI_SHELL_FILE_INFO struct, except has no CONST pointers.
/// This allows for the struct to be populated.
///
typedef struct {
  SCT_LIST_ENTRY Link;
  EFI_STATUS Status;
  CHAR16 *FullName;
  CHAR16 *FileName;
  SHELL_FILE_HANDLE          Handle;
  EFI_FILE_INFO *Info;
} EFI_SHELL_FILE_INFO_NO_CONST;

/**
  Converts a EFI shell list of structures to the corresponding UEFI Shell 2.0 type of list.

  if OldStyleFileList is NULL then ASSERT()

  this function will convert a SHELL_FILE_INFO based list into a callee allocated
  EFI_SHELL_FILE_INFO based list.  it is up to the caller to free the memory via
  the SctShellCloseFileMetaArg function.

  @param[in] FileList           the EFI shell list type
  @param[in, out] ListHead      the list to add to

  @retval the resultant head of the double linked new format list;
**/
SCT_LIST_ENTRY*
EFIAPI
InternalShellConvertFileListType (
  IN SCT_LIST_ENTRY                 *FileList,
  IN OUT SCT_LIST_ENTRY             *ListHead
  )
{
  SHELL_FILE_ARG                *OldInfo;
  SCT_LIST_ENTRY                *Link;
  EFI_SHELL_FILE_INFO_NO_CONST  *NewInfo;

  //
  // ASSERTs
  //
  ASSERT(FileList  != NULL);
  ASSERT(ListHead  != NULL);

  //
  // enumerate through each member of the old list and copy
  //
  for (Link = FileList->ForwardLink; Link != FileList; Link = Link->ForwardLink) {
    OldInfo = CR (Link, SHELL_FILE_ARG, Link, SHELL_FILE_ARG_SIGNATURE);
    ASSERT(OldInfo           != NULL);

    //
    // Skip ones that failed to open...
    //
    if (OldInfo->Status != EFI_SUCCESS) {
      continue;
    }

    //
    // make sure the old list was valid
    //
    ASSERT(OldInfo->Info     != NULL);
    ASSERT(OldInfo->FullName != NULL);
    ASSERT(OldInfo->FileName != NULL);

    //
    // allocate a new EFI_SHELL_FILE_INFO object
    //
    NewInfo               = SctAllocateZeroPool (sizeof(EFI_SHELL_FILE_INFO));
    if (NewInfo == NULL) {
      SctShellCloseFileMetaArg((&ListHead));
      ListHead = NULL;
      break;
    }

    //
    // copy the simple items
    //
    NewInfo->Handle       = OldInfo->Handle;
    NewInfo->Status       = OldInfo->Status;

    // old shell checks for 0 not NULL
    OldInfo->Handle = 0;

    //
    // allocate new space to copy strings and structure
    //
    NewInfo->FullName     = SctAllocateZeroPool (SctStrSize (OldInfo->FullName));
    NewInfo->FileName     = SctAllocateZeroPool (SctStrSize (OldInfo->FileName));
    NewInfo->Info         = SctAllocateZeroPool ((UINTN)OldInfo->Info->Size);

    //
    // make sure all the memory allocations were sucessful
    //
    if (NULL == NewInfo->FullName || NewInfo->FileName == NULL || NewInfo->Info == NULL) {
      SctShellCloseFileMetaArg ((&ListHead));
      ListHead = NULL;
      break;
    }

    //
    // Copt the strings and structure
    //
    SctStrCpy (NewInfo->FullName, OldInfo->FullName);
    SctStrCpy (NewInfo->FileName, OldInfo->FileName);
    tBS->CopyMem (NewInfo->Info, OldInfo->Info, (UINTN)OldInfo->Info->Size);

    //
    // add that to the list
    //
    SctInsertTailList (ListHead, &NewInfo->Link);
  }
  return (ListHead);
}

/**
  Opens a group of files based on a path.

  This function uses the Arg to open all the matching files. Each matched
  file has a SHELL_FILE_ARG structure to record the file information. These
  structures are placed on the list ListHead. Users can get the SHELL_FILE_INFO
  structures from ListHead to access each file. This function supports wildcards
  and will process '?' and '*' as such.  the list must be freed with a call to
  SctShellCloseFileMetaArg().

  If you are NOT appending to an existing list *ListHead must be NULL.  If
  *ListHead is NULL then it must be callee freed.

  @param Arg                    pointer to path string
  @param OpenMode               mode to open files with
  @param ListHead               head of linked list of results

  @retval EFI_SUCCESS           the operation was sucessful and the list head
                                contains the list of opened files
  @return != EFI_SUCCESS        the operation failed

  @sa InternalShellConvertFileListType
**/
EFI_STATUS
SctShellOpenFileMetaArg (
  IN CHAR16                     *Arg,
  IN UINT64                     OpenMode,
  IN OUT SCT_LIST_ENTRY         **ListHead
  )
{
  EFI_STATUS                    Status;
  SCT_LIST_ENTRY                OldStyleFileList;
  BOOLEAN                       WeAllocated = FALSE;

  //
  // ASSERT that Arg and ListHead are not NULL
  //
  ASSERT(Arg      != NULL);
  ASSERT(ListHead != NULL);

  //
  // Check for UEFI Shell 2.0 protocols
  //
  if (gEfiShellProtocol != NULL) {
    if (*ListHead == NULL) {
      *ListHead = SctAllocateZeroPool(sizeof(EFI_SHELL_FILE_INFO));
      if (*ListHead == NULL) {
        return (EFI_OUT_OF_RESOURCES);
      }
      WeAllocated = TRUE;
      SctInitializeListHead(*ListHead);
    }
    Status = gEfiShellProtocol->OpenFileList(
                                  Arg,
                                  OpenMode,
                                  (EFI_SHELL_FILE_INFO **)ListHead
                                  );
    if (EFI_ERROR(Status)) {
      gEfiShellProtocol->RemoveDupInFileList((EFI_SHELL_FILE_INFO **)ListHead);
    } else {
      Status = gEfiShellProtocol->RemoveDupInFileList(
                                    (EFI_SHELL_FILE_INFO **)ListHead
                                    );
    }
    if (*ListHead != NULL && SctIsListEmpty(*ListHead)) {
      if (WeAllocated) {
      SctFreePool(*ListHead);
      *ListHead = NULL;
      }
      return (EFI_NOT_FOUND);
    }
    return (Status);
  }

  //
  // Check for EFI shell
  //
  if (mEfiShellEnvironment2 != NULL) {
    //
    // make sure the list head is initialized
    //
    SctInitializeListHead (&OldStyleFileList);

    //
    // Get the EFI Shell list of files
    //
    Status = mEfiShellEnvironment2->FileMetaArg (Arg, &OldStyleFileList);
    if (EFI_ERROR(Status)) {
      *ListHead = NULL;
      return (Status);
    }

    if (*ListHead == NULL) {
      *ListHead = (SCT_LIST_ENTRY *)SctAllocateZeroPool (sizeof(EFI_SHELL_FILE_INFO));
      if (*ListHead == NULL) {
        return (EFI_OUT_OF_RESOURCES);
      }
      WeAllocated = TRUE;
      SctInitializeListHead (*ListHead);
    }

    //
    // Convert that to equivalent of UEFI Shell 2.0 structure
    //
    InternalShellConvertFileListType (&OldStyleFileList, *ListHead);

    //
    // Free the EFI Shell version that was converted.
    //
    mEfiShellEnvironment2->FreeFileList (&OldStyleFileList);

    if (SctIsListEmpty (*ListHead)) {
      if (WeAllocated) {
        SctFreePool(*ListHead);
        *ListHead = NULL;
      }
      Status = EFI_NOT_FOUND;
    }
    return (Status);
  }

  return (EFI_UNSUPPORTED);
}
/**
  Free the linked list returned from SctShellOpenFileMetaArg.

  if ListHead is NULL then ASSERT().

  @param ListHead               the pointer to free.

  @retval EFI_SUCCESS           the operation was sucessful.
**/
EFI_STATUS
EFIAPI
SctShellCloseFileMetaArg (
  IN OUT SCT_LIST_ENTRY    **ListHead
  )
{
  SCT_LIST_ENTRY                    *Node;

  //
  // ASSERT that ListHead is not NULL
  //
  ASSERT(ListHead != NULL);

  //
  // Check for UEFI Shell 2.0 protocols
  //
  if (gEfiShellProtocol != NULL) {
    return (gEfiShellProtocol->FreeFileList((EFI_SHELL_FILE_INFO **)ListHead));
  } else if (mEfiShellEnvironment2 != NULL) {
    //
    // Since this is EFI Shell version we need to free our internally made copy
    // of the list
    //
    for ( Node = SctGetFirstNode(*ListHead)
        ; *ListHead != NULL && !SctIsListEmpty(*ListHead)
        ; Node = SctGetFirstNode(*ListHead)) {
      SctRemoveEntryList(Node);
      if (((EFI_SHELL_FILE_INFO_NO_CONST*)Node)->Handle != NULL) {
        ((EFI_FILE_PROTOCOL*)((EFI_SHELL_FILE_INFO_NO_CONST*)Node)->Handle)->Close (((EFI_SHELL_FILE_INFO_NO_CONST*)Node)->Handle);
      }
      SctFreePool(((EFI_SHELL_FILE_INFO_NO_CONST*)Node)->FullName);
      SctFreePool(((EFI_SHELL_FILE_INFO_NO_CONST*)Node)->FileName);
      if (((EFI_SHELL_FILE_INFO_NO_CONST*)Node)->Info != NULL) {
        SctFreePool(((EFI_SHELL_FILE_INFO_NO_CONST*)Node)->Info);
      }
      SctFreePool((EFI_SHELL_FILE_INFO_NO_CONST*)Node);
    }
    return EFI_SUCCESS;
  }

  return (EFI_UNSUPPORTED);
}

/**
  Retreives the current directory path

  If the DeviceName is NULL, it returns the current device's current directory
  name. If the DeviceName is not NULL, it returns the current directory name
  on specified drive.

  @param DeviceName             the name of the drive to get directory on

  @retval NULL                  the directory does not exist
  @return != NULL               the directory
**/
CONST CHAR16*
EFIAPI
ShellGetCurrentDir (
  IN CHAR16                     * CONST DeviceName OPTIONAL
  )
{
  //
  // Check for UEFI Shell 2.0 protocols
  //
  if (gEfiShellProtocol != NULL) {
    return (gEfiShellProtocol->GetCurDir(DeviceName));
  }

  //
  // Check for EFI shell
  //
  if (mEfiShellEnvironment2 != NULL) {
    return (mEfiShellEnvironment2->CurDir(DeviceName));
  }

  return (NULL);
}

EFI_STATUS
SctExpandRelativePath (
  IN CHAR16                       *RelativePath,
  OUT CHAR16                      **FileName
  )
/*++

Routine Description:

  Expand a relative file path to a full file path.

--*/
{
  CONST CHAR16              *Cwd;

  //
  // Find the full file path of the file
  //

  Cwd = ShellGetCurrentDir (NULL);
  ASSERT (Cwd != NULL);
  if (Cwd == NULL) {
    return EFI_NOT_FOUND;
  }

  // Append RelativePath to Cwd

  *FileName = SctPoolPrint (L"%s\\%s", Cwd, RelativePath);

  return EFI_SUCCESS;
}

EFI_STATUS
SctGetFilesystemDevicePath (
  IN CHAR16                      *FilePath,
  OUT EFI_DEVICE_PATH_PROTOCOL  **DevicePath,
  OUT CHAR16                    **RemainingFilePath OPTIONAL
  )
/*++

Routine Description:

  Take a file path like "fs0:/foo/bar". Return the device path of the
  filesystem represented by "fs0:" in DevicePath, and the remaining file path,
  "/foo/bar" in RemainingFilePath

--*/
{
  UINTN                     MappingLength;
  CHAR16                    *Mapping;
  EFI_STATUS                Status;

  // Separate CWD int "fs%d" filesystem mapping and file path
  MappingLength = 0;
  while (FilePath[MappingLength] && FilePath[MappingLength] != L':') {
    MappingLength++;
  }
  ASSERT (FilePath[MappingLength] != L'\0');
  MappingLength++;

  Mapping = SctAllocatePool ((MappingLength + 1) * sizeof (CHAR16));
  if (Mapping == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  SctStrnCpy (Mapping, FilePath, MappingLength);
  Mapping[MappingLength] = L'\0';

  Status = SctShellMapToDevicePath (Mapping, DevicePath);
  ASSERT_EFI_ERROR (Status);

  if (RemainingFilePath != NULL) {
    *RemainingFilePath = FilePath + MappingLength;
  }

  SctFreePool (Mapping);
  return Status;
}

EFI_STATUS
EFIAPI
SctShellOpenFileByDevicePath(
  IN OUT EFI_DEVICE_PATH_PROTOCOL     **FilePath,
  OUT EFI_HANDLE                      *DeviceHandle,
  OUT SHELL_FILE_HANDLE               *FileHandle,
  IN UINT64                           OpenMode,
  IN UINT64                           Attributes
  )
{
  CHAR16                          *FileName;
  EFI_STATUS                      Status;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *EfiSimpleFileSystemProtocol;
  EFI_FILE_PROTOCOL               *Handle1;
  EFI_FILE_PROTOCOL               *Handle2;
  CHAR16                          *FnafPathName;
  UINTN                           PathLen;

  if (FilePath == NULL || FileHandle == NULL || DeviceHandle == NULL) {
    return (EFI_INVALID_PARAMETER);
  }

  //
  // which shell interface should we use
  //
  if (gEfiShellProtocol != NULL) {
    //
    // use UEFI Shell 2.0 method.
    //
    FileName = gEfiShellProtocol->GetFilePathFromDevicePath(*FilePath);
    if (FileName == NULL) {
      return (EFI_INVALID_PARAMETER);
    }
    Status = SctShellOpenFileByName(FileName, FileHandle, OpenMode, Attributes);
    SctFreePool(FileName);
    return (Status);
  }


  //
  // use old shell method.
  //
  Status = tBS->LocateDevicePath (&gEfiSimpleFileSystemProtocolGuid,
                                  FilePath,
                                  DeviceHandle);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Status = tBS->OpenProtocol(*DeviceHandle,
                             &gEfiSimpleFileSystemProtocolGuid,
                             (VOID**)&EfiSimpleFileSystemProtocol,
                             gImageHandle,
                             NULL,
                             EFI_OPEN_PROTOCOL_GET_PROTOCOL);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Status = EfiSimpleFileSystemProtocol->OpenVolume(EfiSimpleFileSystemProtocol, &Handle1);
  if (EFI_ERROR (Status)) {
    FileHandle = NULL;
    return Status;
  }

  //
  // go down directories one node at a time.
  //
  while (!SctIsDevicePathEnd (*FilePath)) {
    //
    // For file system access each node should be a file path component
    //
    if (SctDevicePathType    (*FilePath) != MEDIA_DEVICE_PATH ||
        SctDevicePathSubType (*FilePath) != MEDIA_FILEPATH_DP
       ) {
      FileHandle = NULL;
      return (EFI_INVALID_PARAMETER);
    }
    //
    // Open this file path node
    //
    Handle2  = Handle1;
    Handle1 = NULL;

    //
    // File Name Alignment Fix (FNAF)
    // Handle2->Open may be incapable of handling a unaligned CHAR16 data.
    // The structure pointed to by FilePath may be not CHAR16 aligned.
    // This code copies the potentially unaligned PathName data from the
    // FilePath structure to the aligned FnafPathName for use in the
    // calls to Handl2->Open.
    //

    //
    // Determine length of PathName, in bytes.
    //
    PathLen = SctDevicePathNodeLength (*FilePath) - SIZE_OF_FILEPATH_DEVICE_PATH;

    //
    // Allocate memory for the aligned copy of the string Extra allocation is to allow for forced alignment
    // Copy bytes from possibly unaligned location to aligned location
    //
    FnafPathName = SctAllocateCopyPool(PathLen, (UINT8 *)((FILEPATH_DEVICE_PATH*)*FilePath)->PathName);
    if (FnafPathName == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    //
    // Try to test opening an existing file
    //
    Status = Handle2->Open (
                          Handle2,
                          &Handle1,
                          FnafPathName,
                          OpenMode &~EFI_FILE_MODE_CREATE,
                          0
                         );

    //
    // see if the error was that it needs to be created
    //
    if ((EFI_ERROR (Status)) && (OpenMode != (OpenMode &~EFI_FILE_MODE_CREATE))) {
      Status = Handle2->Open (
                            Handle2,
                            &Handle1,
                            FnafPathName,
                            OpenMode,
                            Attributes
                           );
    }

    //
    // Free the alignment buffer
    //
    SctFreePool(FnafPathName);

    //
    // Close the last node
    //
    Handle2->Close (Handle2);

    if (EFI_ERROR(Status)) {
      return (Status);
    }

    //
    // Get the next node
    //
    *FilePath = SctNextDevicePathNode (*FilePath);
  }

  //
  // This is a weak spot since if the undefined SHELL_FILE_HANDLE format changes this must change also!
  //
  *FileHandle = (VOID*)Handle1;
  return (EFI_SUCCESS);
}

EFI_STATUS
EFIAPI
SctShellOpenFileByName(
  IN CONST CHAR16               *FileName,
  OUT SHELL_FILE_HANDLE         *FileHandle,
  IN UINT64                     OpenMode,
  IN UINT64                     Attributes
  )
{
  EFI_HANDLE                    DeviceHandle;
  EFI_DEVICE_PATH_PROTOCOL      *FilePath;
  EFI_STATUS                    Status;
  EFI_FILE_INFO                 *FileInfo;

  //
  // ASSERT if FileName is NULL
  //
  ASSERT(FileName != NULL);

  if (FileName == NULL) {
    return (EFI_INVALID_PARAMETER);
  }

  if (gEfiShellProtocol != NULL) {
    if ((OpenMode & EFI_FILE_MODE_CREATE) == EFI_FILE_MODE_CREATE && (Attributes & EFI_FILE_DIRECTORY) == EFI_FILE_DIRECTORY) {
      return SctShellCreateDirectory(FileName, FileHandle);
    }
    //
    // Use UEFI Shell 2.0 method
    //
    Status = gEfiShellProtocol->OpenFileByName(FileName,
                                               FileHandle,
                                               OpenMode);
    if (SctStrCmp(FileName, L"NUL") != 0 && !EFI_ERROR(Status) && ((OpenMode & EFI_FILE_MODE_CREATE) != 0)){

/*
      FileInfo = FileFunctionMap.GetFileInfo(*FileHandle);
      ASSERT(FileInfo != NULL);
      FileInfo->Attribute = Attributes;
      Status = FileFunctionMap.SetFileInfo(*FileHandle, FileInfo);
      SctFreePool(FileInfo);
*/

      Status = SctGetFileInfo (*FileHandle, &FileInfo);
      ASSERT(FileInfo != NULL);
      FileInfo->Attribute = Attributes;
      Status = SctSetFileInfo (*FileHandle, FileInfo);
      SctFreePool(FileInfo);	  

    }
    return (Status);
  }
  //
  // Using EFI Shell version
  // this means convert name to path and call that function
  // since this will use EFI method again that will open it.
  //
  ASSERT(mEfiShellEnvironment2 != NULL);
  FilePath = mEfiShellEnvironment2->NameToPath ((CHAR16*)FileName);
  if (FilePath != NULL) {
    return (SctShellOpenFileByDevicePath(&FilePath,
                                      &DeviceHandle,
                                      FileHandle,
                                      OpenMode,
                                      Attributes));
  }
  return (EFI_DEVICE_ERROR);
}

EFI_STATUS
EFIAPI
SctShellCreateDirectory(
  IN CONST CHAR16             *DirectoryName,
  OUT SHELL_FILE_HANDLE       *FileHandle
  )
{
  if (gEfiShellProtocol != NULL) {
    //
    // Use UEFI Shell 2.0 method
    //
    return (gEfiShellProtocol->CreateFile(DirectoryName,
                          EFI_FILE_DIRECTORY,
                          FileHandle
                         ));
  } else {
    return (SctShellOpenFileByName(DirectoryName,
                                FileHandle,
                                EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE,
                                EFI_FILE_DIRECTORY
                               ));
  }
}