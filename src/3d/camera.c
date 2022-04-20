
#ifndef CAMERA_C
#define CAMERA_C 1

#include "../libs/cglm/cglm.h"
#include "./transform.c"

struct Camera {
  float fieldOfView;
  float aspect;
  float zNear;
  float zFar;

  //view on screen dimensions
  vec4 viewPort;
  //camera's specs (FOV, etc)
  mat4 projectionMatrix;
  
  struct Transform transform;

  // //position of camera
  // vec3 position;
  // //rotation of camera
  // vec3 direction;
  // //camera's transform (position + rotation)
  // mat4 viewMatrix;

  //combines transform with camera specs
  mat4 viewProjMatrix;

  //combines camera with a 3d model's transform context
  mat4 modelViewProjMatrix;

};
#define CameraP struct Camera *

void Camera_setViewPort (CameraP camera, float x, float y, float w, float h) {
  camera->viewPort[0] = x;
  camera->viewPort[1] = y;
  camera->viewPort[2] = w;
  camera->viewPort[3] = h;
}

void Camera_setFieldOfView (CameraP camera, float fov) {
  camera->fieldOfView = fov;
}
void Camera_setAspect (CameraP camera, float aspect) {
  camera->aspect = aspect;
}
void Camera_setNearFar (CameraP camera, float zNear, float zFar) {
  camera->zNear = zNear;
  camera->zFar = zFar;
}

//updates view matrix
void Camera_lookat (CameraP camera, vec3 target) {
  glm_lookat(camera->transform.position, target, GLM_YUP, camera->transform.modelMatrix);
}

void Camera_point_project (CameraP camera, vec3 point, vec3 dest) {
  glm_project(point, camera->modelViewProjMatrix, camera->viewPort, dest);
}

void Camera_update_viewMatrix (CameraP camera) {
  Transform_update(&camera->transform);
  // glm_look(camera->transform.position, camera->transform.rotation, GLM_YUP, camera->transform.modelMatrix);
}

void Camera_update_projMatrix (CameraP camera) {
  glm_perspective(camera->fieldOfView, camera->aspect, camera->zNear, camera->zFar, camera->projectionMatrix);
}

void Camera_update_viewProjMatrix (CameraP camera) {
  glm_mat4_mul(camera->projectionMatrix, camera->transform.modelMatrix, camera->viewProjMatrix);
}

void Camera_update_modelViewProjMatrix (CameraP camera, mat4 modelMatrix) {
  glm_mat4_mul(camera->viewProjMatrix, modelMatrix, camera->modelViewProjMatrix);
}

void Camera_update (CameraP camera, mat4 modelMatrix, bool view, bool proj) {
  if (view) Camera_update_viewMatrix(camera);
  if (proj) Camera_update_projMatrix(camera);
  Camera_update_viewProjMatrix(camera);
  Camera_update_modelViewProjMatrix(camera, modelMatrix);

}

#endif
