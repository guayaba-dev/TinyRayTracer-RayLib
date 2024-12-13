#pragma once
#include "raylib.h"

// it has an _ so it doesn't interfire with raylib Material
typedef struct _Material {
  Color diffuseColor;
  float albido[4];  // Intesity of
  // Diffuse light
  // Specular light
  // Reflectiveness
  // Refraction
} _Material;
