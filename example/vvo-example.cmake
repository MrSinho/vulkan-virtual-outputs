cmake_minimum_required(VERSION 3.5)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)



if (NOT DEFINED VVO_BINARIES_DIR)
message(FATAL_ERROR "vvo cmake error: missing VVO_BINARIES_DIR")
endif()

if (NOT DEFINED VVO_ROOT_DIR)
message(FATAL_ERROR "vvo cmake error: missing VVO_ROOT_DIR")
endif()



function(build_vvo_example)

add_executable(vvo-example ${VVO_ROOT_DIR}/example/src/vvo-example.c)

target_link_libraries(vvo-example PUBLIC vvo)

set_target_properties(vvo-example vvo PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY      ${VVO_BINARIES_DIR}
    RUNTIME_OUTPUT_DIRECTORY      ${VVO_BINARIES_DIR}
    VS_DEBUGGER_WORKING_DIRECTORY ${VVO_BINARIES_DIR}
)

endfunction()