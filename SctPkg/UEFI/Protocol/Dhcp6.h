/** @file

  Copyright 2006 - 2016 Unified EFI, Inc.<BR>
  Copyright (c) 2010 - 2016, Intel Corporation. All rights reserved.<BR>   

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at 
  http://opensource.org/licenses/bsd-license.php
 
  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
 
**/
/*++


Module Name:

  Dhcp6.h

Abstract:

  UEFI Dynamic Host Configuration Protocol 6 Definition

--*/

#ifndef _EFI_DHCP6_PROTOCOL_H_
#define _EFI_DHCP6_PROTOCOL_H_

#include <Protocol/ServiceBinding.h>

//GUID definitions
#define EFI_DHCP6_PROTOCOL_GUID \
  {0x87c8bad7, 0x595, 0x4053, {0x82, 0x97, 0xde, 0xde, 0x39, 0x5f, 0x5d, 0x5b }}

#define EFI_DHCP6_SERVICE_BINDING_PROTOCOL_GUID \
  {0x9fb9a8a1, 0x2f4a, 0x43a6, {0x88, 0x9c, 0xd0, 0xf7, 0xb6, 0xc4, 0x7a, 0xd5 }}

extern EFI_GUID gBlackBoxEfiDhcp6ProtocolGuid;
extern EFI_GUID gBlackBoxEfiDhcp6ServiceBindingProtocolGuid;

typedef struct _EFI_DHCP6_PROTOCOL EFI_DHCP6_PROTOCOL;

typedef EFI_SERVICE_BINDING_PROTOCOL EFI_DHCP6_SERVICE_BINDING_PROTOCOL;

typedef enum { 
  Dhcp6Init                    = 0x0, 
  Dhcp6Selecting               = 0x1,
  Dhcp6Requesting              = 0x2,
  Dhcp6Declining               = 0x3,
  Dhcp6Confirming              = 0x4,
  Dhcp6Releasing               = 0x5,
  Dhcp6Bound                   = 0x6,
  Dhcp6Renewing                = 0x7,
  Dhcp6Rebinding               = 0x8             
} EFI_DHCP6_STATE;

typedef enum {
  Dhcp6SendSolicit             = 0x0,
  Dhcp6RcvdAdvertise           = 0x1,
  Dhcp6SelectAdvertise         = 0x2,
  Dhcp6SendRequest             = 0x3,
  Dhcp6RcvdReply               = 0x4,
  Dhcp6RcvdReconfigure         = 0x5,
  Dhcp6SendDecline             = 0x6,
  Dhcp6SendConfirm             = 0x7,
  Dhcp6SendRelease             = 0x8,
  Dhcp6EnterRenewing           = 0x9,
  Dhcp6EnterRebinding          = 0xa
} EFI_DHCP6_EVENT;

#define EFI_DHCP6_IA_TYPE_NA   3
#define EFI_DHCP6_IA_TYPE_TA   4

#pragma pack(1)
typedef struct {
  UINT16                       OpCode;
  UINT16                       OpLen;
  UINT8                        Data[1];
} EFI_DHCP6_PACKET_OPTION;

typedef struct{
  UINT32                       MessageType:8;
  UINT32                       TransactionId:24;
} EFI_DHCP6_HEADER;

typedef struct {
  UINT32                       Size;
  UINT32                       Length;
  struct{
    EFI_DHCP6_HEADER           Header;
    UINT8                      Option[1];
  } Dhcp6;
} EFI_DHCP6_PACKET;

typedef struct {
  UINT16                       Length;
  UINT8                        Duid[1];
} EFI_DHCP6_DUID;

#pragma pack()

typedef struct {
  UINT32                       Irt;
  UINT32                       Mrc;
  UINT32                       Mrt;
  UINT32                       Mrd;
} EFI_DHCP6_RETRANSMISSION;

typedef struct {
  EFI_IPv6_ADDRESS             IpAddress;
  UINT32                       PreferredLifetime;
  UINT32                       ValidLifetime;
} EFI_DHCP6_IA_ADDRESS;

typedef struct {
  UINT16                       Type;
  UINT32                       IaId;
} EFI_DHCP6_IA_DESCRIPTOR;

typedef struct {
  EFI_DHCP6_IA_DESCRIPTOR      Descriptor;
  EFI_DHCP6_STATE              State;
  EFI_DHCP6_PACKET             *ReplyPacket;
  UINT32                       IaAddressCount;
  EFI_DHCP6_IA_ADDRESS         IaAddress[1];
} EFI_DHCP6_IA;

typedef struct {
  EFI_DHCP6_DUID               *ClientId;
  EFI_DHCP6_IA                 *Ia;
} EFI_DHCP6_MODE_DATA;

typedef 
EFI_STATUS 
(*EFI_DHCP6_CALLBACK) (
  IN EFI_DHCP6_PROTOCOL        *This,
  IN VOID                      *Context,
  IN EFI_DHCP6_STATE           CurrentState,
  IN EFI_DHCP6_EVENT           Dhcp6Event,
  IN EFI_DHCP6_PACKET          *Packet,        
  OUT EFI_DHCP6_PACKET         **NewPacket     OPTIONAL
  );

typedef struct {
  EFI_DHCP6_CALLBACK           Dhcp6Callback;
  VOID                         *CallbackContext;
  UINT32                       OptionCount;
  EFI_DHCP6_PACKET_OPTION      **OptionList;
  EFI_DHCP6_IA_DESCRIPTOR      IaDescriptor;
  EFI_EVENT                    IaInfoEvent;
  BOOLEAN                      ReconfigureAccept;
  BOOLEAN                      RapidCommit;
  EFI_DHCP6_RETRANSMISSION     *SolicitRetransmission;
} EFI_DHCP6_CONFIG_DATA;

typedef 
EFI_STATUS 
(*EFI_DHCP6_INFO_CALLBACK) (
  IN EFI_DHCP6_PROTOCOL        *This,
  IN VOID                      *Context,
  IN EFI_DHCP6_PACKET          *Packet
  );

typedef 
EFI_STATUS 
(EFIAPI *EFI_DHCP6_GET_MODE_DATA) (
  IN EFI_DHCP6_PROTOCOL        *This,
  OUT EFI_DHCP6_MODE_DATA      *Dhcp6ModeData,  OPTIONAL
  OUT EFI_DHCP6_CONFIG_DATA    *Dhcp6CfgData    OPTIONAL
  );

typedef 
EFI_STATUS
(EFIAPI *EFI_DHCP6_CONFIGURE) (
  IN EFI_DHCP6_PROTOCOL        *This,
  IN EFI_DHCP6_CONFIG_DATA     *Dhcp6CfgData    OPTIONAL
  );


typedef 
EFI_STATUS
(EFIAPI *EFI_DHCP6_START) (
  IN EFI_DHCP6_PROTOCOL        *This
  );

typedef
EFI_STATUS
(EFIAPI *EFI_DHCP6_INFO_REQUEST) (
  IN EFI_DHCP6_PROTOCOL        *This,
  IN BOOLEAN                   SendClientId,
  IN EFI_DHCP6_PACKET_OPTION   *OptionRequest,
  IN UINT32                    OptionCount,
  IN EFI_DHCP6_PACKET_OPTION   *OptionList[],   OPTIONAL
  IN EFI_DHCP6_RETRANSMISSION  *Retransmission, 
  IN EFI_EVENT                 TimeoutEvent,    OPTIONAL
  IN EFI_DHCP6_INFO_CALLBACK   ReplyCallback,
  IN VOID                      *CallbackContext OPTIONAL
  );

typedef
EFI_STATUS
(EFIAPI *EFI_DHCP6_RENEW_REBIND) (
  IN EFI_DHCP6_PROTOCOL        *This,
  IN BOOLEAN                   RebindRequest
  );

typedef 
EFI_STATUS
(EFIAPI *EFI_DHCP6_DECLINE) (
  IN EFI_DHCP6_PROTOCOL        *This,
  IN UINT32                    AddressCount,
  IN EFI_IPv6_ADDRESS          *Addresses
  );

typedef 
EFI_STATUS
(EFIAPI *EFI_DHCP6_RELEASE) (
  IN EFI_DHCP6_PROTOCOL        *This,
  IN UINT32                    AddressCount,
  IN EFI_IPv6_ADDRESS          *Addresses
  );

typedef 
EFI_STATUS
(EFIAPI *EFI_DHCP6_STOP) (
  IN EFI_DHCP6_PROTOCOL        *This
  );

typedef
EFI_STATUS
(EFIAPI *EFI_DHCP6_PARSE) (
  IN EFI_DHCP6_PROTOCOL        *This,
  IN EFI_DHCP6_PACKET          *Packet,
  IN OUT UINT32                *OptionCount,
  OUT EFI_DHCP6_PACKET_OPTION  *PacketOptionList[]  OPTIONAL
);

struct _EFI_DHCP6_PROTOCOL {
  EFI_DHCP6_GET_MODE_DATA      GetModeData;
  EFI_DHCP6_CONFIGURE          Configure;
  EFI_DHCP6_START              Start;
  EFI_DHCP6_INFO_REQUEST       InfoRequest;
  EFI_DHCP6_RENEW_REBIND       RenewRebind;
  EFI_DHCP6_DECLINE            Decline;
  EFI_DHCP6_RELEASE            Release;
  EFI_DHCP6_STOP               Stop;
  EFI_DHCP6_PARSE              Parse;
};

#endif
