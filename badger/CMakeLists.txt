cmake_minimum_required(VERSION 2.8)
		
project(badger)

file(
	GLOB_RECURSE
	source_files
    src/*.cpp 
    ../console/*.cpp 
    ../serial/src/*.cpp 
    ../serial/src/Posix/*.cpp
)

include_directories(../console/ ./include/ ../serial/include ../serial/src/ ../serial/src/Unix)

#add_definitions(-std=c++0x)

add_executable(
	badger
	${source_files}
)
