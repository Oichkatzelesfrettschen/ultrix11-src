# Build rules for the kernel stub

add_executable(kernel-${ARCH} arch/${ARCH}/stub.c)

if (CMAKE_C_COMPILER_ID MATCHES "Clang|GNU")
  target_compile_options(kernel-${ARCH} PRIVATE -Wall -Wextra -Werror)
endif()

set(KERNEL_TARGETS kernel-${ARCH})
set(KERNEL_TARGETS ${KERNEL_TARGETS} PARENT_SCOPE)
