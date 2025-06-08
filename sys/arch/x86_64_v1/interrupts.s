/**
 * @file interrupts.s
 * @brief Interrupt vector stubs for x86_64.
 *
 * Each entry in `int_stub_table` jumps to the common handler
 * `int_common`. This code is only a placeholder for real
 * interrupt handling logic.
 */
.globl int_stub_table
int_stub_table:
    .rept 256
    .quad int_common
    .endr

.globl int_common
int_common:
    iretq
