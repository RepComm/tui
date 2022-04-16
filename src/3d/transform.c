
#ifndef TRANSFORM_C
#define TRANSFORM_C 1

#include "../libs/cglm/cglm.h"

struct Transform {
  vec3 position;
  vec3 rotation;
  mat4 modelMatrix;
  mat4 rotationMatrix;
};
#define TransformP struct Transform *

void Transform_update (TransformP t) {
  glm_mat4_identity(t->modelMatrix);
  glm_translate(t->modelMatrix, t->position);
  glm_euler(t->rotation, t->rotationMatrix);
  glm_mul(t->rotationMatrix, t->modelMatrix, t->modelMatrix);
}

#endif
