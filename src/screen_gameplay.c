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

#include "lights.h"
#include "materials.h"
#include "raylib.h"
#include "raymath.h"
#include "rcamera.h"
#include "screens.h"

#define GLSL_VERSION 100

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;
static Camera camera = {0};
static Shader shaderLight;

Mesh cubeMesh;
Model cubeModel;

Mesh sphereMesh;
Model sphereModel;

Mesh planeMesh;
Model planeModel;
//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void InitGameplayScreen(void) {
  framesCounter = 0;
  finishScreen = 0;

  camera.position = (Vector3){0.f, 4.f, 4.f};
  camera.up = (Vector3){0.f, 1.f, 0.f};
  camera.fovy = 60.f;
  camera.projection = CAMERA_PERSPECTIVE;

  shaderLight =
      LoadShader(TextFormat("resources/shaders/PhongLight.vert", GLSL_VERSION),
                 TextFormat("resources/shaders/PhongLight.frag", GLSL_VERSION));

  cubeMesh = GenMeshCube(1.0f, 1.0f,
                         1.0f);  // Tamaño del cubo (ancho, alto, profundidad)
  cubeModel = LoadModelFromMesh(cubeMesh);  // Crea un modelo a partir del mesh

  cubeModel.materials[0].shader = shaderLight;  // Asigna tu shader al modelo

  sphereMesh = GenMeshSphere(.5, 10, 10);

  sphereModel = LoadModelFromMesh(sphereMesh);

  sphereModel.materials[0].shader = shaderLight;

  planeMesh = GenMeshPlane(2., 2., 2, 2);

  planeModel = LoadModelFromMesh(planeMesh);

  planeModel.transform =
      MatrixMultiply(planeModel.transform, MatrixRotateX(0.01f));

  planeModel.materials[0].shader = shaderLight;

  // Ambient light level (some basic lighting)
  int ambientLoc = GetShaderLocation(shaderLight, "ambient");
  SetShaderValue(shaderLight, ambientLoc, (float[4]){0.1f, 0.1f, 0.1f, 1.0f},
                 SHADER_UNIFORM_VEC4);

  shaderLight.locs[SHADER_LOC_VECTOR_VIEW] =
      GetShaderLocation(shaderLight, "viewPos");

  createLight(LIGHT_POINT, (Vector3){1.f, 1.f, 1.f}, (Vector3){0}, WHITE,
              shaderLight);
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void) {
  UpdateCamera(&camera, CAMERA_PERSPECTIVE);

  float cameraPosition[3] = {camera.position.x, camera.position.y,
                             camera.position.z};

  SetShaderValue(shaderLight, shaderLight.locs[SHADER_LOC_VECTOR_VIEW],
                 cameraPosition, SHADER_UNIFORM_VEC3);

  // Press enter to change to ENDING screen
  if (IsKeyPressed(KEY_ENTER)) {
    finishScreen = 1;
  }
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void) {
  ClearBackground(BLACK);

  BeginMode3D(camera);

  BeginShaderMode(shaderLight);

  DrawModel(cubeModel, (Vector3){0.0f, 0.0f, 0.0f}, .5, RED);
  DrawModel(sphereModel, (Vector3){2.0f, 0.0f, 0.0f}, .5, BLUE);
  DrawModel(cubeModel, (Vector3){2.0f, 0.0f, 2.0f}, .5, GREEN);
  DrawModel(sphereModel, (Vector3){0.0f, 0.0f, 2.0f}, .5, YELLOW);
  DrawModel(planeModel, (Vector3){1.0f, 0.0f, 1.0f}, 2.f, WHITE);

  EndShaderMode();

  drawLights();

  EndMode3D();
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void) {
  UnloadShader(shaderLight);
  UnloadModel(cubeModel);
  UnloadModel(sphereModel);
  UnloadModel(planeModel);
}

// Gameplay Screen should finish
int FinishGameplayScreen(void) { return finishScreen; }
