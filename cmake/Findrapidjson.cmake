if(APPLE)
  set(RAPIDJSON_SEARCH_PATHS
      ~/Library/Frameworks
      /Library/Frameworks
      ${CMAKE_SOURCE_DIR}/lib
      /usr/local
      /usr
      /sw
      /opt/local
      /opt/csw)
else()
  set(RAPIDJSON_SEARCH_PATHS
      ${CMAKE_SOURCE_DIR}/lib
      /usr/local
      /usr)
endif()

find_path(RAPIDJSON_INCLUDE_DIR
          NAMES rapidjson/rapidjson.h
          PATHS ${RAPIDJSON_SEARCH_PATHS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(rapidjson
                                  DEFAULT_MSG 
                                  RAPIDJSON_INCLUDE_DIR
                                  RAPIDJSON_LIBRARY)
if(RAPIDJSON_FOUND)
    set(RAPIDJSON_INCLUDE_DIRS ${RAPIDJSON_INCLUDE_DIR})
    message(STATUS "Found rapidjson (include: ${RAPIDJSON_INCLUDE_DIRS}")
endif()