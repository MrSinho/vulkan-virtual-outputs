cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)



project(shtemplate C)



set(SH_TEMPLATE_VERSION  "pre-release")
set(SH_TEMPLATE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})



option(SH_TEMPLATE_BINARIES_DIR  CACHE OFF)
option(SH_TEMPLATE_BUILD_EXAMPLE CACHE OFF)

if (NOT SH_TEMPLATE_BINARIES_DIR)
set(SH_TEMPLATE_BINARIES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)
endif()



message(STATUS "SH_TEMPLATE_VERSION       : ${SH_TEMPLATE_VERSION}")
message(STATUS "SH_TEMPLATE_ROOT_DIR      : ${SH_TEMPLATE_ROOT_DIR}")
message(STATUS "SH_TEMPLATE_BINARIES_DIR  : ${SH_TEMPLATE_BINARIES_DIR}")
message(STATUS "SH_TEMPLATE_BUILD_EXAMPLE : ${SH_TEMPLATE_BUILD_EXAMPLE}")



include(shtemplate/shtemplate.cmake)
include(docs/shtemplate-docs.cmake)
include(example/shtemplate-example.cmake)




function(main)

build_shtemplate()
build_shtemplate_docs()

if (SH_TEMPLATE_BUILD_EXAMPLE)
    build_shtemplate_example()
endif(SH_TEMPLATE_BUILD_EXAMPLE)

endfunction()



main()
