
#ifndef MESH_C
#define MESH_C

#include "../libs/cglm/cglm.h"
#include "../3d/transform.c"
#include "../linkedlist.c"

struct Mesh {
  float * verticies;
  int vertexCount;

  struct Transform transform;
};
#define MeshP struct Mesh *

struct MeshBuilder {
  llnp verticies;

};
#define MeshBuilderP struct MeshBuilder *

MeshBuilderP MeshBuilder_create () {
  MeshBuilderP result = (MeshBuilderP) malloc(sizeof(struct MeshBuilder));
  // result->verticies = lln_create();
  return result;
}
void MeshBuilder_tri (MeshBuilderP mb, vec3 a, vec3 b, vec3 c) {
  if (mb->verticies == NULL) {
    
     lln_create();
    // lln_add(mb->verticies, );
  }
}

void MeshBuilder_build (MeshBuilderP mb, MeshP m) {
  
}

#endif
