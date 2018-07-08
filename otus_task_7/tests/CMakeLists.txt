cmake_minimum_required(VERSION 3.2)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

function(tests name)
  add_executable (${name} ${name}.cpp)

  set_target_properties(${name} PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
  COMPILE_OPTIONS -Wpedantic -Wall -Wextra
  COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK)

  target_include_directories(${name} PRIVATE ${PROJECT_SOURCE_DIR}/otus_task_7)  
  target_link_libraries (${name} Boost::unit_test_framework lib)
  add_test(${name} ${CMAKE_BINARY_DIR}/otus_task_7/tests/${name})
endfunction(tests)

tests(test_output)
tests(test_principal_task)
tests(test_wrong_input)
tests(test_internal_structures)
