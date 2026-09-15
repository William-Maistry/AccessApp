#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "ZXing::ZXing" for configuration "Debug"
set_property(TARGET ZXing::ZXing APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(ZXing::ZXing PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C;CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libZXing.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS ZXing::ZXing )
list(APPEND _IMPORT_CHECK_FILES_FOR_ZXing::ZXing "${_IMPORT_PREFIX}/lib/libZXing.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
