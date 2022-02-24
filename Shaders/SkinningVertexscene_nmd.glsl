#version 400

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 shadowMatrix;

in vec3 position;
in vec2 texCoord;
in vec4 jointWeights;
in ivec4 jointIndices;

uniform mat4    joints[128];

out Vertex{
       vec2 texCoord;
       vec3 worldPos;
       vec4 shadowProj;
}OUT;

void main(void){
       OUT.texCoord = texCoord;
       vec4 localPos= vec4(position,1.0f);
       vec4 skelPos = vec4(0,0,0,0);
 
       for(int i=0; i<4 ; ++i){
             int  jointIndex = jointIndices[i];
             float jointWeight = jointWeights[i];

             skelPos += joints[jointIndex] * localPos *jointWeight;
       }
       mat4 mvp = projMatrix * viewMatrix * modelMatrix;
       vec4 worldPos = (modelMatrix * vec4(skelPos, 1));
       OUT.worldPos = worldPos.xyz;
       gl_Position = mvp * vec4(skelPos.xyz, 1.0);
       OUT.shadowProj = shadowMatrix * (worldPos);
     
}