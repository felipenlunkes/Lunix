# Copyright (c) 2022, Felipe Miguel Nery Lunkes
# All rights reserved.

# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice, this
#  list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright notice,
#  this list of conditions and the following disclaimer in the documentation
#  and/or other materials provided with the distribution.
#
# * Neither the name of the copyright holder nor the names of its
#  contributors may be used to endorse or promote products derived from
#  this software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

.section .text
.global switchTask

switchTask:

    pusha
    pushf

    mov %cr3, %eax     # Push CR3

    push %eax

    mov 44(%esp), %eax # The first argument, where to save
    mov %ebx, 4(%eax)
    mov %ecx, 8(%eax)
    mov %edx, 12(%eax)
    mov %esi, 16(%eax)
    mov %edi, 20(%eax)
    mov 36(%esp), %ebx # EAX
    mov 40(%esp), %ecx # IP
    mov 20(%esp), %edx # ESP
    add $4, %edx       # Remove the return value ;)
    mov 16(%esp), %esi # EBP
    mov 4(%esp), %edi  # EFLAGS
    mov %ebx, (%eax)
    mov %edx, 24(%eax)
    mov %esi, 28(%eax)
    mov %ecx, 32(%eax)
    mov %edi, 36(%eax)
    
    pop %ebx #CR3
    
    mov %ebx, 40(%eax)
    
    push %ebx #Goodbye again ;)
    
    mov 48(%esp), %eax  # Now it is the new object
    mov 4(%eax), %ebx   # EBX
    mov 8(%eax), %ecx   # ECX
    mov 12(%eax), %edx  # EDX
    mov 16(%eax), %esi  # ESI
    mov 20(%eax), %edi  # EDI
    mov 28(%eax), %ebp  # EBP
    
    push %eax
   
    mov 36(%eax), %eax  # EFLAGS
    
    push %eax
    popf
    pop %eax
    
    mov 24(%eax), %esp  # ESP
    
    push %eax
    mov 40(%eax), %eax  # CR3
    mov %eax, %cr3
    
    pop %eax
    push %eax
    
    mov 32(%eax), %eax  # EIP
    xchg (%esp), %eax   # We do not have any more registers to use as tmp storage
   
    mov (%eax), %eax    # EAX
    
    ret                 # This ends all!
