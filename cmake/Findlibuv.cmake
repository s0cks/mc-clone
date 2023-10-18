# Standard FIND_PACKAGE module for libuv, sets the following variables:
#   - LIBUV_FOUND
#   - LIBUV_INCLUDE_DIRS (only if LIBUV_FOUND)
#   - LIBUV_LIBRARIES (only if LIBUV_FOUND)

# Try to find the header
if(APPLE)
    set(LIBUV_SEARCH_PATHS
        ~/Library/Frameworks
        /Library/Frameworks
        /usr/local
        /usr
        /sw # Fink
        /opt/local # DarwinPorts
        /opt/csw # Blastwave
        /opt)
else()
    set(LIBUV_SEARCH_PATHS
        /usr/local
        /usr)
endif()

find_path(LIBUV_INCLUDE_DIR uv.h
          HINTS
          PATH_SUFFIXES include/uv include
          PATHS ${LIBUV_SEARCH_PATHS})
find_library(LIBUV_LIBRARY 
             NAMES uv libuv
             PATHS ${LIBUV_SEARCH_PATHS})

# Try to find the library


# Handle the QUIETLY/REQUIRED arguments, set LIBUV_FOUND if all variables are
# found
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(libuv
    REQUIRED_VARS
    LIBUV_LIBRARY
    LIBUV_INCLUDE_DIR)

# Hide internal variables
mark_as_advanced(LIBUV_INCLUDE_DIR LIBUV_LIBRARY)

# Set standard variables
IF(LIBUV_FOUND)
    SET(LIBUV_INCLUDE_DIRS "${LIBUV_INCLUDE_DIR}")
    SET(LIBUV_LIBRARIES "${LIBUV_LIBRARY}")
ENDIF()