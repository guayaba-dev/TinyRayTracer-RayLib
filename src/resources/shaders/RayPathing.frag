#version 330

precision mediump float;

#define MAX_SPHERES 3
#define MAX_TRIANGLES 20

in vec2 fragTexCoord;

struct _Material {
  vec3 diffuseColor;
  float albido[4];  // Intesity of
  // Diffuse light
  // Specular light
  // Reflectiveness
  // Refraction
};

struct Sphere {
  vec3 position;
  float radius;
  _Material material;
};

struct Triangle {
  vec3 vertex[3];
  _Material material;
};

uniform Triangle u_triangles[MAX_TRIANGLES];
uniform Sphere u_spheres[MAX_SPHERES];
uniform vec2 resolution; // Dimensiones del RenderTexture

void main() {

  vec3 colorFinal = vec3(0.);

  if(u_spheres[0].position.x == 1){
    colorFinal.g = 1.; 
  }
  else {
    colorFinal.r = 1.;
  }

  gl_FragColor = vec4(colorFinal, 1.0); // Depuración visuali
}
