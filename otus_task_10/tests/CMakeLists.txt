cmake_minimum_required(VERSION 3.2)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
find_package(Threads)

function(tests name)
  add_executable (${name} ${name}.cpp)

  set_target_properties(${name} PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
  COMPILE_OPTIONS -Wpedantic -Wall -Wextra
  COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK)
  SET( CMAKE_EXE_LINKER_FLAGS  "-lstdc++fs" )

  target_include_directories(${name} PRIVATE ${PROJECT_SOURCE_DIR})
  target_link_libraries(${name} ${CMAKE_THREAD_LIBS_INIT})
  target_link_libraries (${name} Boost::unit_test_framework lib)
  add_test(${name} ${name})
endfunction(tests)

tests(test_output)
tests(test_principal_task)
tests(test_wrong_input)
tests(test_internal_structures)
tests(test_output_statistics)
