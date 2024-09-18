cmake_minimum_required(VERSION 3.13)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)



function(build_shtemplate_docs)



if (NOT DEFINED SH_TEMPLATE_BINARIES_DIR)
message(FATAL_ERROR "shtemplate cmake error: missing SH_TEMPLATE_BINARIES_DIR")
endif()

if (NOT DEFINED SH_TEMPLATE_ROOT_DIR)
message(FATAL_ERROR "shtemplate cmake error: missing SH_TEMPLATE_ROOT_DIR")
endif()




find_package(Doxygen)


if (DOXYGEN_FOUND)

    set(SH_TEMPLATE_DOXYFILESRC ${SH_TEMPLATE_ROOT_DIR}/docs/Doxyfile.in)
    set(SH_TEMPLATE_DOXYFILEDST ${SH_TEMPLATE_ROOT_DIR}/docs/Doxyfile)

    message(STATUS "Configuring " ${SH_TEMPLATE_DOXYFILEDST} " from " ${SH_TEMPLATE_DOXYFILESRC})

    configure_file(
        ${SH_TEMPLATE_DOXYFILESRC} 
        ${SH_TEMPLATE_DOXYFILEDST}
    )

    message(STATUS "Creating custom docs command: " ${DOXYGEN_EXECUTABLE} " " ${SH_TEMPLATE_DOXYFILEDST})

    add_custom_target(docs-shtemplate
        COMMAND ${DOXYGEN_EXECUTABLE} ${SH_TEMPLATE_DOXYFILEDST}
        WORKING_DIRECTORY ${SH_TEMPLATE_ROOT_DIR}
        COMMENT "Generating HTML documentation"
        VERBATIM
    )

endif(DOXYGEN_FOUND)

endfunction()