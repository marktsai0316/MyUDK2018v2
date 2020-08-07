/** @file

  Copyright 2006 - 2014 Unified EFI, Inc.<BR>
  Copyright (c) 2010, Intel Corporation. All rights reserved.<BR>
  Copyright (c) 2014, ARM Ltd. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at 
  http://opensource.org/licenses/bsd-license.php
 
  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
 
**/
/*++

  Linked List Library Functions

--*/

#include "SctLib.h"


VOID
SctInitializeListHead (
  SCT_LIST_ENTRY       *List
  )
/*++

Routine Description:

  Initialize the head of the List.  The caller must allocate the memory
  for the EFI_LIST. This function must be called before the other linked
  list macros can be used.

Arguments:

  List - Pointer to list head to initialize

Returns:

  None.

--*/

{
  List->ForwardLink = List;
  List->BackLink    = List;
}


BOOLEAN
SctIsListEmpty (
  CONST SCT_LIST_ENTRY  *List
  )
/*++

Routine Description:

  Return TRUE is the list contains zero nodes. Otherzise return FALSE.
  The list must have been initialized with InitializeListHead () before using
  this function.

Arguments:

  List - Pointer to list head to test


Returns:

  Return TRUE is the list contains zero nodes. Otherzise return FALSE.

--*/
{
  return (BOOLEAN)(List->ForwardLink == List);
}


VOID
SctRemoveEntryList (
  SCT_LIST_ENTRY  *Entry
  )
/*++

Routine Description:

  Remove Node from the doubly linked list. It is the caller's responsibility
  to free any memory used by the entry if needed. The list must have been
  initialized with InitializeListHead () before using this function.

Arguments:

  Entry - Element to remove from the list.

Returns:

  None

--*/
{
  SCT_LIST_ENTRY  *_ForwardLink;
  SCT_LIST_ENTRY  *_BackLink;

  _ForwardLink           = Entry->ForwardLink;
  _BackLink              = Entry->BackLink;
  _BackLink->ForwardLink = _ForwardLink;
  _ForwardLink->BackLink = _BackLink;

  DEBUG_CODE (
    Entry->ForwardLink = (SCT_LIST_ENTRY *) EFI_BAD_POINTER;
    Entry->BackLink    = (SCT_LIST_ENTRY *) EFI_BAD_POINTER;
  );
}


VOID
SctInsertTailList (
  SCT_LIST_ENTRY  *ListHead,
  SCT_LIST_ENTRY  *Entry
  )
/*++

Routine Description:

  Insert a Node into the end of a doubly linked list. The list must have
  been initialized with InitializeListHead () before using this function.

Arguments:

  ListHead - Head of doubly linked list

  Entry    - Element to insert at the end of the list.

Returns:

  None

--*/
{
  SCT_LIST_ENTRY *_ListHead;
  SCT_LIST_ENTRY *_BackLink;

  _ListHead              = ListHead;
  _BackLink              = _ListHead->BackLink;
  Entry->ForwardLink     = _ListHead;
  Entry->BackLink        = _BackLink;
  _BackLink->ForwardLink = Entry;
  _ListHead->BackLink    = Entry;
}



VOID
SctInsertHeadList (
  SCT_LIST_ENTRY  *ListHead,
  SCT_LIST_ENTRY  *Entry
  )
/*++

Routine Description:

  Insert a Node into the start of a doubly linked list. The list must have
  been initialized with InitializeListHead () before using this function.

Arguments:

  ListHead - Head of doubly linked list

  Entry    - Element to insert to beginning of list

Returns:

  None

--*/
{
  SCT_LIST_ENTRY  *_ListHead;
  SCT_LIST_ENTRY  *_ForwardLink;

  _ListHead               = ListHead;
  _ForwardLink            = _ListHead->ForwardLink;
  Entry->ForwardLink      = _ForwardLink;
  Entry->BackLink         = _ListHead;
  _ForwardLink->BackLink  = Entry;
  _ListHead->ForwardLink  = Entry;
}

VOID
SctSwapListEntries (
  SCT_LIST_ENTRY  *Entry1,
  SCT_LIST_ENTRY  *Entry2
  )
/*++

Routine Description:

  Swap the location of the two elements of a doubly linked list. Node2
  is placed in front of Node1. The list must have been initialized with
  InitializeListHead () before using this function.

Arguments:

  Entry1 - Element in the doubly linked list in front of Node2.

  Entry2 - Element in the doubly linked list behind Node1.

Returns:

  None

--*/
{
  SCT_LIST_ENTRY *Entry1BackLink;
  SCT_LIST_ENTRY *Entry2ForwardLink;
  SCT_LIST_ENTRY *Entry2BackLink;

  Entry2ForwardLink           = Entry2->ForwardLink;
  Entry2BackLink              = Entry2->BackLink;
  Entry1BackLink              = Entry1->BackLink;
  Entry2BackLink->ForwardLink = Entry2ForwardLink;
  Entry2ForwardLink->BackLink = Entry2BackLink;
  Entry2->ForwardLink         = Entry1;
  Entry2->BackLink            = Entry1BackLink;
  Entry1BackLink->ForwardLink = Entry2;
  Entry1->BackLink            = Entry2;
}


SCT_LIST_ENTRY *
SctGetFirstNode (
  CONST SCT_LIST_ENTRY  *List
  )
/*++

Routine Description:

  Return the first node pointed to by the list head.  The list must
  have been initialized with InitializeListHead () before using this
  function and must contain data.

Arguments:

  List - The head of the doubly linked list.

Returns:

  Pointer to the first node, if the list contains nodes.  The list will
  return a null value--that is, the value of List--when the list is empty.
    See the description of SctIsNull for more information.


--*/
{
  return List->ForwardLink;
}


SCT_LIST_ENTRY *
SctGetNextNode (
  CONST SCT_LIST_ENTRY  *List,
  CONST SCT_LIST_ENTRY  *Node
  )
/*++

Routine Description:

  Returns the node following Node in the list. The list must
  have been initialized with InitializeListHead () before using this
  function and must contain data.

Arguments:

  List - The head of the list.  MUST NOT be the literal value NULL.
  Node - The node in the list.  This value MUST NOT be the literal value NULL.
    See the description of SctIsNull for more information.

Returns:

  Pointer to the next node, if one exists.  Otherwise, returns a null value,
  which is actually a pointer to List.
    See the description of SctIsNull for more information.

--*/
{
  if (Node == List) {
    return (SCT_LIST_ENTRY *) List;
  }
  return Node->ForwardLink;
}


BOOLEAN
SctIsNull (
  CONST SCT_LIST_ENTRY  *List,
  CONST SCT_LIST_ENTRY  *Node
  )
/*++

Routine Description:

  Determines whether the given node is null.  Note that Node is null
  when its value is equal to the value of List.  It is an error for
  Node to be the literal value NULL [(VOID*)0x0].

Arguments:

  List - The head of the list.  MUST NOT be the literal value NULL.
  Node - The node to test.  MUST NOT be the literal value NULL.  See
    the description above.

Returns:

  Returns true if the node is null.

--*/
{
  return (BOOLEAN)(Node == List);
}


BOOLEAN
SctIsNodeAtEnd (
  CONST SCT_LIST_ENTRY  *List,
  CONST SCT_LIST_ENTRY  *Node
  )
/*++

Routine Description:

  Determines whether the given node is at the end of the list.  Used
  to walk the list.  The list must have been initialized with
  InitializeListHead () before using this function and must contain
  data.

Arguments:

  List - The head of the list.  MUST NOT be the literal value NULL.
  Node - The node to test.  MUST NOT be the literal value NULL.
    See the description of SctIsNull for more information.

Returns:

  Returns true if the list is the tail.

--*/
{
  if (SctIsNull (List, Node)) {
    return FALSE;
  }
  return (BOOLEAN)(List->BackLink == Node);
}

