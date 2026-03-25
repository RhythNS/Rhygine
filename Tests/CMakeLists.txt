project("Tests")

add_subdirectory("RandomTests")
if(WIN32)
    add_subdirectory(WinTests)
endif()
if(UNIX AND NOT APPLE)
    add_subdirectory(LinuxTests)
endif()