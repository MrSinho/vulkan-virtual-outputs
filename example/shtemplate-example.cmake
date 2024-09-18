cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)



if (NOT DEFINED SH_TEMPLATE_BINARIES_DIR)
message(FATAL_ERROR "shtemplate cmake error: missing SH_TEMPLATE_BINARIES_DIR")
endif()

if (NOT DEFINED SH_TEMPLATE_ROOT_DIR)
message(FATAL_ERROR "shtemplate cmake error: missing SH_TEMPLATE_ROOT_DIR")
endif()



function(build_shtemplate_example)

add_executable(shtemplate-example example/src/shtemplate-example.c)

target_link_libraries(shtemplate-example PUBLIC shtemplate)

if (WIN32)
set_target_properties(shtemplate-example shtemplate PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY      ${SH_TEMPLATE_BINARIES_DIR}/windows
    RUNTIME_OUTPUT_DIRECTORY      ${SH_TEMPLATE_BINARIES_DIR}/windows
    VS_DEBUGGER_WORKING_DIRECTORY ${SH_TEMPLATE_BINARIES_DIR}/windows
)
else()
set_target_properties(shtemplate-example shtemplate PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY      ${SH_TEMPLATE_BINARIES_DIR}/linux
    RUNTIME_OUTPUT_DIRECTORY      ${SH_TEMPLATE_BINARIES_DIR}/linux
    VS_DEBUGGER_WORKING_DIRECTORY ${SH_TEMPLATE_BINARIES_DIR}/linux
)
endif(WIN32)

endfunction()