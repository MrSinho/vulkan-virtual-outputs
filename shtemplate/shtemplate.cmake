cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)



if (NOT DEFINED SH_TEMPLATE_BINARIES_DIR)
message(FATAL_ERROR "shtemplate cmake error: missing SH_TEMPLATE_BINARIES_DIR")
endif()

if (NOT DEFINED SH_TEMPLATE_ROOT_DIR)
message(FATAL_ERROR "shtemplate cmake error: missing SH_TEMPLATE_ROOT_DIR")
endif()



function(build_shtemplate)

add_library(shtemplate shtemplate/src/shtemplate.c)

target_include_directories(shtemplate PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/shtemplate/include
)

if (WIN32)
set_target_properties(shtemplate PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY      ${SH_TEMPLATE_BINARIES_DIR}/windows
)
else()
set_target_properties(shtemplate PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY      ${SH_TEMPLATE_BINARIES_DIR}/linux
)
endif(WIN32)

endfunction()