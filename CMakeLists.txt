cmake_minimum_required(VERSION 3.16)
project(ultrix11 C ASM)

# Option for target architecture. Matches the Makefile variable.
set(ARCH "x86_64_v1" CACHE STRING "Target architecture")

# Global compile warnings for all targets
add_compile_options(-Wall -Wextra -Werror)

# Export compile settings from the environment when invoked via Makefile
if(DEFINED ENV{CC})
  set(CMAKE_C_COMPILER $ENV{CC} CACHE FILEPATH "" FORCE)
endif()
if(DEFINED ENV{CFLAGS})
  set(CMAKE_C_FLAGS "$ENV{CFLAGS} ${CMAKE_C_FLAGS}")
endif()
if(DEFINED ENV{AS})
  set(CMAKE_ASM_COMPILER $ENV{AS} CACHE FILEPATH "" FORCE)
endif()
if(DEFINED ENV{LDFLAGS})
  set(CMAKE_EXE_LINKER_FLAGS "$ENV{LDFLAGS} ${CMAKE_EXE_LINKER_FLAGS}" CACHE STRING "" FORCE)
endif()

add_subdirectory(src)
add_subdirectory(sys)

# Aggregate targets matching historical make workflow
add_custom_target(userland DEPENDS ${USERLAND_TARGETS})
add_custom_target(kernel DEPENDS ${KERNEL_TARGETS})
add_custom_target(default ALL DEPENDS userland kernel)
