cmake_minimum_required(VERSION 2.6)
	
project(badger)


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


# For GNU C++ compiler add support for c++11 standard
if (CMAKE_COMPILER_IS_GNUCXX)
    execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)

    if (GCC_VERSION VERSION_LESS 4.3)
        message(FATAL_ERROR "gcc version < 4.3")
    endif()

    add_definitions(-std=c++0x)
endif()


add_executable(badger ${COMMUN_SOURCES_FILES} ${PLATFORM_SOURCES_FILES})

