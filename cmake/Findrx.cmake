if(APPLE)
    set(RX_SEARCH_PATHS
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
    set(RX_SEARCH_PATHS
        ${CMAKE_SOURCE_DIR}/lib
        /usr/local
        /usr)
endif()


find_path(RX_INCLUDE_DIR rxcpp/rx.hpp
          HINTS
          PATH_SUFFIXES rx
          PATHS ${NK_SEARCH_PATHS})

# Handle the QUIETLY/REQUIRED arguments, set LIBUV_FOUND if all variables are
# found
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(rx
                                  REQUIRED_VARS
                                  RX_INCLUDE_DIR)

# Hide internal variables
mark_as_advanced(RX_INCLUDE_DIR)

# Set standard variables
IF(RX_FOUND)
    SET(RX_INCLUDE_DIRS "${RX_INCLUDE_DIR}")
    message("found rxcpp. (include: ${RX_INCLUDE_DIRS})")
else()
    message("cannot find rxcpp.")
ENDIF()