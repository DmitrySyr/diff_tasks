cmake_minimum_required(VERSION 3.2)

if($ENV{TRAVIS_BUILD_NUMBER})
    set(VERSION_BUILD $ENV{TRAVIS_BUILD_NUMBER})
else()
    set(VERSION_BUILD 0)
endif()

project(allocator VERSION 0.0.${VERSION_BUILD} LANGUAGES CXX)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

#set (PROJECT_VERSION_PATCH "1.0")

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/version.h.in"
  "${PROJECT_BINARY_DIR}/version.h"
  )

#set(SOURCE_PATH "${PROJECT_SOURCE_DIR}")

add_executable(allocator main.cpp )

add_library(alllib pool_allocator.h m_list.h)

target_link_libraries(allocator alllib)

add_executable(allocator_tests tests.cpp)

set_target_properties(allocator alllib allocator_tests PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
  LINKER_LANGUAGE CXX
  COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)

target_compile_definitions(allocator_tests PRIVATE BOOST_TEST_DYN_LINK)

target_link_libraries(allocator_tests
  Boost::unit_test_framework
  alllib
)

install(TARGETS allocator RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT dmitry.syr@gmail.com)

include (CPack)

enable_testing()

add_test(test_fact_func allocator_tests)
add_test(test_suite_m_list allocator_tests)
add_test(test_allocator allocator_tests)
