#include "raylib.h"
#include "screens.h"

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;
static Camera camera = {0};
static Shader shader;
RenderTexture2D target;

void InitRayPathScreen() {
  framesCounter = 0;
  finishScreen = 0;
  camera.position = (Vector3){0.f, 4.f, 4.f};
  camera.up = (Vector3){0.f, 1.f, 0.f};
  camera.fovy = 60.f;
  target = LoadRenderTexture(GetScreenWidth(), GetScreenWidth());
  camera.projection = CAMERA_PERSPECTIVE;
}

void UpdateRayPathScreen() {}

void DrawRayPathScreen() { DrawTexture(target.texture, 0, 0, WHITE); }

void UnloadRayPathScreen() {}

int FinishRayPathScreen() { return finishScreen; }
