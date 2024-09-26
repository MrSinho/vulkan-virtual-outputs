cmake_minimum_required(VERSION 3.5)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)



if (NOT DEFINED VVO_BINARIES_DIR)
message(FATAL_ERROR "vvo cmake error: missing VVO_BINARIES_DIR")
endif()

if (NOT DEFINED VVO_ROOT_DIR)
message(FATAL_ERROR "vvo cmake error: missing VVO_ROOT_DIR")
else()
set(VVO_INCLUDE_DIR ${VVO_ROOT_DIR}/vvo/include)
endif()



function(build_vvo)


if (NOT TARGET shvulkan)

set(SH_VULKAN_ROOT_DIR     ${VVO_ROOT_DIR}/externals/shvulkan)
set(SH_VULKAN_BINARIES_DIR ${VVO_BINARIES_DIR})

include(${VVO_ROOT_DIR}/externals/shvulkan/shvulkan/shvulkan.cmake)
build_shvulkan()

endif()


add_library(vvo 
    ${VVO_ROOT_DIR}/vvo/src/vvo.c
    ${VVO_ROOT_DIR}/externals/mongoose/mongoose.c
)

target_include_directories(vvo PUBLIC 
    ${VVO_INCLUDE_DIR}
    ${VVO_ROOT_DIR}/externals
    ${SH_VULKAN_ROOT_DIR}/shvulkan/include
)

target_link_libraries(vvo PUBLIC
    shvulkan
)

set_target_properties(vvo PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${VVO_BINARIES_DIR}
)

endfunction()