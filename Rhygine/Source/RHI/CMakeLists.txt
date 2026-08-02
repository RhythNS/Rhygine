add_library(
	RhyRHI INTERFACE
	"RHIEnums.h"
	"RHIDescriptors.h"
	"ICommandBuffer.h"
	"ISwapchain.h"
	"IDevice.h"
)

target_include_directories (RhyRHI INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(
	RhyRHI
	INTERFACE RhyCore
)

function(RHY_COMPILE_SHADER OUT_BIN_PATH SOURCE_PATH SHADER_TYPE ENTRY_POINT)
    find_program(GLSLANG_VALIDATOR glslangValidator REQUIRED)

    string(SUBSTRING "${SHADER_TYPE}" 0 2 SHADER_STAGE_PREFIX)
    if(SHADER_STAGE_PREFIX STREQUAL "vs")
        set(GLSLANG_STAGE "vert")
    elseif(SHADER_STAGE_PREFIX STREQUAL "ps")
        set(GLSLANG_STAGE "frag")
    elseif(SHADER_STAGE_PREFIX STREQUAL "cs")
        set(GLSLANG_STAGE "comp")
    elseif(SHADER_STAGE_PREFIX STREQUAL "gs")
        set(GLSLANG_STAGE "geom")
    elseif(SHADER_STAGE_PREFIX STREQUAL "hs")
        set(GLSLANG_STAGE "tesc")
    elseif(SHADER_STAGE_PREFIX STREQUAL "ds")
        set(GLSLANG_STAGE "tese")
    else()
        message(FATAL_ERROR "Unsupported shader type: ${SHADER_TYPE}")
    endif()

    get_filename_component(SOURCE_FILENAME ${SOURCE_PATH} NAME_WE)
    set(OUTPUT_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Shaders")
    set(OUTPUT_PATH "${OUTPUT_DIR}/${SOURCE_FILENAME}.spv")

    add_custom_command(
        OUTPUT ${OUTPUT_PATH}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_DIR}
        COMMAND ${GLSLANG_VALIDATOR}
            -D
            -V
            -S ${GLSLANG_STAGE}
            -e ${ENTRY_POINT}
            -o ${OUTPUT_PATH}
            ${SOURCE_PATH}
        DEPENDS ${SOURCE_PATH}
        COMMENT "Compiling SPIR-V shader ${OUTPUT_PATH}"
    )

    set(${OUT_BIN_PATH} ${OUTPUT_PATH} PARENT_SCOPE)
endfunction()

if(ENABLE_VULKAN)
	find_package(Vulkan REQUIRED)

	target_link_libraries(RhyRHI INTERFACE Vulkan::Vulkan)
	target_link_libraries(RhyRHI INTERFACE VulkanMemoryAllocator)

	add_subdirectory("Vulkan")
endif()
