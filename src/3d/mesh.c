
#ifndef MESH_C
#define MESH_C

struct Mesh {
  float * verticies;
  int vertexCount;

  struct Transform transform;
};
#define MeshP struct Mesh *

#endif
