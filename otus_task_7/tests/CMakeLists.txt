cmake_minimum_required(VERSION 3.2)

function(tests name)
  add_executable (${name} ${name}.cpp)
  target_compile_definitions(${name} PRIVATE BOOST_TEST_DYN_LINK BOOST_LOG_DYN_LINK)
  target_include_directories(${name} PRIVATE ${PROJECT_SOURCE_DIR})
  target_link_libraries (${name} ${Boost_LIBRARIES})
  add_test(${name} ${CMAKE_BINARY_DIR}/bin/${name})
endfunction(tests)

tests(test_output)
tests(test_principal_task)
tests(test_wrong_input)
tests(test_internal_structures)
