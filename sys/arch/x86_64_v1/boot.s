/**
 * @file boot.s
 * @brief Minimal x86_64 bootstrap stub.
 *
 * This placeholder simply jumps to `kernel_main` after disabling
 * interrupts. It halts the processor if `kernel_main` returns.
 */
.globl _start
_start:
    cli                     # disable interrupts
    call kernel_main        # jump to kernel entry
1:
    hlt                     # halt CPU
    jmp 1b                  # loop forever
