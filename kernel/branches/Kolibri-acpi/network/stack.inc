;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;                                                                 ;;
;; Copyright (C) KolibriOS team 2004-2015. All rights reserved.    ;;
;; Distributed under terms of the GNU General Public License       ;;
;;                                                                 ;;
;;  STACK.INC                                                      ;;
;;                                                                 ;;
;;  TCP/IP stack for KolibriOS                                     ;;
;;                                                                 ;;
;;    Written by hidnplayr@kolibrios.org                           ;;
;;                                                                 ;;
;;     Some parts of code are based on the work of:                ;;
;;      Mike Hibbett (menuetos network stack)                      ;;
;;      Eugen Brasoveanu (solar os network stack and drivers)      ;;
;;      mike.dld (kolibrios socket code)                           ;;
;;                                                                 ;;
;;     TCP part is based on 4.4BSD                                 ;;
;;                                                                 ;;
;;          GNU GENERAL PUBLIC LICENSE                             ;;
;;             Version 2, June 1991                                ;;
;;                                                                 ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

$Revision: 5565 $

uglobal
        net_10ms        dd ?
        net_tmr_count   dw ?
endg

DEBUG_NETWORK_ERROR     = 1
DEBUG_NETWORK_VERBOSE   = 0

NET_DEVICES_MAX         = 16
NET_BUFFERS             = 512
NET_BUFFER_SIZE         = 2048
ARP_BLOCK               = 1             ; true or false

EPHEMERAL_PORT_MIN      = 49152
EPHEMERAL_PORT_MAX      = 61000
MIN_EPHEMERAL_PORT_N    = 0x00C0        ; same in Network byte order (FIXME)
MAX_EPHEMERAL_PORT_N    = 0x48EE        ; same in Network byte order (FIXME)

; Ethernet protocol numbers
ETHER_PROTO_ARP                 = 0x0608
ETHER_PROTO_IPv4                = 0x0008
ETHER_PROTO_IPv6                = 0xDD86
ETHER_PROTO_PPP_DISCOVERY       = 0x6388
ETHER_PROTO_PPP_SESSION         = 0x6488

; Internet protocol numbers
IP_PROTO_IP             = 0
IP_PROTO_ICMP           = 1
IP_PROTO_TCP            = 6
IP_PROTO_UDP            = 17

; PPP protocol numbers
PPP_PROTO_IPv4          = 0x2100
PPP_PROTO_IPV6          = 0x5780
PPP_PROTO_ETHERNET      = 666           ; FIXME

;Protocol family
AF_UNSPEC               = 0
AF_LOCAL                = 1
AF_INET4                = 2
AF_INET6                = 10
AF_PPP                  = 777           ; FIXME

; Socket types
SOCK_STREAM             = 1
SOCK_DGRAM              = 2
SOCK_RAW                = 3

; Socket options
SO_ACCEPTCON            = 1 shl 0
SO_BROADCAST            = 1 shl 1
SO_DEBUG                = 1 shl 2
SO_DONTROUTE            = 1 shl 3
SO_KEEPALIVE            = 1 shl 4
SO_OOBINLINE            = 1 shl 5
SO_REUSEADDR            = 1 shl 6
SO_REUSEPORT            = 1 shl 7
SO_USELOOPBACK          = 1 shl 8
SO_BINDTODEVICE         = 1 shl 9

SO_NONBLOCK             = 1 shl 31

; Socket flags for user calls
MSG_PEEK                = 0x02
MSG_DONTWAIT            = 0x40

; Socket level
SOL_SOCKET              = 0


; Socket States
SS_NOFDREF              = 0x0001        ; no file table ref any more
SS_ISCONNECTED          = 0x0002        ; socket connected to a peer
SS_ISCONNECTING         = 0x0004        ; in process of connecting to peer
SS_ISDISCONNECTING      = 0x0008        ; in process of disconnecting
SS_CANTSENDMORE         = 0x0010        ; can't send more data to peer
SS_CANTRCVMORE          = 0x0020        ; can't receive more data from peer
SS_RCVATMARK            = 0x0040        ; at mark on input
SS_ISABORTING           = 0x0080        ; aborting fd references - close()
SS_RESTARTSYS           = 0x0100        ; restart blocked system calls
SS_ISDISCONNECTED       = 0x0800        ; socket disconnected from peer

SS_ASYNC                = 0x1000        ; async i/o notify
SS_ISCONFIRMING         = 0x2000        ; deciding to accept connection req
SS_MORETOCOME           = 0x4000

SS_BLOCKED              = 0x8000


SOCKET_MAXDATA          = 4096*64       ; must be 4096*(power of 2) where 'power of 2' is at least 8
MAX_backlog             = 20            ; maximum backlog for stream sockets

; Error Codes
ENOBUFS                 = 1
EINPROGRESS             = 2
EOPNOTSUPP              = 4
EWOULDBLOCK             = 6
ENOTCONN                = 9
EALREADY                = 10
EINVAL                  = 11
EMSGSIZE                = 12
ENOMEM                  = 18
EADDRINUSE              = 20
ECONNREFUSED            = 61
ECONNRESET              = 52
EISCONN                 = 56
ETIMEDOUT               = 60
ECONNABORTED            = 53

; Api protocol numbers
API_ETH                 = 0
API_IPv4                = 1
API_ICMP                = 2
API_UDP                 = 3
API_TCP                 = 4
API_ARP                 = 5
API_PPPOE               = 6
API_IPv6                = 7

; Network device types
NET_DEVICE_LOOPBACK     = 0
NET_DEVICE_ETH          = 1
NET_DEVICE_SLIP         = 2

; Network link types (link protocols)
NET_LINK_LOOPBACK       = 0
NET_LINK_MAC            = 1     ; Media access control (ethernet, isdn, ...)
NET_LINK_PPP            = 2     ; Point to Point Protocol (PPPoE, ...)
NET_LINK_IEEE802.11     = 3     ; IEEE 802.11 (WiFi)

; Hardware acceleration bits
NET_HWACC_TCP_IPv4_IN   = 1 shl 0
NET_HWACC_TCP_IPv4_OUT  = 1 shl 1

; Network frame types
NET_BUFF_LOOPBACK      = 0
NET_BUFF_ETH           = 1

struct  NET_DEVICE

        device_type     dd ?    ; Type field
        mtu             dd ?    ; Maximal Transmission Unit
        name            dd ?    ; Ptr to 0 terminated string

        unload          dd ?    ; Ptrs to driver functions
        reset           dd ?    ;
        transmit        dd ?    ;

        bytes_tx        dq ?    ; Statistics, updated by the driver
        bytes_rx        dq ?    ;
        packets_tx      dd ?    ;
        packets_rx      dd ?    ;

        link_state      dd ?    ; link state (0 = no link)
        hwacc           dd ?    ; bitmask stating enabled HW accelerations (offload engines)

ends

struct  NET_BUFF

        NextPtr         dd ?    ; pointer to next frame in list
        PrevPtr         dd ?    ; pointer to previous frame in list
        device          dd ?    ; ptr to NET_DEVICE structure
        type            dd ?    ; encapsulation type: e.g. Ethernet
        length          dd ?    ; size of encapsulated data
        offset          dd ?    ; offset to actual data (24 bytes for default frame)
        data            rb 0

ends


; Exactly as it says..
macro pseudo_random reg {
        add     reg, [esp]
        rol     reg, 5
        xor     reg, [timer_ticks]
;        add     reg, [CPU_FREQ]
        imul    reg, 214013
        xor     reg, 0xdeadbeef
        rol     reg, 9
}

; Network to Hardware byte order (dword)
macro ntohd reg {

        rol     word reg, 8
        rol     dword reg, 16
        rol     word reg , 8

}

; Network to Hardware byte order (word)
macro ntohw reg {

        rol     word reg, 8

}


include "queue.inc"

include "loopback.inc"
include "ethernet.inc"

include "PPPoE.inc"

include "ARP.inc"
include "IPv4.inc"
include "IPv6.inc"

include "icmp.inc"
include "udp.inc"
include "tcp.inc"

include "socket.inc"



uglobal
align 4

        NET_RUNNING     dd ?
        NET_DRV_LIST    rd NET_DEVICES_MAX

        NET_BUFFS_FREE  rd NET_BUFFERS
        .current        dd ?

endg


;-----------------------------------------------------------------
;
; stack_init
;
;  This function calls all network init procedures
;
;  IN:  /
;  OUT: /
;
;-----------------------------------------------------------------
align 4
stack_init:

; allocate network buffers
        stdcall kernel_alloc, NET_BUFFER_SIZE*NET_BUFFERS
        test    eax, eax
        jz      .fail

        mov     edi, NET_BUFFS_FREE
        mov     ecx, NET_BUFFERS
        cld
  .loop:
        stosd
        add     eax, NET_BUFFER_SIZE
        dec     ecx
        jnz     .loop

        mov     eax, NET_BUFFS_FREE
        stosd

; Init the network drivers list
        xor     eax, eax
        mov     edi, NET_RUNNING
        mov     ecx, (NET_DEVICES_MAX + 1)
        rep stosd

        ETH_init

        PPPoE_init

        IPv4_init
;        IPv6_init
        ICMP_init

        ARP_init
        UDP_init
        TCP_init

        SOCKET_init

        LOOP_init

        mov     [net_tmr_count], 0
        ret

  .fail:
        DEBUGF  DEBUG_NETWORK_ERROR, "Stack init failed!\n"
        ret



; Wakeup every tick.
proc stack_handler_has_work?

        mov     eax, [timer_ticks]
        cmp     eax, [net_10ms]

        ret
endp


;-----------------------------------------------------------------
;
; stack_handler
;
;  This function is called in kernel loop
;
;  IN:  /
;  OUT: /
;
;-----------------------------------------------------------------
align 4
stack_handler:

        ; Test for 10ms tick
        mov     eax, [timer_ticks]
        cmp     eax, [net_10ms]
        je      .exit
        mov     [net_10ms], eax

        cmp     [NET_RUNNING], 0
        je      .exit

        test    [net_10ms], 0x0f        ; 160ms
        jnz     .exit

        TCP_timer_160ms

        test    [net_10ms], 0x3f        ; 640ms
        jnz     .exit

        ARP_decrease_entry_ttls
        IPv4_decrease_fragment_ttls

        xor     edx, edx
        mov     eax, [TCP_timer1_event]
        mov     ebx, [eax + EVENT.id]
        xor     esi, esi
        call    raise_event

  .exit:
        ret


align 4
proc NET_BUFF_alloc stdcall, buffersize
        cmp     [buffersize], NET_BUFFER_SIZE
        ja      .too_large

        spin_lock_irqsave

        mov     eax, [NET_BUFFS_FREE.current]
        cmp     eax, NET_BUFFS_FREE+NET_BUFFERS*4
        jae     .out_of_mem
        mov     eax, [eax]
        add     [NET_BUFFS_FREE.current], 4

        spin_unlock_irqrestore

        DEBUGF  DEBUG_NETWORK_VERBOSE, "net alloc: 0x%x\n", eax
        ret

  .out_of_mem:
        spin_unlock_irqrestore

        xor     eax, eax
        DEBUGF  DEBUG_NETWORK_ERROR, "NET_BUFF_alloc: out of mem!\n"
        ret

  .too_large:
        xor     eax, eax
        DEBUGF  DEBUG_NETWORK_ERROR, "NET_BUFF_alloc: too large!\n"
        ret
endp


align 4
proc NET_BUFF_free stdcall, buffer

        DEBUGF  DEBUG_NETWORK_VERBOSE, "net free: 0x%x\n", [buffer]

        spin_lock_irqsave

        sub     [NET_BUFFS_FREE.current], 4
        mov     eax, [NET_BUFFS_FREE.current]
        push    [buffer]
        pop     dword[eax]

        spin_unlock_irqrestore

        ret
endp


align 4
NET_link_changed:

        DEBUGF  DEBUG_NETWORK_VERBOSE, "NET_link_changed device=0x%x status=0x%x\n", ebx, [ebx + NET_DEVICE.link_state]

align 4
NET_send_event:

        DEBUGF  DEBUG_NETWORK_VERBOSE, "NET_send_event\n"

; Send event to all applications
        push    edi ecx
        mov     edi, SLOT_BASE
        mov     ecx, [TASK_COUNT]
  .loop:
        add     edi, 256
        or      [edi + APPDATA.event_mask], EVENT_NETWORK2
        loop    .loop
        pop     ecx edi

        ret



;-----------------------------------------------------------------
;
; NET_add_device:
;
;  This function is called by the network drivers,
;  to register each running NIC to the kernel
;
;  IN:  Pointer to device structure in ebx
;  OUT: Device num in eax, -1 on error
;
;-----------------------------------------------------------------
align 4
NET_add_device:

        DEBUGF  DEBUG_NETWORK_VERBOSE, "NET_Add_Device: %x\n", ebx   ;;; TODO: use mutex to lock net device list

        cmp     [NET_RUNNING], NET_DEVICES_MAX
        jae     .error

;----------------------------------
; Check if device is already listed
        mov     eax, ebx
        mov     ecx, NET_DEVICES_MAX    ; We need to check whole list because a device may be removed without re-organizing list
        mov     edi, NET_DRV_LIST

        repne scasd                     ; See if device is already in the list
        jz      .error

;----------------------------
; Find empty slot in the list
        xor     eax, eax
        mov     ecx, NET_DEVICES_MAX
        mov     edi, NET_DRV_LIST

        repne scasd
        jnz     .error

        sub     edi, 4

;-----------------------------
; Add device to the found slot
        mov     [edi], ebx              ; add device to list

        mov     eax, edi                ; Calculate device number in eax
        sub     eax, NET_DRV_LIST
        shr     eax, 2

        inc     [NET_RUNNING]           ; Indicate that one more network device is up and running

        call    NET_send_event

        DEBUGF  DEBUG_NETWORK_VERBOSE, "Device number: %u\n", eax
        ret

  .error:
        or      eax, -1
        DEBUGF  DEBUG_NETWORK_ERROR, "Adding network device failed\n"
        ret



;-----------------------------------------------------------------
;
; NET_Remove_Device:
;
;  This function is called by network drivers,
;  to unregister network devices from the kernel
;
;  IN:  Pointer to device structure in ebx
;  OUT: eax: -1 on error
;
;-----------------------------------------------------------------
align 4
NET_remove_device:

        cmp     [NET_RUNNING], 0
        je      .error

;----------------------------
; Find the driver in the list

        mov     eax, ebx
        mov     ecx, NET_DEVICES_MAX
        mov     edi, NET_DRV_LIST

        repne scasd
        jnz     .error

;------------------------
; Remove it from the list

        xor     eax, eax
        mov     dword [edi-4], eax
        dec     [NET_RUNNING]

        call    NET_send_event

        xor     eax, eax
        ret

  .error:
        or      eax, -1
        ret



;-----------------------------------------------------------------
;
; NET_ptr_to_num
;
; IN:  ebx = ptr to device struct
; OUT: edi = -1 on error, device number otherwise
;
;-----------------------------------------------------------------
align 4
NET_ptr_to_num:

        call    NET_ptr_to_num4
        ror     edi, 2          ; If -1, stay -1
                                ; valid device numbers have last two bits 0, so do just shr

        ret

align 4
NET_ptr_to_num4:                ; Todo, place number in device structure so we only need to verify?

        test    ebx, ebx
        jz      .fail

        push    ecx
        mov     ecx, NET_DEVICES_MAX
        mov     edi, NET_DRV_LIST
  .loop:
        cmp     ebx, [edi]
        je      .found
        add     edi, 4
        dec     ecx
        jnz     .loop

        pop     ecx
  .fail:
        or      edi, -1
        ret

  .found:
        sub     edi, NET_DRV_LIST
        pop     ecx
        ret

;-----------------------------------------------------------------
;
; checksum_1
;
;  This is the first of two functions needed to calculate a checksum.
;
;  IN:  edx = start offset for semi-checksum
;       esi = pointer to data
;       ecx = data size
;  OUT: edx = semi-checksum
;
;
; Code was optimized by diamond
;
;-----------------------------------------------------------------
align 4
checksum_1:

        shr     ecx, 1
        pushf
        jz      .no_2

        shr     ecx, 1
        pushf
        jz      .no_4

        shr     ecx, 1
        pushf
        jz      .no_8

  .loop:
        add     dl, [esi+1]
        adc     dh, [esi+0]

        adc     dl, [esi+3]
        adc     dh, [esi+2]

        adc     dl, [esi+5]
        adc     dh, [esi+4]

        adc     dl, [esi+7]
        adc     dh, [esi+6]

        adc     edx, 0
        add     esi, 8

        dec     ecx
        jnz     .loop

        adc     edx, 0

  .no_8:
        popf
        jnc     .no_4

        add     dl, [esi+1]
        adc     dh, [esi+0]

        adc     dl, [esi+3]
        adc     dh, [esi+2]

        adc     edx, 0
        add     esi, 4

  .no_4:
        popf
        jnc     .no_2

        add     dl, [esi+1]
        adc     dh, [esi+0]

        adc     edx, 0
        inc     esi
        inc     esi

  .no_2:
        popf
        jnc     .end

        add     dh, [esi+0]
        adc     edx, 0
  .end:
        ret

;-----------------------------------------------------------------
;
; checksum_2
;
;  This function calculates the final ip/tcp/udp checksum for you
;
;  IN:  edx = semi-checksum
;  OUT: dx = checksum (in INET byte order)
;
;-----------------------------------------------------------------
align 4
checksum_2:

        mov     ecx, edx
        shr     ecx, 16
        and     edx, 0xffff
        add     edx, ecx

        mov     ecx, edx
        shr     ecx, 16
        add     dx, cx
        test    dx, dx          ; it seems that ZF is not set when CF is set :(
        not     dx
        jnz     .not_zero
        dec     dx
  .not_zero:
        xchg    dl, dh

        DEBUGF  DEBUG_NETWORK_VERBOSE, "Checksum: %x\n", dx

        ret



;----------------------------------------------------------------
;
;  System function to work with network devices (74)
;
;----------------------------------------------------------------
align 4
sys_network:

        cmp     bl, 255
        jne     @f

        mov     eax, [NET_RUNNING]
        mov     [esp+32], eax
        ret

   @@:
        cmp     bh, NET_DEVICES_MAX             ; Check if device number exists
        jae     .doesnt_exist

        mov     esi, ebx
        and     esi, 0x0000ff00
        shr     esi, 6

        cmp     dword [esi + NET_DRV_LIST], 0   ; check if driver is running
        je      .doesnt_exist

        mov     eax, [esi + NET_DRV_LIST]

        and     ebx, 0x000000ff
        cmp     ebx, .number
        ja      .doesnt_exist
        jmp     dword [.table + 4*ebx]

  .table:
        dd      .get_type               ; 0
        dd      .get_dev_name           ; 1
        dd      .reset                  ; 2
        dd      .stop                   ; 3
        dd      .get_ptr                ; 4
        dd      .get_drv_name           ; 5

        dd      .packets_tx             ; 6
        dd      .packets_rx             ; 7
        dd      .bytes_tx               ; 8
        dd      .bytes_rx               ; 9
        dd      .state                  ; 10
  .number = ($ - .table) / 4 - 1

  .get_type:
        mov     eax, [eax + NET_DEVICE.device_type]
        mov     [esp+32], eax
        ret

  .get_dev_name:
        mov     esi, [eax + NET_DEVICE.name]
        mov     edi, ecx

        mov     ecx, 64/4 ; max length
        rep movsd

        xor     eax, eax
        mov     [esp+32], eax
        ret

  .reset:
        call    [eax + NET_DEVICE.reset]
        mov     [esp+32], eax
        ret

  .stop:
        call    [eax + NET_DEVICE.unload]
        mov     [esp+32], eax
        ret


  .get_ptr:
        mov     [esp+32], eax
        ret


  .get_drv_name:
        xor     eax, eax
        mov     [esp+32], eax
        ret

  .packets_tx:
        mov     eax, [eax + NET_DEVICE.packets_tx]
        mov     [esp+32], eax
        ret

  .packets_rx:
        mov     eax, [eax + NET_DEVICE.packets_rx]
        mov     [esp+32], eax
        ret

  .bytes_tx:
        mov     ebx, dword [eax + NET_DEVICE.bytes_tx + 4]
        mov     [esp+20], ebx
        mov     eax, dword [eax + NET_DEVICE.bytes_tx]
        mov     [esp+32], eax
        ret

  .bytes_rx:
        mov     ebx, dword [eax + NET_DEVICE.bytes_rx + 4]
        mov     [esp+20], ebx
        mov     eax, dword [eax + NET_DEVICE.bytes_rx]
        mov     [esp+32], eax
        ret

  .state:
        mov     eax, [eax + NET_DEVICE.link_state]
        mov     [esp+32], eax
        ret


  .doesnt_exist:
        mov     dword[esp+32], -1
        ret



;----------------------------------------------------------------
;
;  System function to work with protocols  (76)
;
;----------------------------------------------------------------
align 4
sys_protocols:
        cmp     bh, NET_DEVICES_MAX             ; Check if device number exists
        jae     .doesnt_exist

        mov     esi, ebx
        and     esi, 0x0000ff00
        shr     esi, 6                          ; now we have the device num * 4 in esi
        cmp     [esi + NET_DRV_LIST], 0         ; check if driver is running
        je      .doesnt_exist

        push    .return                         ; return address (we will be using jumps instead of calls)

        mov     eax, ebx                        ; set ax to protocol number
        shr     eax, 16                         ;

        cmp     ax, API_ETH
        je      ETH_api

        cmp     ax, API_IPv4
        je      IPv4_api

        cmp     ax, API_ICMP
        je      ICMP_api

        cmp     ax, API_UDP
        je      UDP_api

        cmp     ax, API_TCP
        je      TCP_api

        cmp     ax, API_ARP
        je      ARP_api

        cmp     ax, API_PPPOE
        je      PPPoE_api

        cmp     ax, API_IPv6
        je      IPv6_api

        add     esp, 4                           ; if we reached here, no function was called, so we need to balance stack

  .doesnt_exist:
        mov     eax, -1

  .return:
        mov     [esp+28+4], eax                 ; return eax value to the program
        ret
