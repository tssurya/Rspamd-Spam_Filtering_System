# Install script for directory: /home/linux/rspamd/rspamd-0.8.1/src

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Debug")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FOREACH(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/rspamd-0.8.1"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/rspamd"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      FILE(RPATH_CHECK
           FILE "${file}"
           RPATH "/usr/local/lib/rspamd")
    ENDIF()
  ENDFOREACH()
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES
    "/home/linux/rspamd/rspamd.build/src/rspamd-0.8.1"
    "/home/linux/rspamd/rspamd.build/src/rspamd"
    )
  FOREACH(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/rspamd-0.8.1"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/rspamd"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      FILE(RPATH_CHANGE
           FILE "${file}"
           OLD_RPATH "/home/linux/rspamd/rspamd.build/src/libmime:/home/linux/rspamd/rspamd.build/src/libserver:/home/linux/rspamd/rspamd.build/src/libutil:/home/linux/rspamd/rspamd.build/src/lua:/home/linux/rspamd/rspamd.build/contrib/snowball:/home/linux/rspamd/rspamd.build/src/cdb:/home/linux/rspamd/rspamd.build/contrib/http-parser:"
           NEW_RPATH "/usr/local/lib/rspamd")
      IF(CMAKE_INSTALL_DO_STRIP)
        EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "${file}")
      ENDIF(CMAKE_INSTALL_DO_STRIP)
    ENDIF()
  ENDFOREACH()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("/home/linux/rspamd/rspamd.build/src/cdb/cmake_install.cmake")
  INCLUDE("/home/linux/rspamd/rspamd.build/src/ucl/cmake/cmake_install.cmake")
  INCLUDE("/home/linux/rspamd/rspamd.build/src/rdns/cmake_install.cmake")
  INCLUDE("/home/linux/rspamd/rspamd.build/src/lua/cmake_install.cmake")
  INCLUDE("/home/linux/rspamd/rspamd.build/src/libutil/cmake_install.cmake")
  INCLUDE("/home/linux/rspamd/rspamd.build/src/libserver/cmake_install.cmake")
  INCLUDE("/home/linux/rspamd/rspamd.build/src/libmime/cmake_install.cmake")
  INCLUDE("/home/linux/rspamd/rspamd.build/src/client/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

