cmake_minimum_required(VERSION 3.5)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)



project(vvo C)



set(VVO_VERSION  "pre-release")
set(VVO_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})



option(VVO_BINARIES_DIR  CACHE OFF)
option(VVO_BUILD_EXAMPLE CACHE OFF)



if (NOT VVO_BINARIES_DIR)

if (WIN32)
set(VVO_BINARIES_DIR ${VVO_ROOT_DIR}/bin/windows)
elseif(UNIX)
set(VVO_BINARIES_DIR ${VVO_ROOT_DIR}/bin/linux)
endif(WIN32)

endif()



message(STATUS "VVO_VERSION       : ${VVO_VERSION}")
message(STATUS "VVO_ROOT_DIR      : ${VVO_ROOT_DIR}")
message(STATUS "VVO_BINARIES_DIR  : ${VVO_BINARIES_DIR}")
message(STATUS "VVO_BUILD_EXAMPLE : ${VVO_BUILD_EXAMPLE}")



include(vvo/vvo.cmake)
include(docs/vvo-docs.cmake)
include(example/vvo-example.cmake)



function(main)

build_vvo()
build_vvo_docs()

if (VVO_BUILD_EXAMPLE)
build_vvo_example()
endif(VVO_BUILD_EXAMPLE)

endfunction()



main()
