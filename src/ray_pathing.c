#include <stdio.h>

#include "lights.h"
#include "materials.h"
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include "screens.h"
#include "shapes.h"

#define GLSL_VERSION 330

// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;
static Camera camera = {0};
static Shader shaderRayPathing;
RenderTexture2D target;

_Material shiningMate = {(Vector3){.4, .4, .3}, {1., 1., 0.2, 0.}};
_Material mirror = {(Vector3){1., 1., 1.}, {0., 10., .8, 0.}};
_Material glass = {(Vector3){0.6, 0.7, 0.8}, {0., .5, .1, 0.8}};
_Material redRubber = {(Vector3){0.3, 0.1, 0.1}, {1., .2, 0., 0.}};

int viewEyeLoc;
int viewCenterLoc;
int runTimeLoc;
int resolutionLoc;

// function declarations
void LoadShaders();
void InitRayPathScreen();
void drawShaderOnTexture();

void loadShaders() {
  UnloadShader(shaderRayPathing);
  resetLights();

  shaderRayPathing = LoadShader(
      0, TextFormat("resources/shaders/RayPathing.frag", GLSL_VERSION));
  // Resolution uniform

  viewEyeLoc = GetShaderLocation(shaderRayPathing, "viewEye");
  viewCenterLoc = GetShaderLocation(shaderRayPathing, "viewCenter");
  resolutionLoc = GetShaderLocation(shaderRayPathing, "resolution");

  Vector2 resolution = (Vector2){GetScreenWidth(), GetScreenHeight()};
  int resolutionLoc = GetShaderLocation(shaderRayPathing, "resolution");
  SetShaderValue(shaderRayPathing, resolutionLoc, &resolution,
                 SHADER_UNIFORM_VEC2);

  createLight(LIGHT_POINT, (Vector3){-20., 20., -20.}, Vector3Zero(), WHITE,
              shaderRayPathing);

  createLight(LIGHT_POINT, (Vector3){30., 50., -25.}, Vector3Zero(), WHITE,
              shaderRayPathing);

  createLight(LIGHT_POINT, (Vector3){30., 20., 30.}, Vector3Zero(), WHITE,
              shaderRayPathing);

  updateSphereShapeUniforms(shaderRayPathing);
  updateTriangleShapeUniforms(shaderRayPathing);
  drawShaderOnTexture();
}

void InitRayPathScreen() {
  framesCounter = 0;
  finishScreen = 0;
  camera.position = (Vector3){0.f, 0.f, 0.f};
  camera.target = (Vector3){0.f, 0.f, -1.f};
  camera.up = (Vector3){0.f, 1.f, 0.f};
  camera.fovy = 60.f;
  camera.projection = CAMERA_PERSPECTIVE;

  target = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

  _Material tempMaterial = {0};

  createSphere((Vector3){-3., -0., 16.}, 2., shiningMate);
  createSphere((Vector3){-1., -1.5, -12.}, 2., glass);
  createSphere((Vector3){1.5, -0.5, -18.}, 3., redRubber);
  createSphere((Vector3){7., 5., 18.}, 4., mirror);

  Vector3 tempVertex[3] = {
      {1.0f, 0.0f, 0.0f},  // Primer vector
      {0.0f, 0.0f, 0.0f},  // Segundo vector
      {0.0f, 0.0f, 0.0f}   // Tercer vector
  };

  createTriangle(tempVertex, tempMaterial);

  loadShaders();  // Always last function to call so the uniforms are updated
}

void drawShaderOnTexture() {
  BeginTextureMode(target);
  SetShapesTexture(target.texture,
                   (Rectangle){0.0f, 0.0f, (float)target.texture.width,
                               (float)target.texture.height});

  BeginShaderMode(shaderRayPathing);

  DrawRectangle(0, 0, target.texture.width, target.texture.height, WHITE);

  EndShaderMode();

  EndTextureMode();
}

void UpdateRayPathScreen() {
  UpdateCamera(&camera, CAMERA_PERSPECTIVE);

  float cameraPos[3] = {camera.position.x, camera.position.y,
                        camera.position.z};
  float cameraTarget[3] = {camera.target.x, camera.target.y, camera.target.z};

  printf("CameraPos: x=%f, y=%f, z=%f \n ", cameraPos[0], cameraPos[1],
         cameraPos[2]);

  printf("cameraTarget: x=%f, y=%f, z=%f \n ", cameraTarget[0], cameraTarget[1],
         cameraTarget[2]);

  printf("camera normalize: x=%f, y=%f, z=%f \n ",
         cameraTarget[0] - cameraPos[0], cameraTarget[1] - cameraPos[1],
         cameraTarget[2] - cameraPos[2]);

  SetShaderValue(shaderRayPathing, viewEyeLoc, cameraPos, SHADER_UNIFORM_VEC3);
  SetShaderValue(shaderRayPathing, viewCenterLoc, cameraTarget,
                 SHADER_UNIFORM_VEC3);

  if (IsKeyPressed(KEY_R)) loadShaders();

  drawShaderOnTexture();
}

void DrawRayPathScreen() { DrawTexture(target.texture, 0, 0, WHITE); }

void UnloadRayPathScreen() {
  UnloadShader(shaderRayPathing);
  UnloadRenderTexture(target);
}

int FinishRayPathScreen() { return finishScreen; }
