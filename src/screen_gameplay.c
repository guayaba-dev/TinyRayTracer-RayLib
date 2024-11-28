/**********************************************************************************************
 *
 *   raylib - Advance Game template
 *
 *   Gameplay Screen Functions Definitions (Init, Update, Draw, Unload)
 *
 *   Copyright (c) 2014-2022 Ramon Santamaria (@raysan5)
 *
 *   This software is provided "as-is", without any express or implied warranty.
 *In no event will the authors be held liable for any damages arising from the
 *use of this software.
 *
 *   Permission is granted to anyone to use this software for any purpose,
 *including commercial applications, and to alter it and redistribute it freely,
 *subject to the following restrictions:
 *
 *     1. The origin of this software must not be misrepresented; you must not
 *claim that you wrote the original software. If you use this software in a
 *product, an acknowledgment in the product documentation would be appreciated
 *but is not required.
 *
 *     2. Altered source versions must be plainly marked as such, and must not
 *be misrepresented as being the original software.
 *
 *     3. This notice may not be removed or altered from any source
 *distribution.
 *
 **********************************************************************************************/

#include <stdio.h>

#include "raylib.h"
#include "rcamera.h"
#include "screens.h"

#define GLSL_VERSION 300

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;
static Camera camera = {0};
static Shader shader;
//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void InitGameplayScreen(void) {
  printf("ScreenGameplay showing up");
  framesCounter = 0;
  finishScreen = 0;

  camera.position = (Vector3){0.f, 4.f, 4.f};
  camera.up = (Vector3){0.f, 1.f, 0.f};
  camera.fovy = 60.f;
  camera.projection = CAMERA_PERSPECTIVE;

  shader =
      LoadShader(TextFormat("resources/shaders/RayTracer.vert", GLSL_VERSION),
                 TextFormat("resources/shaders/RayTracer.frag", GLSL_VERSION));
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void) {
  UpdateCamera(&camera, CAMERA_PERSPECTIVE);

  // Press enter or tap to change to ENDING screen
  if (IsKeyPressed(KEY_ENTER)) {
    finishScreen = 1;
  }
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void) {
  BeginDrawing();

  ClearBackground((Color){0});

  BeginMode3D(camera);

  DrawSphereEx((Vector3){0}, 0.5f, 10.f, 10.f, WHITE);

  BeginShaderMode(shader);

  EndShaderMode();

  EndMode3D();

  EndDrawing();
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void) { UnloadShader(shader); }

// Gameplay Screen should finish?
int FinishGameplayScreen(void) { return finishScreen; }
