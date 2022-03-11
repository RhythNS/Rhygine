add_library(
	SoloudSound
	Include/Sound/SoloudModule.h
	Include/Sound/SoloudComponent.h
	Include/Sound/SoundListener.h
	Source/Sound/SoloudModule.cpp
	Source/Sound/SoloudComponent.cpp
	Source/Sound/SoundListener.cpp
)

target_include_directories (SoloudSound PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Include")
target_include_directories (SoloudSound PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source")

target_link_libraries (
	SoloudSound
	RhyCore
	soloud
)

# There is probably a better way to this.
get_target_property(SOLOUD_INCLUDES soloud INCLUDE_DIRECTORIES)
foreach(dir ${SOLOUD_INCLUDES})
	target_include_directories (SoloudSound PUBLIC ${dir})
endforeach()
