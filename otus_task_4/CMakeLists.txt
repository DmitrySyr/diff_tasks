cmake_minimum_required(VERSION 3.2)

if($ENV{TRAVIS_BUILD_NUMBER})
    set(VERSION_BUILD $ENV{TRAVIS_BUILD_NUMBER})
else()
    set(VERSION_BUILD 0)
endif()

project(print_ip VERSION 0.0.${VERSION_BUILD} LANGUAGES CXX)


#set (PROJECT_VERSION_PATCH "1.0")

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/version.h.in"
  "${PROJECT_BINARY_DIR}/version.h"
  )

#set(SOURCE_PATH "${PROJECT_SOURCE_DIR}")

add_executable(print main.cpp )

add_library(print_lib print_ip.h)

target_link_libraries(print print_lib)

add_executable(print_ip_test tests.cpp)

set_target_properties(print print_lib print_ip_test PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
  LINKER_LANGUAGE CXX
  COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)

target_compile_definitions(print_ip_test PRIVATE BOOST_TEST_DYN_LINK)

target_link_libraries(print_ip_test 
  Boost::unit_test_framework
  print_lib
)

install(TARGETS print RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT dmitry.syr@gmail.com)

include (CPack)

enable_testing()

add_test(test_printing_ip print_ip_test)
