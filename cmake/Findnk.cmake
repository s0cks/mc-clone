if(APPLE)
    set(NK_SEARCH_PATHS
        ${CMAKE_SOURCE_DIR}/lib
        ~/Library/Frameworks
        /Library/Frameworks
        /usr/local
        /usr
        /sw # Fink
        /opt/local # DarwinPorts
        /opt/csw # Blastwave
        /opt)
else()
    set(NK_SEARCH_PATHS
        ${CMAKE_SOURCE_DIR}/lib
        /usr/local
        /usr)
endif()


find_path(NK_INCLUDE_DIR nuklear.h
          HINTS
          PATH_SUFFIXES nuklear
          PATHS ${NK_SEARCH_PATHS})

# Handle the QUIETLY/REQUIRED arguments, set LIBUV_FOUND if all variables are
# found
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(nk
                                  REQUIRED_VARS
                                  NK_INCLUDE_DIR)

# Hide internal variables
mark_as_advanced(NK_INCLUDE_DIR)

# Set standard variables
IF(NK_FOUND)
    SET(NK_INCLUDE_DIRS "${NK_INCLUDE_DIR}")
    message("found nuklear. (include: ${NK_INCLUDE_DIRS})")
ENDIF()