# CMakeList.txt : CMake project for FilesSize, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (FilesSize "main.cpp" )

set_property(TARGET FilesSize PROPERTY CXX_STANDARD 17)
# TODO: Add tests and install targets if needed.
