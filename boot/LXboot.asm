;; Copyright (c) 2018, Carlos Fenollosa
;; Copyright (c) 2022, Felipe Miguel Nery Lunkes
;; All rights reserved.

;; Redistribution and use in source and binary forms, with or without
;; modification, are permitted provided that the following conditions are met:
;;
;; * Redistributions of source code must retain the above copyright notice, this
;;  list of conditions and the following disclaimer.
;;
;; * Redistributions in binary form must reproduce the above copyright notice,
;;  this list of conditions and the following disclaimer in the documentation
;;  and/or other materials provided with the distribution.
;;
;; * Neither the name of the copyright holder nor the names of its
;;  contributors may be used to endorse or promote products derived from
;;  this software without specific prior written permission.
;; 
;; THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
;; AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
;; IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
;; DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
;; FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
;; DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
;; SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
;; CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
;; OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
;; OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

; Identical to lesson 13's boot sector, but the %included files have new paths
[org 0x7c00]

KERNEL_OFFSET equ 0x1000 ; The same one we used when linking the kernel

    mov [BOOT_DRIVE], dl ; Remember that the BIOS sets us the boot drive in 'dl' on boot
    mov bp, 0x9000
    mov sp, bp

    mov bx, MSG_REAL_MODE 

    call print
    call print_nl

    call load_kernel ; read the kernel from disk
    call switch_to_pm ; disable interrupts, load GDT,  etc. Finally jumps to 'BEGIN_PM'

    jmp $ ; Never executed

%include "boot/LXprint.asm"
%include "boot/LXhex.asm"
%include "boot/LXdisk.asm"
%include "boot/LXgdt.asm"
%include "boot/LXconsole.asm"
%include "boot/LXpm.asm"

[bits 16]

load_kernel:

    mov bx, MSG_LOAD_KERNEL

    call print
    call print_nl

    mov bx, KERNEL_OFFSET ; Read from disk and store in 0x1000
    mov dh, 31 ; Our future kernel will be larger, make this big
    mov dl, [BOOT_DRIVE]

    call disk_load

    ret

[bits 32]

BEGIN_PM:

    mov ebx, MSG_PROT_MODE
   
    call print_string_pm
    call KERNEL_OFFSET ; Give control to the kernel
    
    jmp $ ; Stay here when the kernel returns control to us (if ever)


BOOT_DRIVE db 0 ; It is a good idea to store it in memory because 'dl' may get overwritten
MSG_REAL_MODE db 10, 13, "Lunix loader v0.0.0.1", 0
MSG_PROT_MODE db "We are now in 32-bit Protected Mode", 0
MSG_LOAD_KERNEL db "Loading the Lunix kernel", 0
MSG_RETURNED_KERNEL db "Returned from kernel. Error?", 0

; padding

times 510 - ($-$$) db 0
dw 0xaa55
