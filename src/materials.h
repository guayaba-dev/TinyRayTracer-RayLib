#include "raylib.h"

// it has an _ so it doesn't interfire with raylib Material
typedef struct _Material {
  Color diffuseColor;
  float albido[4];
  // intesity of diffuse light, specular light,
  //             reflectiveness and refraction
} _Material;

// TODO: write the functions for materials to work properly
