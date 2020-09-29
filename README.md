Rhygine

## Build
To build the project you need [vcpkg](https://docs.microsoft.com/en-us/cpp/build/vcpkg?view=vs-2019).

    git clone https://github.com/Microsoft/vcpkg.git
    cd vcpkg
    bootstrap-vcpkg.bin
    vcpkg integrate install
    vcpkg install assimp:x64-windows
    vcpkg install stb:x64-windows
