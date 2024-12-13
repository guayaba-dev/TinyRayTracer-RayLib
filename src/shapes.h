#include <stdio.h>

#include "materials.h"
#include "raylib.h"

#define MAX_SPHERES 3
#define MAX_TRIANGLES 20

typedef struct Sphere {
  Vector3 position;
  float radius;
  _Material material;

} SPHERE;

typedef struct triangle {
  Vector3 vertex[3];
  _Material material;

} TRIANGLE;

static int triangleQuantity = 0;
static int sphereQuantity = 0;

SPHERE spheres[MAX_SPHERES] = {0};

TRIANGLE triangles[MAX_TRIANGLES] = {0};

void createSphere(Vector3 position, float radius, _Material material) {
  SPHERE tempSphere;
  tempSphere.position = position;
  tempSphere.radius = radius;
  tempSphere.material = material;

  if (sphereQuantity >= MAX_SPHERES) return;

  spheres[sphereQuantity] = tempSphere;
  sphereQuantity++;
};

void createTriangle(Vector3 vertex[3], _Material material) {
  TRIANGLE tempTriangle;
  tempTriangle.vertex[0] = vertex[0];
  tempTriangle.vertex[1] = vertex[1];
  tempTriangle.vertex[2] = vertex[2];
  tempTriangle.material = material;

  if (triangleQuantity >= MAX_TRIANGLES) return;

  triangles[triangleQuantity] = tempTriangle;
  triangleQuantity++;
};

void updateSphereShapeUniforms(Shader shader) {
  for (int i = 0; i < sphereQuantity; i++) {
    char uniformName[40];

    snprintf(uniformName, sizeof(uniformName), "u_spheres[%d].position", i);
    int SpherePositionLoc = GetShaderLocation(shader, uniformName);
    SetShaderValue(shader, SpherePositionLoc, &spheres[i].position,
                   SHADER_UNIFORM_VEC3);

    snprintf(uniformName, sizeof(uniformName), "u_spheres[%d].radius", i);
    int SphereRadiusLoc = GetShaderLocation(shader, uniformName);
    SetShaderValue(shader, SphereRadiusLoc, &spheres[i].radius,
                   SHADER_UNIFORM_FLOAT);

    snprintf(uniformName, sizeof(uniformName),
             "u_spheres[%d].material.diffuseColor", i);
    int SphereMaterialDiffeseColorLoc = GetShaderLocation(shader, uniformName);
    SetShaderValue(shader, SphereMaterialDiffeseColorLoc,
                   &spheres[i].material.diffuseColor, SHADER_UNIFORM_VEC4);

    snprintf(uniformName, sizeof(uniformName),
             "u_spheres[%d].material.albido[0]", i);
    int SphereMaterialAlbidoDiffuseLoc = GetShaderLocation(shader, uniformName);
    SetShaderValue(shader, SphereMaterialAlbidoDiffuseLoc,
                   &spheres[i].material.albido[0], SHADER_UNIFORM_FLOAT);

    snprintf(uniformName, sizeof(uniformName),
             "u_spheres[%d].material.albido[1]", i);
    int SphereMaterialAlbidoSpecularLoc =
        GetShaderLocation(shader, uniformName);
    SetShaderValue(shader, SphereMaterialAlbidoSpecularLoc,
                   &spheres[i].material.albido[1], SHADER_UNIFORM_FLOAT);

    snprintf(uniformName, sizeof(uniformName),
             "u_spheres[%d].material.albido[2]", i);
    int SphereMaterialAlbidoReflectiveLoc =
        GetShaderLocation(shader, uniformName);
    SetShaderValue(shader, SphereMaterialAlbidoReflectiveLoc,
                   &spheres[i].material.albido[2], SHADER_UNIFORM_FLOAT);

    snprintf(uniformName, sizeof(uniformName),
             "u_spheres[%d].material.albido[3]", i);
    int SphereMaterialAlbidoRefacctionLoc =
        GetShaderLocation(shader, uniformName);
    SetShaderValue(shader, SphereMaterialAlbidoRefacctionLoc,
                   &spheres[i].material.albido[3], SHADER_UNIFORM_FLOAT);
  }
}

void updateTriangleShapeUniforms(Shader shader) {
  for (int i = 0; i < triangleQuantity; i++) {
    char uniformName[40];

    snprintf(uniformName, sizeof(uniformName), "u_triangles[%d].vertex[0]", i);
    int TriangleVertex0Loc = GetShaderLocation(shader, uniformName);
    SetShaderValue(shader, TriangleVertex0Loc, &triangles[i].vertex[0],
                   SHADER_UNIFORM_VEC3);

    snprintf(uniformName, sizeof(uniformName), "u_triangles[%d].vertex[1]", i);
    int TriangleVertex1Loc = GetShaderLocation(shader, uniformName);
    SetShaderValue(shader, TriangleVertex1Loc, &triangles[i].vertex[1],
                   SHADER_UNIFORM_VEC3);

    snprintf(uniformName, sizeof(uniformName), "u_triangles[%d].vertex[2]", i);
    int TriangleVertex2Loc = GetShaderLocation(shader, uniformName);
    SetShaderValue(shader, TriangleVertex2Loc, &triangles[i].vertex[2],
                   SHADER_UNIFORM_VEC3);

    snprintf(uniformName, sizeof(uniformName),
             "u_triangles[%d].material.diffuseColor", i);
    int TriangleMaterialDiffeseColorLoc =
        GetShaderLocation(shader, uniformName);
    SetShaderValue(shader, TriangleMaterialDiffeseColorLoc,
                   &triangles[i].material.diffuseColor, SHADER_UNIFORM_VEC4);

    snprintf(uniformName, sizeof(uniformName),
             "u_triangles[%d].material.albido[0]", i);
    int TriangleMaterialAlbidoDiffuseLoc =
        GetShaderLocation(shader, uniformName);
    SetShaderValue(shader, TriangleMaterialAlbidoDiffuseLoc,
                   &triangles[i].material.albido[0], SHADER_UNIFORM_FLOAT);

    snprintf(uniformName, sizeof(uniformName),
             "u_triangles[%d].material.albido[1]", i);
    int TriangleMaterialAlbidoSpecularLoc =
        GetShaderLocation(shader, uniformName);
    SetShaderValue(shader, TriangleMaterialAlbidoSpecularLoc,
                   &triangles[i].material.albido[1], SHADER_UNIFORM_FLOAT);

    snprintf(uniformName, sizeof(uniformName),
             "u_triangles[%d].material.albido[2]", i);
    int TriangleMaterialAlbidoReflectiveLoc =
        GetShaderLocation(shader, uniformName);
    SetShaderValue(shader, TriangleMaterialAlbidoReflectiveLoc,
                   &triangles[i].material.albido[2], SHADER_UNIFORM_FLOAT);

    snprintf(uniformName, sizeof(uniformName),
             "u_triangles[%d].material.albido[3]", i);
    int TriangleMaterialAlbidoRefacctionLoc =
        GetShaderLocation(shader, uniformName);
    SetShaderValue(shader, TriangleMaterialAlbidoRefacctionLoc,
                   &triangles[i].material.albido[3], SHADER_UNIFORM_FLOAT);
  }
}
