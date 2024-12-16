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


bool rayIntersectsSphere(vec3 orig, vec3 dir, vec3 sphereCenter, float radius, float intersectionPoint0){

  vec3 hypotenuse = sphereCenter - orig; 

  float projectionHtoDir = dot(hypotenuse, dir);

  float opposeCathetus = dot(hypotenuse,hypotenuse) - (projectionHtoDir*projectionHtoDir);

  if(opposeCathetus > radius*radius) return false;

  float adyacentCathetus = sqrt(radius*radius - opposeCathetus);

  intersectionPoint0 = projectionHtoDir - adyacentCathetus; 

  float intersectionPoint1 = projectionHtoDir + adyacentCathetus;

  if(intersectionPoint0 < 0) intersectionPoint0 = intersectionPoint1;

  if(intersectionPoint0 < 0) return false;

  return true;
}

void main() {

  //Temporal camera values
  const float PI = 3.14159265359;
  float fov = PI/4.;
  vec2 midlePixelCalc = .5/resolution; 

  float aspectRatio = resolution.y/resolution.x;

  vec3 rayDirection = vec3(1.); 

  rayDirection.xy = (fragTexCoord - 0.5)*tan(fov/2.)*2.;
  rayDirection.y *= aspectRatio;
  rayDirection.z = -1;
  
  rayDirection = normalize(rayDirection);

  vec3 colorFinal = vec3(0.2, 0.7, 0.8);
  
  float z_buffer = 1e20; // Profundidad inicial muy grande

  for (int i = 0; i < MAX_SPHERES; i++) {
    float intersectionPoint = 0.0;

    if(u_spheres[i].radius == 0.) continue; //would you beleave if i told you that it took me a day to figure this out?

    // Verifica si hay intersección
    if (!rayIntersectsSphere(vec3(0.), rayDirection, u_spheres[i].position, u_spheres[i].radius, intersectionPoint)) continue;

    // Actualiza el z_buffer si es más cercano
    if (intersectionPoint > z_buffer) continue;

    z_buffer = intersectionPoint;
    colorFinal = vec3(0.4, 0.4, 0.3); // Cambia el color si hay intersección
  }

  // if(u_spheres[0].position.z ==  -10.) colorFinal = vec3(0.,1.,0.);
 
  gl_FragColor = vec4(colorFinal, 1.0); 
}
