#include <stdio.h>

#include "raylib.h"

#define MAX_LIGHTS 4
#define LIGHT_DIRECTIONAL 0
#define LIGHT_POINT 1

static int totalLights = 0;

typedef struct Light {
  int enabled;
  int type;
  Vector3 position;
  Vector3 target;
  Color color;
} LIGHT;

LIGHT lights[MAX_LIGHTS] = {0};

void drawLights() {
  for (int i = 0; i < MAX_LIGHTS; i++) {
    DrawSphereEx(lights[i].position, 0.2, 8.f, 8.f, lights[i].color);
  }
}

void createLight(int type, Vector3 position, Vector3 target, Color color,
                 Shader shader) {
  if (!(totalLights < MAX_LIGHTS)) return;

  lights[totalLights].enabled = 1;
  lights[totalLights].type = type;
  lights[totalLights].position = position;
  lights[totalLights].target = target;
  lights[totalLights].color = color;

  totalLights++;

  int lightEnableLoc[MAX_LIGHTS];
  int lightTypeLoc[MAX_LIGHTS];
  int lightPosLoc[MAX_LIGHTS];
  int lightTarLoc[MAX_LIGHTS];
  int lightColLoc[MAX_LIGHTS];

  for (int i = 0; i < MAX_LIGHTS; i++) {
    char uniformName[32];

    snprintf(uniformName, sizeof(uniformName), "lights[%d].enabled", i);
    lightEnableLoc[i] = GetShaderLocation(shader, uniformName);

    snprintf(uniformName, sizeof(uniformName), "lights[%d].type", i);
    lightTypeLoc[i] = GetShaderLocation(shader, uniformName);

    snprintf(uniformName, sizeof(uniformName), "lights[%d].position", i);
    lightPosLoc[i] = GetShaderLocation(shader, uniformName);

    snprintf(uniformName, sizeof(uniformName), "lights[%d].target", i);
    lightTarLoc[i] = GetShaderLocation(shader, uniformName);

    snprintf(uniformName, sizeof(uniformName), "lights[%d].color", i);
    lightColLoc[i] = GetShaderLocation(shader, uniformName);
  }

  // Enviar datos al shader
  for (int i = 0; i < MAX_LIGHTS; i++) {
    int enabled = lights[i].enabled;
    // int enabled = 1;
    SetShaderValue(shader, lightEnableLoc[i], &enabled, SHADER_UNIFORM_INT);

    int type = lights[i].type;
    SetShaderValue(shader, lightTypeLoc[i], &type, SHADER_UNIFORM_INT);

    Vector3 position = lights[i].position;
    SetShaderValue(shader, lightPosLoc[i], &position, SHADER_UNIFORM_VEC3);

    Vector3 target = lights[i].target;
    SetShaderValue(shader, lightTarLoc[i], &target, SHADER_UNIFORM_VEC3);

    Vector4 color = {lights[i].color.r / 255.f, lights[i].color.g / 255.f,
                     lights[i].color.b / 255.f, lights[i].color.a / 255.f};
    SetShaderValue(shader, lightColLoc[i], &color, SHADER_UNIFORM_VEC4);
  }
}
