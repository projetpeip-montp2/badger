# Find mysqlclient
# Find the native MySQL includes and library
#
# MYSQL_INCLUDE_DIR - where to find mysql.h, etc.
# MYSQL_LIBRARIES   - List of libraries when using MySQL.
# MYSQL_FOUND       - True if MySQL found.

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)


# Find mysql include dir
find_path(MYSQL_INCLUDE_DIR mysql.h
    /usr/local/include/mysql
    /usr/include/mysql
	MYSQL/
	C:/MinGW/include/MYSQL
)	


# Find mysql libs
set(MYSQL_NAMES mysqlclient mysqlclient_r)
find_library(MYSQL_LIBRARY NAMES ${MYSQL_NAMES}
    PATHS /usr/lib /usr/local/lib lib/ C:/MinGW/lib 
    PATH_SUFFIXES mysql
)


# Check final status
if(MYSQL_INCLUDE_DIR AND MYSQL_LIBRARY)
    set(MYSQL_FOUND TRUE)
    set(MYSQL_LIBRARIES ${MYSQL_LIBRARY})

    message(STATUS "Found MySQL: ${MYSQL_LIBRARY}")
   
else()
    if(MYSQL_FIND_REQUIRED)
	    set(MYSQL_FOUND FALSE)
		set(MYSQL_LIBRARIES )
	
        message(STATUS "Looked for MySQL libraries named ${MYSQL_NAMES}.")
        message(FATAL_ERROR "Could NOT find MySQL library")
    endif()
endif()


MARK_AS_ADVANCED(MYSQL_LIBRARY MYSQL_INCLUDE_DIR)
