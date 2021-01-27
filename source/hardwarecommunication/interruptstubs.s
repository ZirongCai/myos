
.set IRQ_BASE, 0x20

.section .text

.extern _ZN4myos21hardwarecommunication16InterruptManager15handleInterruptEhj




.macro HandleException num
.global _ZN4myos21hardwarecommunication16InterruptManager16HandleInterruptRequest\num\()Ev
_ZN4myos21hardwarecommunication16InterruptManager16HandleInterruptRequest\num\()Ev:
    movb $\num, (interruptnumber)
    jmp int_bottom
.endm

.macro HandleInterruptRequest num
.global _ZN4myos21hardwarecommunication16InterruptManager26HandleInterruptRequest\num\()Ev
_ZN4myos21hardwarecommunication16InterruptManager26HandleInterruptRequest\num\()Ev:
    movb $\num + IRQ_BASE, (interruptnumber)
    pushl $0
    jmp int_bottom
.endm

HandleInterruptRequest 0x00
HandleInterruptRequest 0x01
HandleInterruptRequest 0x0C



int_bottom:

    #save registers
    #pusha
    #pushl %ds #pushl:= push dword
    #pushl %es
    #pushl %fs
    #pushl %gs

    pushl %ebp
    pushl %edi
    pushl %esi

    pushl %edx
    pushl %ecx
    pushl %ebx
    pushl %eax


    #call C++ Handler
    pushl %esp
    push (interruptnumber)
    call _ZN4myos21hardwarecommunication16InterruptManager15handleInterruptEhj


    movl %eax, %esp #switch stack


    #restore registers
    popl %eax
    popl %ebx
    popl %ecx
    popl %edx

    popl %esi
    popl %edi
    popl %ebp

    #popl %gs
    #popl %fs
    #popl %es
    #popl %ds
    #popa


    add $4, %esp



.global _ZN4myos21hardwarecommunication16InterruptManager22IgnoreInterruptRequestEv
_ZN4myos21hardwarecommunication16InterruptManager22IgnoreInterruptRequestEv:

    iret

.data
    interruptnumber: .byte 0
    
