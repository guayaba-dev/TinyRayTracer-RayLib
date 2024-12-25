#version 330

precision mediump float;

#define RECURSIVE 8
#define MAX_SPHERES 5
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
  int enabled;
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
    vec3 position; vec3 target; vec4 color; 
};

uniform Light lights[MAX_LIGHTS];
uniform Triangle u_triangles[MAX_TRIANGLES];
uniform Sphere u_spheres[MAX_SPHERES];
uniform vec2 resolution; // Dimensiones del RenderTexture
uniform vec3 viewEye;
uniform vec3 viewCenter;

// function definitions 

bool rayIntersectsSphere(in vec3 orig, in vec3 dir, in vec3 sphereCenter, in float radius, inout float intersectionPoint);
bool rayIntersectScene(in vec3 orig, in vec3 dir, inout vec3 hitPos, inout _Material material, inout vec3 normal);
vec3 calculateLight(in vec3 hitpos, in vec3 dir, in vec3 normal, in _Material material);
vec3 reflectionCalc(in vec3 hitpos, in vec3 dir, in vec3 normal, in _Material material);
vec3 refractCalc(in vec3 hitpos, in vec3 dir, in vec3 normal, in _Material material);

void main() {
 
  //Temporal camera values
  const float PI = 3.14159265359;
  float fov = PI/2.0;
  vec2 midlePixelCalc = .5/resolution; 

  float aspectRatio = resolution.y/resolution.x;

  vec3 up = vec3(0.,1.,0.);
  vec3 viewDir = normalize(viewCenter - viewEye);
  vec3 planeRight = normalize(cross(viewDir, up));
  vec3 planeUp = normalize(cross(planeRight, viewDir));

  vec2 planeCoords = vec2(0.0);
  planeCoords.xy = (fragTexCoord - 0.5)*tan(fov/2.)*2.;
  planeCoords.y *= aspectRatio;
   
  vec3 rayDirection = vec3(0.); 
  rayDirection = planeCoords.x*planeRight + planeCoords.y * planeUp + viewDir;

  // rayDirection = vec3(planeCoords.x, planeCoords.y, -1.f);

  rayDirection = normalize(rayDirection);

  vec3 hitpos = vec3(0.0);
  vec3 normal = vec3(0.0);
  _Material currentMaterial; currentMaterial.diffuseColor = vec3(0.2, 0.7, 0.8); //background color

  if(rayIntersectScene(viewEye, rayDirection, hitpos, currentMaterial, normal)){

  vec3 reflectColor = reflectionCalc(hitpos, rayDirection, normal, currentMaterial);
  vec3 refractColor = refractCalc(hitpos, rayDirection, normal, currentMaterial);
  currentMaterial.diffuseColor = calculateLight(hitpos, rayDirection, normal, currentMaterial) + reflectColor + refractColor;

  }

  gl_FragColor = vec4(currentMaterial.diffuseColor, 1.0); 

}

vec3 refractRayCalc(in vec3 dir, in vec3 normal, in float refractIndex){
  
  float incidentCos = dot(dir, -normal);
  float incidentRefractIndex = 1;
  float materialRefractIndex = refractIndex;

  if(incidentCos < 0){
    float temporalIndex = incidentRefractIndex;

    incidentRefractIndex = materialRefractIndex;
    materialRefractIndex = temporalIndex;
    incidentCos = -incidentCos;
    normal = -normal;
  }

  float refractRatio = incidentRefractIndex/materialRefractIndex;
  float k = 1 - refractRatio*refractRatio*(1 - incidentCos*incidentCos);
  return k < 0 ? vec3(0,0,0) : dir*refractRatio + normal*(refractRatio * incidentCos - sqrt(k));
}

vec3 reflectionCalc(in vec3 hitpos, in vec3 dir, in vec3 normal, in _Material material){

  vec3 lightCalc[RECURSIVE];
  float reflectiveAlbido[RECURSIVE];
 
  for(int i = 0; i < RECURSIVE; i++){
    lightCalc[i] = vec3(0.0);
    reflectiveAlbido[i] = 0.0;
  }

  vec3 reflectOrig;
  vec3 reflectDir = dir;
  vec3 reflectHit = hitpos;
  vec3 reflectNormal = normal; 
  _Material reflectMaterial = material;
  
  for(int i = 0; i < RECURSIVE; i++){

    reflectOrig = reflectHit + reflectNormal*1e-3;
    reflectDir = normalize(reflect(reflectDir, reflectNormal));
    reflectiveAlbido[i] = reflectMaterial.albido[2];

    if(!rayIntersectScene(reflectOrig, reflectDir, reflectHit, reflectMaterial, reflectNormal) 
      )
    {
      lightCalc[i] = vec3(0.2, 0.7, 0.8);
      break;
    }
    
    lightCalc[i] = calculateLight(reflectHit, reflectDir, reflectNormal, reflectMaterial);
  }

  vec3 reflectionColor = vec3(0.0); 

  for(int i = 0; i < RECURSIVE; i++){
    
    int reverseIndex = RECURSIVE - i - 1;
    
    reflectionColor = (lightCalc[reverseIndex] + reflectionColor) * reflectiveAlbido[reverseIndex];

  }

  return reflectionColor;

}


vec3 refractCalc(in vec3 hitpos, in vec3 dir, in vec3 normal, in _Material material){

  vec3 lightCalc[RECURSIVE];
  float refractAlbido[RECURSIVE];
 
  for(int i = 0; i < RECURSIVE; i++){
    lightCalc[i] = vec3(0.0);
    refractAlbido[i] = 0.0;
  }

  vec3 refractOrig;
  vec3 refractDir = dir;
  vec3 refractHit = hitpos;
  vec3 refractNormal = normal; 
  _Material refractMaterial = material;
  
  for(int i = 0; i < RECURSIVE; i++){

    refractOrig = dot(refractNormal, refractDir) < 0.0 ? refractHit - refractNormal*1e-3 : refractHit + refractNormal*1e-3;
    refractDir = normalize(refractRayCalc(refractDir, refractNormal, 1.5));
    refractAlbido[i] = refractMaterial.albido[3];

    if(!rayIntersectScene(refractOrig, refractDir, refractHit, refractMaterial, refractNormal) 
      )
    {
      lightCalc[i] = vec3(0.2, 0.7, 0.8);
      break;
    }
    
    lightCalc[i] = calculateLight(refractHit, refractDir, refractNormal, refractMaterial);
  }

  vec3 refractColor = vec3(0.0); 

  for(int i = 0; i < RECURSIVE; i++){
    int reverseIndex = RECURSIVE - i - 1;
    refractColor = (lightCalc[reverseIndex] + refractColor) * refractAlbido[reverseIndex];
  }                                                                                                

  return refractColor;

}

bool rayIntersectsSphere(in vec3 orig, in vec3 dir, in vec3 sphereCenter, in float radius, inout float intersectionPoint){
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

bool rayIntersectsTriangle(in vec3 orig, in vec3 dir, in vec3[3] vertex, 
                           inout float intersectionPoint, inout vec3 normal){
  
  vec3 ABdir = (vertex[1] - vertex[0]);
  vec3 ACdir = (vertex[2] - vertex[0]);

  normal = normalize(cross(ABdir,ACdir));
  
  if(abs(dot(dir, normal)) < 1e-6) return false;
  
  intersectionPoint = dot(normal,(vertex[0] - orig))/dot(normal,dir); 
  
  if(intersectionPoint < 0) return false;

  vec3 intersectionPlane = orig + intersectionPoint*dir; 
  
  //Barycentric coordinate
  vec3 v0 = vertex[1] - vertex[0]; // AB
  vec3 v1 = vertex[2] - vertex[0]; // AC
  vec3 v2 = intersectionPlane - vertex[0]; // Punto de intersección desde A

  float d00 = dot(v0, v0);
  float d01 = dot(v0, v1);
  float d11 = dot(v1, v1);
  float d20 = dot(v2, v0);
  float d21 = dot(v2, v1);

  float denom = d00 * d11 - d01 * d01;
  if (abs(denom) < 1e-6) return false; // Evita divisiones por cero

  float v = (d11 * d20 - d01 * d21) / denom;
  float w = (d00 * d21 - d01 * d20) / denom;
  float u = 1.0 - v - w;

  // Verifica si está dentro del triángulo
  if (u < 0.0 || v < 0.0 || w < 0.0) return false;

  return true;

}


bool rayIntersectScene(in vec3 orig, in vec3 dir, inout vec3 hitPos,
                       inout _Material material, inout vec3 normal)
{

  float z_buffer = 1e20; 

  for (int j=0; j < MAX_TRIANGLES; j++) {
    float intersectionPoint = 0.0;
    vec3 tempNormal = vec3(0.0);

    if(u_triangles[j].enabled == 0) 
      continue;

    if (!rayIntersectsTriangle(orig, dir, u_triangles[j].vertex,
                               intersectionPoint, tempNormal))
      continue;

    if (intersectionPoint > z_buffer) continue;
    z_buffer = intersectionPoint;
    hitPos = orig + intersectionPoint*dir;
    normal = tempNormal;
    material = u_triangles[j].material;
  }
  
  for (int i = 0; i < MAX_SPHERES; i++) {
    float intersectionPoint = 0.0;

    //would you believe me if i told you that it took me a day to figure this out?
    if(u_spheres[i].radius == 0.) continue;

    if (!rayIntersectsSphere(orig, dir, u_spheres[i].position,
                             u_spheres[i].radius, intersectionPoint))
      continue;

    if (intersectionPoint > z_buffer) continue;
    z_buffer = intersectionPoint;
    hitPos = orig + intersectionPoint*dir;
    normal = normalize(hitPos - u_spheres[i].position);
    material = u_spheres[i].material;
  }  



  return z_buffer < 1e20;
}


vec3 calculateLight(in vec3 hitpos, in vec3 dir, in vec3 normal, in _Material material){

  vec3 specularSum = vec3(0); 
  vec3 diffuseSum = vec3(0);


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
      
      float light_distance = distance(lights[j].position, hitpos);
      vec3 shadowHit = vec3(0.0);
      vec3 shadow_orig = hitpos + normal*1e-3;
      vec3 shadowNormal = vec3(0.0);
      _Material tempMaterial; 
      tempMaterial.diffuseColor = vec3(0.0);

      if(rayIntersectScene(shadow_orig, light, shadowHit, tempMaterial, shadowNormal) && 
        (distance(shadow_orig, shadowHit) < light_distance)) continue;

      float NdotL = max(dot(normal, light), 0.0);
      vec3 lightDot = (lights[j].color.rgb*material.diffuseColor*NdotL);

      //specular light
      float specCo = 0.0;
      if (NdotL > 0.0) specCo = pow(max(0.0, dot(-dir, reflect(-(light), normal))), 100.0);
       // 16 refers to shine

      diffuseSum += lightDot; 
      specularSum += specCo;
  }  

  return diffuseSum*material.albido[0] + specularSum*material.albido[1];
}
