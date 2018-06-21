cmake_minimum_required(VERSION 3.2)

if($ENV{TRAVIS_BUILD_NUMBER})
    set(VERSION_BUILD $ENV{TRAVIS_BUILD_NUMBER})
else()
    set(VERSION_BUILD 0)
endif()

project(bulk VERSION 0.0.${VERSION_BUILD} LANGUAGES CXX)


#set (PROJECT_VERSION_PATCH "1.0")

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/version.h.in"
  "${PROJECT_BINARY_DIR}/version.h"
  )

#set(SOURCE_PATH "${PROJECT_SOURCE_DIR}")

add_executable(bulk main.cpp )

add_library(lib HW7.h)

target_link_libraries(bulk lib)

add_executable(bulk_test tests.cpp)

set_target_properties(bulk lib bulk_test PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
  LINKER_LANGUAGE CXX
  COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)

target_compile_definitions(bulk_test PRIVATE BOOST_TEST_DYN_LINK)

target_link_libraries(bulk_test 
  Boost::unit_test_framework
  lib
)

install(TARGETS bulk RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT dmitry.syr@gmail.com)

include (CPack)

enable_testing()

add_test(bulk_test bulk_tests)
