cmake_minimum_required(VERSION 3.4 FATAL_ERROR)

if (POLICY CMP0042)
    cmake_policy(SET CMP0042 NEW) # MACOSX_RPATH is enabled by default.
endif()
if (POLICY CMP0054)
    cmake_policy(SET CMP0054 NEW)
endif()
if (POLICY CMP0068)
    cmake_policy(SET CMP0068 NEW) # RPATH settings on macOS do not affect install_name.
endif()
if (POLICY CMP0074)
    cmake_policy(SET CMP0074 NEW)
endif()
if (POLICY CMP0075)
    cmake_policy(SET CMP0075 NEW)
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release
        CACHE STRING "Set build type to Debug or Release (default Release)" FORCE)
    message(STATUS "Set CMAKE_BUILD_TYPE to Release (default)")
endif()
string(TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_LOWER)

project(cmake_inspect C)

include(GNUInstallDirs)

macro(Configure_RPATH)
    # NOTE: that CMAKE_INSTALL_FULL_LIBDIR not always normalized correctly, i.e.:
    # - "///" -> "/"
    # - "/////usr///" -> "//usr"
    # So it should be normalized again.
    get_filename_component(CMAKE_INSTALL_LIBDIR_NORMALIZED "${CMAKE_INSTALL_FULL_LIBDIR}" REALPATH)
    list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_LIBDIR_NORMALIZED}" isSystemDir)

    if(DEFINED CMAKE_INSTALL_RPATH)
        message(STATUS "CMAKE_INSTALL_RPATH already set")
    elseif("${isSystemDir}" STREQUAL "-1")
        message(STATUS "Set CMAKE_INSTALL_RPATH={CMAKE_INSTALL_LIBDIR_NORMALIZED}")
        set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_LIBDIR_NORMALIZED}")
    else()
        message(STATUS "Detected install to system directory")
    endif()
endmacro()
Configure_RPATH()

add_library(dummy_shared SHARED dummy.c)

set_target_properties(dummy_shared PROPERTIES
    OUTPUT_NAME "dummy"
    VERSION "0.0"
    SOVERSION "0"
    PUBLIC_HEADER dummy.h
)

add_executable(testmain main.c)
target_link_libraries(testmain dummy_shared)

install(
    TARGETS dummy_shared testmain
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

message(STATUS "CMAKE_SKIP_RPATH:         ${CMAKE_SKIP_RPATH}")
message(STATUS "CMAKE_SKIP_INSTALL_RPATH: ${CMAKE_SKIP_INSTALL_RPATH}")
message(STATUS "CMAKE_INSTALL_RPATH:      ${CMAKE_INSTALL_RPATH}")
message(STATUS "CMAKE_VERBOSE_MAKEFILE:   ${CMAKE_VERBOSE_MAKEFILE}")
message(STATUS "CMAKE_MACOSX_RPATH:       ${CMAKE_MACOSX_RPATH}")
message(STATUS "CMAKE_INSTALL_NAME_DIR:   ${CMAKE_INSTALL_NAME_DIR}")
