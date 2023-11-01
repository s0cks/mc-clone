if(APPLE)
  set(LEVELDB_SEARCH_PATHS
      ~/Library/Frameworks
      /Library/Frameworks
      /usr/local
      /usr
      /sw # Fink
      /opt/local # DarwinPorts
      /opt/csw # Blastwave
      /opt)
else()
  set(LEVELDB_SEARCH_PATHS
      /usr/local
      /usr)
endif()

find_path(
   LEVELDB_INCLUDE_DIR
   NAMES leveldb/db.h
   PATHS ${LEVELDB_SEARCH_PATHS})
find_library(
   LEVELDB_LIBRARY
   NAMES leveldb
   PATHS ${LEVELDB_SEARCH_PATHS})
find_library(
  LEVELDB_SNAPPY_LIBRARY
  NAMES snappy
  PATHS ${LEVELDB_SEARCH_PATHS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(leveldb DEFAULT_MSG LEVELDB_INCLUDE_DIR LEVELDB_LIBRARY)
if(LEVELDB_FOUND)
    set(LEVELDB_INCLUDE_DIRS ${LEVELDB_INCLUDE_DIR})
    set(LEVELDB_LIBRARIES ${LEVELDB_LIBRARY} ${LEVELDB_SNAPPY_LIBRARY})
    message(STATUS "Found leveldb (include: ${LEVELDB_INCLUDE_DIRS}, libraries: ${LEVELDB_LIBRARIES})")
endif()