#
# Scratchbox/N900/Maemo toolchain file for CMake
#
# (c) Copyrights 2009-2011 Hartmut Seichter
# 
# Note: only tested on Linux
#

set(SCRATCHBOX_PATH "/opt/scratchbox" CACHE STRING "Scratchbox Root Path")

set(SCRATCHBOX_TARGET "armv6" CACHE STRING "Scratchbox Target")

set(SCRATCHBOX_TARGET_PATH "${SCRATCHBOX_PATH}/users/$ENV{USERNAME}/targets/${SCRATCHBOX_TARGET}" CACHE STRING "Scratchbox User Space Path")


set(TARGET_ROOT "${SCRATCHBOX_TARGET_PATH}/usr" CACHE STRING "root path")

message(STATUS "${TARGET_ROOT}")

# 
#
# 
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 1)

set(CMAKE_SYSTEM_PROCESSOR arm-eabi)


# cs2007q3-glibc2.5-arm7
# cs2007q3-glibc2.5-i486
set(SCRATCHBOX_CS_COMPILER "cs2009q1-eglibc2.8-armv7" CACHE STRING "Scratchbox CodeSourcery Compiler")

set(CMAKE_C_COMPILER ${SCRATCHBOX_PATH}/compilers/${SCRATCHBOX_CS_COMPILER}/bin/arm-none-linux-gnueabi-gcc)
set(CMAKE_CXX_COMPILER ${SCRATCHBOX_PATH}/compilers/${SCRATCHBOX_CS_COMPILER}/bin/arm-none-linux-gnueabi-g++)

set(CMAKE_C_FLAGS "-mcpu=arm926ej-s -march=armv5te -mfpu=vfp -mfloat-abi=soft"
	CACHE STRING "Scratchbox - GCC/C flags" FORCE
)

set(CMAKE_CXX_FLAGS 
	${CMAKE_C_FLAGS}
	CACHE STRING "Scratchbox - GCC/C++ flags" FORCE
)

set(CMAKE_FIND_ROOT_PATH
	${SCRATCHBOX_TARGET_PATH}
	${SCRATCHBOX_TARGET_PATH}/usr
	)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
 
