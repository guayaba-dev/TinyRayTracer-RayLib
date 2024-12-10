#include "raylib.h"
#include "screens.h"

#define GLSL_VERSION 330

// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;
static Camera camera = {0};
static Shader shaderRayPathing;
RenderTexture2D target;

void loadShaders() {
  shaderRayPathing = LoadShader(
      0, TextFormat("resources/shaders/RayPathing.frag", GLSL_VERSION));
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
}

void drawShaderOnTexture() {
  BeginTextureMode(target);

  BeginShaderMode(shaderRayPathing);

  DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), WHITE);

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
