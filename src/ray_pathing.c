#include "materials.h"
#include "raylib.h"
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

// function declarations
void LoadShaders();
void InitRayPathScreen();
void drawShaderOnTexture();

void loadShaders() {
  shaderRayPathing = LoadShader(
      0, TextFormat("resources/shaders/RayPathing.frag", GLSL_VERSION));
  drawShaderOnTexture();
}

void InitRayPathScreen() {
  framesCounter = 0;
  finishScreen = 0;
  camera.position = (Vector3){0.f, 4.f, 4.f};
  camera.up = (Vector3){0.f, 1.f, 0.f};
  camera.fovy = 60.f;

  target = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
  camera.projection = CAMERA_PERSPECTIVE;
  loadShaders();

  _Material tempMaterial = {0};

  createSphere((Vector3){1., 1., 0.}, 5.f, tempMaterial);

  Vector3 tempVertex[3] = {
      {1.0f, 0.0f, 0.0f},  // Primer vector
      {0.0f, 0.0f, 0.0f},  // Segundo vector
      {0.0f, 0.0f, 0.0f}   // Tercer vector
  };

  createTriangle(tempVertex, tempMaterial);

  updateSphereShapeUniforms(shaderRayPathing);
  updateTriangleShapeUniforms(shaderRayPathing);

  printf("%f,", spheres[0].position.x);
  printf("%f,", spheres[0].position.y);
  printf("%f\n", spheres[0].position.z);

  printf("%f\n", spheres[0].radius);
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
  if (IsKeyPressed(KEY_R)) loadShaders();
  if (IsKeyPressed(KEY_P)) drawShaderOnTexture();
}

void DrawRayPathScreen() { DrawTexture(target.texture, 0, 0, WHITE); }

void UnloadRayPathScreen() {
  UnloadShader(shaderRayPathing);
  UnloadRenderTexture(target);
}

int FinishRayPathScreen() { return finishScreen; }
