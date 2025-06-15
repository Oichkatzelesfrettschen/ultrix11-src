# Build rules for userland utilities

# Collect a small representative program
add_executable(ac cmd/ac.c)

# Enable warnings for modern compilation
if (CMAKE_C_COMPILER_ID MATCHES "Clang|GNU")
  target_compile_options(ac PRIVATE -Wall -Wextra)
endif()

set(USERLAND_TARGETS ac)
set(USERLAND_TARGETS ${USERLAND_TARGETS} PARENT_SCOPE)
