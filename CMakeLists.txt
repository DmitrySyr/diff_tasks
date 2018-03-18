cmake_minimum_required(VERSION 3.2)

project(cpp01_2)

#find_package(Boost COMPONENTS unit_test_framework REQUIRED)

# The version number.
set (PROJECT_VERSION_PATCH "1.0")
 
# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/version.h.in"
  "${PROJECT_BINARY_DIR}/version.h"
  )
 
# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")

add_executable(cpp01_2_cli HelloWorld.cpp stdafx.cpp)

add_library(cpp01_2 lib.cpp)

#add_executable(test_main_cli test_main.cpp)

set_target_properties(cpp01_2_cli PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
  COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)

#set_target_properties(test_main_cli PROPERTIES
#  COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
#  INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
#)

target_link_libraries(cpp01_2_cli cpp01_2)

#target_link_libraries(test_main_cli
#  ${Boost_LIBRARIES}
#  cpp01_2
#)

# install(TARGETS cpp01_2_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

# set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
# set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
# set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

# #set(CPACK_PACKAGE_CONTACT vvz.otus@gmail.com)

# include (CPack)

#enable_testing()

#add_test(test_version_valid test_main_cli)

set (CPACK_PROJECT_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
include (CPack)