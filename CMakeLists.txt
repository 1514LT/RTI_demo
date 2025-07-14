###################################global#######################################
cmake_minimum_required(VERSION 3.14)
#project(test LANGUAGES CXX)
project(test LANGUAGES C)
#project(test LANGUAGES C CXX)

#set(CMAKE_CXX_STANDARD      17                )
#set(CMAKE_CXX_STANDARD_REQUIRED    ON         )
#set(CMAKE_CXX_EXTENSIONS    OFF               )
set(CMAKE_C_STANDARD      11                 )
set(CMAKE_C_STANDARD_REQUIRED    ON          )
set(CMAKE_C_EXTENSIONS      OFF              )

set(CMAKE_INSTALL_PREFIX    "${CMAKE_BINARY_DIR}"               )
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  "${CMAKE_BINARY_DIR}/lib"   )
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  "${CMAKE_BINARY_DIR}/lib"   )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  "${CMAKE_BINARY_DIR}/bin"   )
set(CMAKE_MODULE_PATH      "${PROJECT_SOURCE_DIR}/cmake"        )
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE    "RelWithDebInfo"                      )
endif()
include(enable_warning)

include_directories   (
  "${CMAKE_SOURCE_DIR}/thirdparty/rti_dds_micro/include"
)
link_directories      (
  "${CMAKE_SOURCE_DIR}/thirdparty/rti_dds_micro/lib"
)
add_definitions       ()
###################################global#######################################

add_subdirectory(src)
