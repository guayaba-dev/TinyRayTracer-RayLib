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

// NOTE: Add here your custom variables

#define     MAX_LIGHTS              4
#define     LIGHT_DIRECTIONAL       0
#define     LIGHT_POINT             1

struct Light {
    int enabled;
    int type;
    vec3 position;
    vec3 target;
    vec4 color;
};

uniform Light lights[MAX_LIGHTS];
uniform Triangle u_triangles[MAX_TRIANGLES];
uniform Sphere u_spheres[MAX_SPHERES];
uniform vec2 resolution; // Dimensiones del RenderTexture

float intersectionPoint;

bool rayIntersectsSphere(vec3 orig, vec3 dir, vec3 sphereCenter, float radius){

  vec3 hypotenuse = sphereCenter - orig; 

  float projectionHtoDir = dot(hypotenuse, dir);

  float opposeCathetus = dot(hypotenuse,hypotenuse) - (projectionHtoDir*projectionHtoDir);

  if(opposeCathetus > radius*radius) return false;

  float adyacentCathetus = sqrt(radius*radius - opposeCathetus);

  intersectionPoint = projectionHtoDir - adyacentCathetus; 

  float intersectionPoint1 = projectionHtoDir + adyacentCathetus;

  if(intersectionPoint < 0) intersectionPoint = intersectionPoint1;

  if(intersectionPoint < 0) return false;

  return true;
}


void main() {

  //Temporal camera values
  const float PI = 3.14159265359;
  float fov = PI/2.;
  vec2 midlePixelCalc = .5/resolution; 
  vec3 cameraPos = vec3(0.);

  float aspectRatio = resolution.y/resolution.x;

  vec3 rayDirection = vec3(1.); 

  rayDirection.xy = (fragTexCoord - 0.5)*tan(fov/2.)*2.;
  rayDirection.y *= aspectRatio;
  rayDirection.z = -1;
  
  rayDirection = normalize(rayDirection);

  vec3 colorFinal = vec3(0.2, 0.7, 0.8);
  
  float z_buffer = 1e20; 

  vec3 hitpos = vec3(0.0);
  vec3 normal = vec3(0.0);

  for (int i = 0; i < MAX_SPHERES; i++) {
    intersectionPoint = 0.0;

    if(u_spheres[i].radius == 0.) continue; //would you believe me if i told you that it took me a day to figure this out?

    if (!rayIntersectsSphere(cameraPos, rayDirection, u_spheres[i].position, u_spheres[i].radius)) continue;

    if (intersectionPoint > z_buffer) continue;
   
    z_buffer = intersectionPoint;
   
    hitpos = intersectionPoint*rayDirection;

    normal = normalize(hitpos - u_spheres[i].position);

    //light calcualtions 
    colorFinal = u_spheres[i].material.diffuseColor;
  }  

  for(int j = 0; j < MAX_LIGHTS; j++){

    if(lights[j].enabled == 0) continue; 

      vec3 light = vec3(0.0);
     
      if (lights[j].type == LIGHT_DIRECTIONAL)
      {
        light = -normalize(lights[j].target - lights[j].position);
      }

      if (lights[j].type == LIGHT_POINT)
      {
         light = normalize(lights[j].position - hitpos);
      }
     
     //Diffuse light
      
      if(dot(normal,light) < 0.0) continue;

      float NdotL = max(dot(normal, light), 0.0);
      vec3 lightDot = (lights[j].color.rgb*colorFinal*NdotL);

    //specular light
      float specCo = 0.0;
      if (NdotL > 0.0) specCo = pow(max(0.0, dot(-rayDirection, reflect(-(light), normal))), 10.0);
     // 16 refers to shine

      colorFinal += lightDot; 
      colorFinal += specCo;
  }  

  
  gl_FragColor = vec4(colorFinal, 1.0); 
}
