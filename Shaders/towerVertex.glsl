#version 330 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 shadowMatrix; // a new uniform!

uniform vec4 ParlightColour;
uniform vec3 ParlightPos;
uniform vec3 ParlightTarget;
uniform vec3 ParlightDir;

uniform mat4 shadowparmat;
uniform mat4 shadowspotmat;

uniform vec3 cameraPos;

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
} OUT;

void main(void) {
       OUT.colour = colour;
       OUT.texCoord = texCoord;
       mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
       vec3 wNormal = normalize(normalMatrix * normalize(normal));
       vec3 wTangent = normalize(normalMatrix * normalize(tangent.xyz));

       OUT.normal = wNormal;
       OUT.tangent = wTangent;
       OUT.binormal = cross(wNormal, wTangent) * tangent.w;

       vec4 worldPos =  (modelMatrix * vec4(position, 1));
       OUT.worldPos = worldPos.xyz;
       gl_Position = (projMatrix * viewMatrix) * worldPos;

       //changing
       //parlight
       //vec3 viewDir_par = normalize(ParlightDir);
      // vec4 pushVal = vec4(OUT.normal,0) * dot(viewDir_par , OUT.normal);
       OUT.shadowProj_par = shadowparmat * worldPos;
       OUT.shadowProj_spot = shadowspotmat * worldPos;

      

}