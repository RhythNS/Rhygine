cmake_minimum_required (VERSION 3.15)

project ("Rhygine")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set executable path and related directory variables
set (RHY_EXECUTABLE_DIR ${CMAKE_BINARY_DIR}/Exe)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${RHY_EXECUTABLE_DIR})
set (RHY_ASSETS_DIR ${RHY_EXECUTABLE_DIR}/Assets)
set (RHY_SHADER_FOLDER ${RHY_ASSETS_DIR}/Shader)

# Download and update submodules
#find_package(Git QUIET)
#if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
# Update submodules as needed
#    option(GIT_SUBMODULE "Check submodules during build" ON)
#    if(GIT_SUBMODULE)
#        message(STATUS "Submodule update")
#        execute_process(
#            COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
#            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#            RESULT_VARIABLE GIT_SUBMOD_RESULT)
#        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
#            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
#        endif()
#    endif()
#endif()

# find fxc for shader compiling
find_program(FXC fxc DOC "fx compiler")
if(NOT FXC)
message(SEND_ERROR "Cannot find fxc.")
endif(NOT FXC)

# Macros for shader compiliation taken from: https://stackoverflow.com/questions/60878233/cmake-custom-post-build-step-command-always-executes

# Generate shader output
function(RHY_COMPILE_SHADER VAR_BINARY_PATH SOURCE_PATH SHADER_TYPE ENTRY_POINT)
    get_filename_component(SOURCE_FILENAME ${SOURCE_PATH} NAME_WE)
    get_filename_component(SOURCE_PATH "${SOURCE_PATH}" ABSOLUTE)
    set(BINARY_PATH "${RHY_SHADER_FOLDER}/${SOURCE_FILENAME}.cso")
    add_custom_command(OUTPUT ${BINARY_PATH}
        COMMENT "Generating ${BINARY_PATH}..."
        COMMAND ${FXC} /T \"${SHADER_TYPE}\" /E\"${ENTRY_POINT}\" /Od /Zi /Fo \"${BINARY_PATH}\" \"${SOURCE_PATH}\"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS "${SOURCE_PATH}"
    )
    set(${VAR_BINARY_PATH} "${BINARY_PATH}" PARENT_SCOPE)
endfunction()

# Generate vs shader output
macro(RHY_COMPILE_VS VAR_BINARY_PATH SOURCE_PATH)
    RHY_COMPILE_SHADER(${VAR_BINARY_PATH} ${SOURCE_PATH} "vs_5_0" "main")
endmacro()

# Generate ps shader output
macro(RHY_COMPILE_PS VAR_BINARY_PATH SOURCE_PATH)
    RHY_COMPILE_SHADER(${VAR_BINARY_PATH} ${SOURCE_PATH} "ps_5_0" "main")
endmacro()


# Copy entire folder to the assets folder
macro (RHY_ADD_FOLDER_TO_ASSETS VAR_TARGET VAR_TO_COPY_FOLDER)
get_filename_component (VAR_NAME ${VAR_TO_COPY_FOLDER} NAME_WE)
add_custom_command(
	TARGET ${VAR_TARGET} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${VAR_TO_COPY_FOLDER} ${RHY_ASSETS_DIR}/${VAR_NAME}
)
endmacro ()

# Copy entire assets folder to the executable assets folder
macro (RHY_COPY_ASSETS VAR_TARGET VAR_SOURCE_DIR)
add_custom_command(
	TARGET ${VAR_TARGET} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${VAR_SOURCE_DIR}/Assets ${RHY_ASSETS_DIR}
)
endmacro ()

# Set bullet library
set(USE_BULLET_PHYSICS ON CACHE BOOL "Adds the bullet physics engine via the physics module")

if (USE_BULLET_PHYSICS)
set (BULLET_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/Libs/bullet3/src/)
set (BULLET_LIBRARY BulletDynamics BulletCollision LinearMath)
endif ()


set(USE_REACT_PHYSICS ON CACHE BOOL "Adds the react physics engine via the physics module")
set(USE_SOLOUD ON CACHE BOOL "Adds the soloud sound library")

set(USE_MODEL3D ON CACHE BOOL "Adds libraries for 3D models via the Model3D module")
if (USE_MODEL3D)
add_definitions(-DUSE_MODEL3D)
endif ()

set(USE_IMGUI ON CACHE BOOL "Adds imgui via the ImGUI module")
set(USE_COMMON_COMPOENTS ON CACHE BOOL "Adds default components and shaders")

# Add external libs
add_subdirectory ("Libs")

# Include sub-projects.

set(RHY_USE_EXAMPLES ON CACHE BOOL "Adds examples")
if (RHY_USE_EXAMPLES)
add_subdirectory ("Examples")
endif ()

add_subdirectory ("Source")
