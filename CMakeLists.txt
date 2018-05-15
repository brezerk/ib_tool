cmake_minimum_required (VERSION 2.8)

include(CheckCXXCompilerFlag)

project (ibtool CXX)

set (CMAKE_VERBOSE_MAKEFILE ON)
# Do not do stuff automatically
set (CMAKE_AUTOMOC OFF)

set (APP_VERSION "0.0.1a")
set (APP_NAME_GLOBAL "cpp_exercises")

if (POLICY CMP0011)
	cmake_policy(SET CMP0011 NEW)
endif (POLICY CMP0011)

if (POLICY CMP0017)
	cmake_policy (SET CMP0017 NEW)
endif (POLICY CMP0017)

# We have some custom .cmake scripts not in the official distribution.
# Maybe this can be used override existing behavior if needed?
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")

# Build options
option (DEBUG	"If ON, then build project with extra debug output. Use it in debug or bug report cases"	"OFF")
option (RELEASE	"Disable debug. Use it for user-ready buils."							"OFF")

# Banner
message (" * Just random cpp_exercises")
message (" * (C) 2018, Oleksii S. Malakhov")
message (" * https://brezblock.org.ua/\n")

if (${CMAKE_SYSTEM_NAME} MATCHES Linux)
	set(_OS_LINUX_ 1)
	add_definitions(-Wall -g -O0 -std=c++11)
endif (${CMAKE_SYSTEM_NAME} MATCHES Linux)

if (${CMAKE_SYSTEM_NAME} MATCHES FreeBSD)
	SET(_OS_FREEBSD_ 1)
	add_definitions(-Wall -g -DKERNEL -std=c++11)
endif (${CMAKE_SYSTEM_NAME} MATCHES FreeBSD)

if (${CMAKE_SYSTEM_NAME} MATCHES Darwin)
	link_directories(/usr/lib)
	set(_OS_DARWIN_ 1)
	add_definitions(-Wall -g -DKERNEL -std=c++11)
endif (${CMAKE_SYSTEM_NAME} MATCHES Darwin)

#if (NOT RELEASE)
        #set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -Wall")
	#Some extra (-Wshadow)
        #set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
	#And more extra
        #set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wstrict-aliasing -Wstrict-overflow")
	#And even more
        #check_cxx_compiler_flag(-pedantic HAVE_PEDANTIC_FLAG)
        #if (HAVE_PEDANTIC_FLAG)
        #	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
        #endif (HAVE_PEDANTIC_FLAG)
#endif (NOT RELEASE)

message(STATUS "Build realm is   : ${CMAKE_SYSTEM_NAME} ${CMAKE_HOST_SYSTEM_PROCESSOR}\n")

#find_package(Boost COMPONENTS filesystem system serialization regex unit_test_framework REQUIRED)
find_package(Boost COMPONENTS filesystem system regex REQUIRED)

configure_file (${CMAKE_SOURCE_DIR}/config.hpp.cmake ${CMAKE_SOURCE_DIR}/config.hpp)
file (APPEND ${CMAKE_SOURCE_DIR}/config.hpp "#define APP_PREF \"${CMAKE_SOURCE_DIR}\"\n")
file (APPEND ${CMAKE_SOURCE_DIR}/config.hpp "#define APP_VERS \"${APP_VERSION}\"\n")

include_directories(
	${CMAKE_SOURCE_DIR}
#	${Boost_INCLUDE_DIRS}
)

set (CORE_HEADERS
	${CMAKE_SOURCE_DIR}/config.hpp
        ${CMAKE_SOURCE_DIR}/core/utils.hpp
        ${CMAKE_SOURCE_DIR}/core/ibf_parser.hpp
        ${CMAKE_SOURCE_DIR}/core/ibf_node.hpp
)

set (CORE_SOURCES
	${CMAKE_SOURCE_DIR}/main.cpp
        ${CMAKE_SOURCE_DIR}/core/utils.cpp
        ${CMAKE_SOURCE_DIR}/core/ibf_parser.cpp
        ${CMAKE_SOURCE_DIR}/core/ibf_node.cpp
)

set (CORE_LIBRARIES
        ${Boost_LIBRARIES}
)

add_executable (ibtool ${CORE_HEADERS} ${CORE_SOURCES})
target_link_libraries (ibtool ${CORE_LIBRARIES})

# requres boot::test framework. we won't use it here, right? :)
#add_subdirectory(test)

#enable_testing()

# Unintall target
configure_file (
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY
    )

add_custom_target (uninstall
    "${CMAKE_COMMAND}"
    -P
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    )

# EOF
