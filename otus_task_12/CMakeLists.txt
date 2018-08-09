cmake_minimum_required(VERSION 3.2)

if($ENV{TRAVIS_BUILD_NUMBER})
    set(VERSION_BUILD $ENV{TRAVIS_BUILD_NUMBER})
else()
    set(VERSION_BUILD 0)
endif()

project(BulkServer VERSION 0.0.${VERSION_BUILD} LANGUAGES CXX)

#set (PROJECT_VERSION_PATCH "1.0")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

find_package(Boost COMPONENTS system thread unit_test_framework REQUIRED)
find_package(Threads)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/version.h.in"
  "${PROJECT_BINARY_DIR}/version.h"
  )

#set(SOURCE_PATH "${PROJECT_SOURCE_DIR}")

add_executable(bulk_server main.cpp)

add_library(bulk_server_lib Client.h Client.cpp StartTask.h H12.h Processor.h Queue.h thread_wrapper.h)

target_link_libraries(bulk_server bulk_server_lib ${CMAKE_THREAD_LIBS_INIT} stdc++fs ${Boost_LIBRARIES})
target_include_directories(bulk_server ${Boost_INCLUDE_DIR})
target_include_directories(bulk_server_lib ${Boost_INCLUDE_DIR})

set_target_properties(bulk_server bulk_server_lib PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
  LINKER_LANGUAGE CXX
  LINKER_FLAGS -lstdc++fs
  COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
  COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)

install(TARGETS bulk_server RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT dmitry.syr@gmail.com)

include (CPack)

#enable_testing()

#add_subdirectory(Tests)

MESSAGE(STATUS "!!!!! BOOST LIBS LIST: ${Boost_LIBRARIES}")
