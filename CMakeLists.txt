cmake_minimum_required(VERSION 3.20)
project(CX-AE-Plugins VERSION 1.0.0 LANGUAGES CXX)

# ============================================================================
# Project Configuration
# ============================================================================

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output directory for compiled plugins
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)

# ============================================================================
# After Effects SDK Path Configuration
# ============================================================================

# Default SDK path (can be overridden via -DAE_SDK_PATH=...)
if(NOT DEFINED AE_SDK_PATH)
    set(AE_SDK_PATH "C:/Users/tammc/Documents/0_Develop/ae25.6_61.64bit.AfterEffectsSDK/Examples"
        CACHE PATH "Path to After Effects SDK Examples directory")
endif()

# Verify SDK path exists
if(NOT EXISTS "${AE_SDK_PATH}")
    message(FATAL_ERROR
        "After Effects SDK not found at: ${AE_SDK_PATH}\n"
        "Please set AE_SDK_PATH to your SDK installation:\n"
        "  cmake -DAE_SDK_PATH=/path/to/ae_sdk/Examples ..")
endif()

message(STATUS "Using After Effects SDK: ${AE_SDK_PATH}")

# ============================================================================
# SDK Include Directories
# ============================================================================

set(AE_SDK_INCLUDES
    ${AE_SDK_PATH}/Headers
    ${AE_SDK_PATH}/Headers/SP
    ${AE_SDK_PATH}/Headers/Win
    ${AE_SDK_PATH}/Resources
    ${AE_SDK_PATH}/Util
)

# ============================================================================
# Common Settings for All Plugins
# ============================================================================

# Shared include directory
set(CX_SHARED_INCLUDE ${CMAKE_SOURCE_DIR}/shared)

# Common preprocessor definitions
add_compile_definitions(
    MSWindows
    WIN32
    _WINDOWS
)

# Common compiler flags
if(MSVC)
    add_compile_options(
        /W3                 # Warning level 3
        /MP                 # Multi-processor compilation
        /permissive-       # Standards conformance
    )

    # Runtime library
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

# ============================================================================
# PiPL Resource Compilation Function
# ============================================================================

function(add_pipl_resource TARGET_NAME PIPL_SOURCE)
    get_filename_component(PIPL_NAME ${PIPL_SOURCE} NAME_WE)
    get_filename_component(PIPL_DIR ${PIPL_SOURCE} DIRECTORY)

    set(PIPL_RR ${CMAKE_CURRENT_BINARY_DIR}/${PIPL_NAME}.rr)
    set(PIPL_RRC ${CMAKE_CURRENT_BINARY_DIR}/${PIPL_NAME}.rrc)
    set(PIPL_RC ${CMAKE_CURRENT_BINARY_DIR}/${PIPL_NAME}.rc)

    # Step 1: Preprocess .r file
    add_custom_command(
        OUTPUT ${PIPL_RR}
        COMMAND ${CMAKE_CXX_COMPILER} /I "${AE_SDK_PATH}/Headers" /EP "${PIPL_SOURCE}" > "${PIPL_RR}"
        DEPENDS ${PIPL_SOURCE}
        COMMENT "Preprocessing PiPL resource: ${PIPL_SOURCE}"
        VERBATIM
    )

    # Step 2: Run PiPLTool
    add_custom_command(
        OUTPUT ${PIPL_RRC}
        COMMAND "${AE_SDK_PATH}/Resources/PiPLTool.exe" "${PIPL_RR}" "${PIPL_RRC}"
        DEPENDS ${PIPL_RR}
        COMMENT "Running PiPLTool on ${PIPL_NAME}.rr"
        VERBATIM
    )

    # Step 3: Preprocess .rrc to generate .rc
    add_custom_command(
        OUTPUT ${PIPL_RC}
        COMMAND ${CMAKE_CXX_COMPILER} /D "MSWindows" /EP "${PIPL_RRC}" > "${PIPL_RC}"
        DEPENDS ${PIPL_RRC}
        COMMENT "Generating RC file: ${PIPL_NAME}.rc"
        VERBATIM
    )

    # Add RC file to target sources
    target_sources(${TARGET_NAME} PRIVATE ${PIPL_RC})

    # Enable RC compilation
    set_source_files_properties(${PIPL_RC} PROPERTIES LANGUAGE RC)
endfunction()

# ============================================================================
# Plugin Creation Function
# ============================================================================

function(add_ae_plugin PLUGIN_NAME PLUGIN_DIR)
    # Source files
    set(PLUGIN_SOURCES
        ${PLUGIN_DIR}/${PLUGIN_NAME}.cpp
        ${AE_SDK_PATH}/Util/Smart_Utils.cpp
    )

    set(PLUGIN_HEADERS
        ${PLUGIN_DIR}/${PLUGIN_NAME}.h
        ${CX_SHARED_INCLUDE}/CXCommon.h
    )

    # Create shared library (.aex is actually a DLL)
    add_library(${PLUGIN_NAME} MODULE ${PLUGIN_SOURCES} ${PLUGIN_HEADERS})

    # Set output name and extension
    set_target_properties(${PLUGIN_NAME} PROPERTIES
        OUTPUT_NAME ${PLUGIN_NAME}
        SUFFIX ".aex"
        PREFIX ""
    )

    # Include directories
    target_include_directories(${PLUGIN_NAME} PRIVATE
        ${CX_SHARED_INCLUDE}
        ${PLUGIN_DIR}
        ${AE_SDK_INCLUDES}
    )

    # Link options
    if(MSVC)
        target_link_options(${PLUGIN_NAME} PRIVATE
            /SUBSYSTEM:WINDOWS
            /MACHINE:X64
        )
    endif()

    # Add PiPL resource
    set(PIPL_FILE ${PLUGIN_DIR}/${PLUGIN_NAME}PiPL.r)
    if(EXISTS ${PIPL_FILE})
        add_pipl_resource(${PLUGIN_NAME} ${PIPL_FILE})
    else()
        message(WARNING "PiPL file not found: ${PIPL_FILE}")
    endif()

    # Install target
    install(TARGETS ${PLUGIN_NAME}
        LIBRARY DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        RUNTIME DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    )
endfunction()

# ============================================================================
# Add Plugins
# ============================================================================

add_ae_plugin(cx_ColorLines ${CMAKE_SOURCE_DIR}/plugins/cx_ColorLines)

# Add more plugins here as they are developed:
# add_ae_plugin(cx_NewPlugin ${CMAKE_SOURCE_DIR}/plugins/cx_NewPlugin)

# ============================================================================
# Summary
# ============================================================================

message(STATUS "")
message(STATUS "CX-AE-Plugins Configuration Summary:")
message(STATUS "  Build type:        ${CMAKE_BUILD_TYPE}")
message(STATUS "  Output directory:  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "  SDK path:          ${AE_SDK_PATH}")
message(STATUS "")
