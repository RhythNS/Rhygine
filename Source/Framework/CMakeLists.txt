add_subdirectory ("Core")

if (USE_BULLET_PHYSICS)
add_subdirectory ("BulletPhysics")
endif ()

if (USE_REACT_PHYSICS)
add_subdirectory ("ReactPhysics")
endif ()

if (USE_COMMON_COMPOENTS)
add_subdirectory ("CommonComponents")
endif ()

if (USE_IMGUI)
add_subdirectory ("ImGui")
endif ()

if (USE_MODEL3D)
add_subdirectory ("Model3D")
endif ()

if (USE_SOLOUD)
add_subdirectory ("soloud")
endif ()
