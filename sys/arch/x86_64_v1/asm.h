/**
 * @file asm.h
 * @brief Helper macros for low-level x86_64 assembly.
 */
#ifndef X86_64_V1_ASM_H
#define X86_64_V1_ASM_H

/** Halt the CPU. */
#define HALT() __asm__("hlt")

#endif /* X86_64_V1_ASM_H */
