#include <Eigen/Core>
#include <meshfix.h>
//
// meshfix_from_eigen_matrices(V,F,tin)
// meshfix_to_eigen_matrices(tin,W,G)

// Convert from libigl-style mesh as eigen matrices to MeshFix mesh class
//
// Inputs:
//   V  #V by 3 list of mesh vertex positions
//   F  #F by 3 list of triangle indices into rows of V
// Outputs:
//   tin  MeshFix mesh datastructure
void meshfix_from_eigen_matrices(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  T_MESH::Basic_TMesh & tin)
{
  T_MESH::TMesh::begin_progress();
  for(int v = 0;v<V.rows();v++)
  {
    tin.V.appendTail(tin.newVertex(V(v,0),V(v,1),V(v,2)));
  }
  int nv = tin.V.numels();
  T_MESH::ExtVertex ** var = (T_MESH::ExtVertex **)malloc(sizeof(T_MESH::ExtVertex *)*nv);
  {
    int i=0; 
    T_MESH::Vertex *v;
    T_MESH::Node *n;
    using namespace T_MESH;
    auto & V = tin.V;
    FOREACHVERTEX(v, n) var[i++] = new T_MESH::ExtVertex(v);
  }
  for(int f = 0;f<F.rows();f++)
  {
    if(!tin.CreateIndexedTriangle(var, F(f,0),F(f,1),F(f,2))) T_MESH::TMesh::warning("\nloadOFF: This shouldn't happen!!! Skipping triangle.\n");
  }
  tin.eulerUpdate();
  T_MESH::TMesh::end_progress();
  tin.closeLoadingSession(NULL, F.rows(), var, false);
}

// Convert from libigl-style mesh as eigen matrices to MeshFix mesh class
//
// Inputs:
//   tin  MeshFix mesh datastructure
// Outputs:
//   W  #W by 3 list of mesh vertex positions
//   G  #G by 3 list of triangle indices into rows of W
void meshfix_to_eigen_matrices(
  T_MESH::Basic_TMesh & tin,
  Eigen::MatrixXd & W,
  Eigen::MatrixXi & G)
{
  W.resize(tin.V.numels(),3);
  G.resize(tin.T.numels(),3);
  {
    int i=0; 
    T_MESH::Vertex *v;
    T_MESH::Node *n;
    using namespace T_MESH;
    auto & V = tin.V;
    auto & T = tin.T;
    FOREACHVERTEX(v, n) W.row(i++)<< TMESH_TO_FLOAT(v->x), TMESH_TO_FLOAT(v->y), TMESH_TO_FLOAT(v->z);
    coord * ocds = new coord[V.numels()];
    // TMesh savers slip index into x-coordinate then write back
    i=0; FOREACHVERTEX(v, n) ocds[i++] = v->x;
    i=0; FOREACHVERTEX(v, n) v->x = i++;
#define TVI1(a) (TMESH_TO_INT(((Triangle *)a->data)->v1()->x))
#define TVI2(a) (TMESH_TO_INT(((Triangle *)a->data)->v2()->x))
#define TVI3(a) (TMESH_TO_INT(((Triangle *)a->data)->v3()->x))
    i=0; FOREACHNODE(T, n) G.row(i++)<< TVI1(n),TVI2(n),TVI3(n);
    i=0; FOREACHVERTEX(v, n) v->x = ocds[i++];
    delete[] ocds;
  }
}
