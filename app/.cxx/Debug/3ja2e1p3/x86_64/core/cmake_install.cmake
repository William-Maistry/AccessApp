# Install script for directory: C:/Android Studio Gemini Test/Scanner/app/src/main/cpp/core

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/OpenScanSA")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/Users/William Maistry/AppData/Local/Android/Sdk/ndk/29.0.13846066/toolchains/llvm/prebuilt/windows-x86_64/bin/llvm-objdump.exe")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Android Studio Gemini Test/Scanner/app/.cxx/Debug/3ja2e1p3/x86_64/core/libZXing.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ZXing" TYPE FILE FILES
    "C:/Android Studio Gemini Test/Scanner/app/src/main/cpp/core/src/Barcode.h"
    "C:/Android Studio Gemini Test/Scanner/app/src/main/cpp/core/src/BarcodeFormat.h"
    "C:/Android Studio Gemini Test/Scanner/app/src/main/cpp/core/src/CharacterSet.h"
    "C:/Android Studio Gemini Test/Scanner/app/src/main/cpp/core/src/ContentType.h"
    "C:/Android Studio Gemini Test/Scanner/app/src/main/cpp/core/src/CreateBarcode.h"
    "C:/Android Studio Gemini Test/Scanner/app/src/main/cpp/core/src/Error.h"
    "C:/Android Studio Gemini Test/Scanner/app/src/main/cpp/core/src/GTIN.h"
    "C:/Android Studio Gemini Test/Scanner/app/src/main/cpp/core/src/ImageView.h"
    "C:/Android Studio Gemini Test/Scanner/app/src/main/cpp/core/src/Point.h"
    "C:/Android Studio Gemini Test/Scanner/app/src/main/cpp/core/src/Quadrilateral.h"
    "C:/Android Studio Gemini Test/Scanner/app/src/main/cpp/core/src/ReadBarcode.h"
    "C:/Android Studio Gemini Test/Scanner/app/src/main/cpp/core/src/ReaderOptions.h"
    "C:/Android Studio Gemini Test/Scanner/app/src/main/cpp/core/src/WriteBarcode.h"
    "C:/Android Studio Gemini Test/Scanner/app/src/main/cpp/core/src/ZXingCpp.h"
    "C:/Android Studio Gemini Test/Scanner/app/src/main/cpp/core/src/ZXVersion.h"
    "C:/Android Studio Gemini Test/Scanner/app/src/main/cpp/core/src/ZXingQt.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ZXing" TYPE FILE FILES "C:/Android Studio Gemini Test/Scanner/app/.cxx/Debug/3ja2e1p3/x86_64/core/Version.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/ZXing/ZXingTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/ZXing/ZXingTargets.cmake"
         "C:/Android Studio Gemini Test/Scanner/app/.cxx/Debug/3ja2e1p3/x86_64/core/CMakeFiles/Export/lib/cmake/ZXing/ZXingTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/ZXing/ZXingTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/ZXing/ZXingTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/ZXing" TYPE FILE FILES "C:/Android Studio Gemini Test/Scanner/app/.cxx/Debug/3ja2e1p3/x86_64/core/CMakeFiles/Export/lib/cmake/ZXing/ZXingTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/ZXing" TYPE FILE FILES "C:/Android Studio Gemini Test/Scanner/app/.cxx/Debug/3ja2e1p3/x86_64/core/CMakeFiles/Export/lib/cmake/ZXing/ZXingTargets-debug.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "C:/Android Studio Gemini Test/Scanner/app/.cxx/Debug/3ja2e1p3/x86_64/core/zxing.pc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/ZXing" TYPE FILE FILES
    "C:/Android Studio Gemini Test/Scanner/app/.cxx/Debug/3ja2e1p3/x86_64/core/ZXingConfig.cmake"
    "C:/Android Studio Gemini Test/Scanner/app/.cxx/Debug/3ja2e1p3/x86_64/core/ZXingConfigVersion.cmake"
    )
endif()

