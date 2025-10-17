# OpenGLProject

## Installation instructions

Install CMake and vcpkg
Install dependencies with vcpkg (glad, imgiu, glfw3) `vcpkg install`

Build the project with msvc (replace `<vcpkg-root>` with the path to vcpkg) `cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=<vcpkg-root>/scripts/buildsystems/vcpkg.cmake`

Optionally add the target triplet at the end of the build command. I use `-DVCPKG_TARGET_TRIPLET=x64-windows` for my system.
