cmake_minimum_required(VERSION 3.2)

find_package(Boost REQUIRED)
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
  target_link_libraries(${name} ${CMAKE_THREAD_LIBS_INIT} stdc++fs)
  target_link_libraries (${name} Boost::unit_test_framework ${Boost_LIBRARIES} bulk_server_lib)
  add_test(${name} ${name})
endfunction(tests)

tests(test_bulk)
tests(HW11_test_output)
tests(test_wrong)
