cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
	
project(badger)

option(BUILD_STATIC_PROGRAM "TRUE to build the program statically, otherwise not" FALSE)

# Detect the OS
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(WINDOWS 1)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(LINUX 1)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
    # FreeBSD compile path is the same as Linux
    set(LINUX 1)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOSX 1)
endif()


set(MYSQL_FIND_REQUIRED TRUE)
include("FindMySQL.cmake")


# Detect the compiler and its version
# Note: on some platforms (OS X), CMAKE_COMPILER_IS_GNUCXX is true
# even when CLANG is used, therefore the Clang test is done first

# CMAKE_CXX_COMPILER_ID is an internal CMake variable subject to change,
# but there is no other way to detect CLang at the moment
if(CMAKE_CXX_COMPILER MATCHES ".*clang[+][+]" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    execute_process(COMMAND "${CMAKE_CXX_COMPILER}" "--version" OUTPUT_VARIABLE CLANG_VERSION_OUTPUT)
    string(REGEX REPLACE ".*clang version ([0-9]+\\.[0-9]+).*" "\\1" CLANG_VERSION "${CLANG_VERSION_OUTPUT}")

    add_definitions(-std=c++0x)

elseif(CMAKE_COMPILER_IS_GNUCXX)
    execute_process(COMMAND "${CMAKE_CXX_COMPILER}" "-dumpversion" OUTPUT_VARIABLE GCC_VERSION_OUTPUT)

    if (GCC_VERSION_OUTPUT VERSION_LESS 4.3)
        message(FATAL_ERROR "Need gcc >= 4.3")
    endif()

    add_definitions(-std=c++0x)

endif()


# Set default cmake build type to Release (None Debug Release RelWithDebInfo MinSizeRel)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()


# Add communs files and directories
file(GLOB COMMUN_SOURCES_FILES src/*.cpp ../console/*.cpp ../serial/src/*.cpp)
include_directories(../console/ ./include/ ../serial/include ../serial/src/)


# Add platform-dependant files and directories
if(LINUX OR MACOSX)
    file(GLOB PLATFORM_SOURCES_FILES ../serial/src/Posix/*.cpp)
    include_directories(../serial/src/Unix)

elseif(WINDOWS)
    file(GLOB PLATFORM_SOURCES_FILES ../serial/src/Win32/*.cpp)
    include_directories(../serial/src/Win32)
endif()


# Add the executable
add_executable(badger ${COMMUN_SOURCES_FILES} ${PLATFORM_SOURCES_FILES})


# Compile with static library is wanted
if(CMAKE_COMPILER_IS_GNUCXX)
    if(BUILD_STATIC_PROGRAM)
        set_target_properties(badger PROPERTIES LINK_FLAGS "--static")
    endif()
endif()


# Config link stage
target_link_libraries(badger ${MYSQL_LIBRARY})

