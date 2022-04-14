
#ifndef CAMERA_C
#define CAMERA_C 1

#include "../libs/cglm/cglm.h"

struct Camera {
  mat4 perspectiveMatrix;

  mat4 modelViewMatrix;
  
  mat4 projectionMatrix;

  vec4 viewPort;

  vec3 position;
  vec3 rotationEuler;

  mat4 rotationMatrix;
};
#define CameraP struct Camera *

void Camera_XYZ (CameraP camera, float x, float y, float z) {
  // camera->position
  camera->position[0] = x;
  camera->position[1] = y;
  camera->position[2] = z;
}
void Camera_rotateEulerXYZ (CameraP camera, float x, float y, float z) {
  Camera_rotateEuler(camera, (vec3) {x, y ,z});
}
void Camera_rotateEuler (CameraP camera, vec3 euler) {
  glm_euler(euler, camera->rotationMatrix);
}

void Camera_point_project (CameraP camera, vec3 point, vec3 dest) {
  glm_project(point, camera->projectionMatrix, camera->viewPort, dest);
}

void Camera_update (CameraP camera) {
  //clear transform (TRS) matrix
  glm_mat4_identity(camera->modelViewMatrix);
  //clear rotation matrix
  glm_mat4_identity(camera->rotationMatrix);

  //move to camera's position
  glm_translate(camera->modelViewMatrix, camera->position);
  //rotate to camera
  glm_euler(camera->rotationEuler, camera->rotationMatrix);

  //combine rotation onto translation
  glm_mul(camera->modelViewMatrix, camera->rotationMatrix, camera->modelViewMatrix);

  //combine perspective with translation/rotation onto projection matrix
  glm_mul(camera->perspectiveMatrix, camera->modelViewMatrix, camera->projectionMatrix);
}

#endif
