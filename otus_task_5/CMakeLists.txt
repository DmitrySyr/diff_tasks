cmake_minimum_required(VERSION 3.2)

if($ENV{TRAVIS_BUILD_NUMBER})
    set(VERSION_BUILD $ENV{TRAVIS_BUILD_NUMBER})
else()
    set(VERSION_BUILD 0)
endif()

project(VectorPaint VERSION 0.0.${VERSION_BUILD} LANGUAGES CXX)


#set (PROJECT_VERSION_PATCH "1.0")

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/version.h.in"
  "${PROJECT_BINARY_DIR}/version.h"
  )

#set(SOURCE_PATH "${PROJECT_SOURCE_DIR}")

add_executable(paint main.cpp )

add_library(paint_lib Application.h Drawing.h Drawing.cpp Figures.h Figures.cpp GUIForms.h GUIForms.cpp GUIImpl.h GUIImpl.cpp)

target_link_libraries(paint paint_lib)

set_target_properties(paint paint_lib PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
  LINKER_LANGUAGE CXX
  COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)


install(TARGETS paint RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT dmitry.syr@gmail.com)

include (CPack)


