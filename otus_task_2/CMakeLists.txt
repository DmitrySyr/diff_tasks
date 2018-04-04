cmake_minimum_required(VERSION 3.2)



if($ENV{TRAVIS_BUILD_NUMBER})
    set(VERSION_BUILD $ENV{TRAVIS_BUILD_NUMBER})
else()
    set(VERSION_BUILD 0)
endif()



project(ip_config VERSION 0.0.${VERSION_BUILD})

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

# The version number.
#set (PROJECT_VERSION_PATCH "1.0")

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/version.h.in"
  "${PROJECT_BINARY_DIR}/version.h"
  )

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
#include_directories("${PROJECT_BINARY_DIR}")

add_executable(ip_config ip_config.cpp )

add_library(ip_config_lib lib.h lib.cpp)

add_executable(test_ip_fconfig tests.cpp)

set_target_properties(ip_config PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
  COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)

set_target_properties(test_ip_fconfig PROPERTIES
  COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
  INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

target_link_libraries(ip_config ip_config_lib)

target_link_libraries(test_ip_fconfig
  ${Boost_LIBRARIES}
  ip_config_lib
)

install(TARGETS ip_config RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT dmitry.syr@gmail.com)

include (CPack)

enable_testing()

add_test(test_code_valid test_ip_fconfig)
