cmake_minimum_required(VERSION 3.5)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)



function(build_vvo_docs)



if (NOT DEFINED VVO_BINARIES_DIR)
message(FATAL_ERROR "vvo cmake error: missing VVO_BINARIES_DIR")
endif()

if (NOT DEFINED VVO_ROOT_DIR)
message(FATAL_ERROR "vvo cmake error: missing VVO_ROOT_DIR")
endif()




find_package(Doxygen)


if (DOXYGEN_FOUND)

    set(VVO_DOXYFILESRC ${VVO_ROOT_DIR}/docs/Doxyfile.in)
    set(VVO_DOXYFILEDST ${VVO_ROOT_DIR}/docs/Doxyfile)

    message(STATUS "Configuring " ${VVO_DOXYFILEDST} " from " ${VVO_DOXYFILESRC})

    configure_file(
        ${VVO_DOXYFILESRC} 
        ${VVO_DOXYFILEDST}
    )

    message(STATUS "Creating custom docs command: " ${DOXYGEN_EXECUTABLE} " " ${VVO_DOXYFILEDST})

    add_custom_target(docs-vvo
        COMMAND ${DOXYGEN_EXECUTABLE} ${VVO_DOXYFILEDST}
        WORKING_DIRECTORY ${VVO_ROOT_DIR}
        COMMENT "Generating HTML documentation"
        VERBATIM
    )

endif(DOXYGEN_FOUND)

endfunction()