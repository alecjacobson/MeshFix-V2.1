Example using meshfix as a library in an eigen/libigl setting.

Compile, for example, with:

    clang++ -std=c++11 -I ~/Repos/libigl/include/ -I ~/Repos/libigl/external/eigen/ -I ~/Repos/MeshFix-V2.1/include/  -I ~/Repos/MeshFix-V2.1/include/TMesh/  -I ~/Repos/MeshFix-V2.1/include/Kernel/ libigl_example.cpp -L ~/Repos/MeshFix-V2.1/build/ -lMeshFix -o libigl_example

Run with

    libigl_example input.obj output.obj
