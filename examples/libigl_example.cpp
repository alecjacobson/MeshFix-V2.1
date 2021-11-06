// clang++ -std=c++11 -I ~/Repos/libigl/include/ -I ~/Repos/libigl/external/eigen/ -I ~/Repos/MeshFix-V2.1/include/  -I ~/Repos/MeshFix-V2.1/include/TMesh/  -I ~/Repos/MeshFix-V2.1/include/Kernel/ libigl_example.cpp -L ~/Repos/MeshFix-V2.1/build/ -lMeshFix -o libigl_example
#include <meshfix.h>
#include "meshfix_eigen.h"

#include <igl/read_triangle_mesh.h>
#include <igl/write_triangle_mesh.h>
#include <iostream>

int main(int argc, char * argv[])
{
  // Load in libigl's (V,F) format
  Eigen::MatrixXd V,W;
  Eigen::MatrixXi F,G;
  if(argc <= 2)
  {
    std::cout<<R"(Usage:
    ./meshfix-libigl [input](.obj|.ply|.stl|.off) [output](.obj|.ply|.stl|.off)
)";
    return EXIT_FAILURE;
  }
  igl::read_triangle_mesh(argv[1],V,F);

  /////////////////////////////////////////////////////////////////////////
  // Convert to meshfix type, call meshfix, convert back from meshfix type
  T_MESH::TMesh::init(); // This is mandatory // Alec: or is it?
  T_MESH::Basic_TMesh tin;
  meshfix_from_eigen_matrices(V,F,tin);
  meshfix(false,tin);
  meshfix_to_eigen_matrices(tin,W,G);
  /////////////////////////////////////////////////////////////////////////

  // Write to OBJ
  igl::write_triangle_mesh(argv[2],W,G);
}
