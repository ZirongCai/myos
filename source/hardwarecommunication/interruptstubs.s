
.set IRQ_BASE, 0x20

.section .text

.extern _ZN16InterruptManager15handleInterruptEhj


.global _ZN16InterruptManager22IgnoreInterruptRequestEv


.macro HandleException num
.global _ZN16InterruptManager16HandleInterruptRequest\num\()Ev
_ZN16InterruptManager16HandleInterruptRequest\num\()Ev:
    movb $\num, (interruptnumber)
    jmp int_bottom
.endm

.macro HandleInterruptRequest num
.global _ZN16InterruptManager26HandleInterruptRequest\num\()Ev
_ZN16InterruptManager26HandleInterruptRequest\num\()Ev:
    movb $\num + IRQ_BASE, (interruptnumber)
    jmp int_bottom
.endm

HandleInterruptRequest 0x00
HandleInterruptRequest 0x01
HandleInterruptRequest 0x0C



int_bottom:

    pusha
    pushl %ds #pushl:= push dword
    pushl %es
    pushl %fs
    pushl %gs

    pushl %esp;
    push (interruptnumber)
    call _ZN16InterruptManager15handleInterruptEhj
    #addl $5, %esp; to delete the number from stack, but since we will overwrite the stack later so its not neccesarry.
    movl %eax, %esp 


    popl %gs
    popl %fs
    popl %es
    popl %ds
    popa


_ZN16InterruptManager22IgnoreInterruptRequestEv:

    iret

.data
    interruptnumber: .byte 0
    
