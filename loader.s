.set MAGIC, 0x1badb002; # so that the bootloader can recognize the file as a kernel.
.set FLAGS, (1<<0 | 1<<1)
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot; stores some data.
    .long MAGIC
    .long FLAGS
    .long CHECKSUM
    


.section .text
.extern kernelMain
.global loader

loader:
    mov $kernel_stack, %esp
    push %eax ; # pointer to the multibpoot structure
    push %ebx ; # magic number
    call kernelMain

_stop:
    cli
    hlt
    jmp _stop



.section .bss
.space 2*1024*1024 ;  # 2 Mib, in case stack overwrite some other data

kernel_stack:
