cmake_minimum_required(VERSION 3.5)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)



if (NOT DEFINED VVO_BINARIES_DIR)
message(FATAL_ERROR "vvo cmake error: missing VVO_BINARIES_DIR")
endif()

if (NOT DEFINED VVO_ROOT_DIR)
message(FATAL_ERROR "vvo cmake error: missing VVO_ROOT_DIR")
endif()



function(build_vvo)

find_package(Vulkan REQUIRED)

message(STATUS "vvo message: found Vulkan")
message(STATUS "vvo message: Vulkan_INCLUDE_DIR: ${Vulkan_INCLUDE_DIR}")
message(STATUS "vvo message: Vulkan_LIBRARY:     ${Vulkan_LIBRARY}")

include(ExternalProject)

#set(ENABLE_STATIC ON)
#ExternalProject_Add(libjpeg-turbo 
#    SOURCE_DIR ${VVO_ROOT_DIR}/externals/libjpeg-turbo
#)


add_library(vvo 
    vvo/src/vvo.c
    ${VVO_ROOT_DIR}/externals/mongoose/mongoose.c
)

target_include_directories(vvo PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/vvo/include
    ${Vulkan_INCLUDE_DIR}
    ${VVO_ROOT_DIR}/externals
)

target_link_libraries(vvo PUBLIC
    ${Vulkan_LIBRARY}
)

if (WIN32)
set_target_properties(vvo PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY      ${VVO_BINARIES_DIR}/windows
)
else()
set_target_properties(vvo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY      ${VVO_BINARIES_DIR}/linux
)
endif(WIN32)

endfunction()