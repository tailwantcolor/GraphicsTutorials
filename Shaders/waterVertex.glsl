#version 330 core
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;
uniform mat4 shadowMatrix;
uniform mat4 shadowparmat;
uniform mat4 shadowspotmat;

uniform float time;

uniform sampler2D waveTex;

in vec3 position;
in vec4 colour;
in vec3 normal;
in vec4 tangent; 
in vec2 texCoord;

out Vertex{
  vec4 colour;
  vec2 texCoord;
  vec3 normal;
  vec3 tangent;
  vec3 binormal;
  vec3 worldPos;
  vec4 shadowProj_par; // a new value!
  vec4 shadowProj_spot;
}OUT;

void main(void){
   OUT.colour =colour;
   OUT.texCoord = (textureMatrix * vec4(texCoord, 0.0, 1.0)).xy;

   mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));

   vec3 wNormal = normalize(normalMatrix * normalize(normal));
   vec3 wTangent = normalize(normalMatrix * normalize(tangent.xyz));

   OUT.normal = wNormal;
   OUT.tangent = wTangent;
   OUT.binormal = cross(wTangent, wNormal) * tangent.w;

   mat4 mvp = projMatrix * viewMatrix * modelMatrix;
   float offset = texture(waveTex,texCoord).x+texture(waveTex,texCoord).y+texture(waveTex,texCoord).z;
   vec4 worldPos = modelMatrix * vec4(position,1.0);
   worldPos.y *= 80*sin(offset*time);
   OUT.worldPos = worldPos.xyz;
   vec4 s = (projMatrix * viewMatrix) * worldPos;

  // vec4 a = mvp * vec4(position,1.0);
   //a.y *= sin(offset*time);
   gl_Position = s;
   OUT.shadowProj_par = shadowparmat * worldPos;
   OUT.shadowProj_spot = shadowspotmat * worldPos;
}