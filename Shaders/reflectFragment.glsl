#version 330 core

uniform sampler2D diffuseTex;
uniform samplerCube cubeTex;
uniform sampler2D bumpTex;
//uniform sampler2D shadowTex;

uniform vec3 cameraPos;

in Vertex{
   vec4 colour;
   vec2 texCoord;
   vec3 normal;
   vec3 tangent;
   vec3 binormal;
   vec3 worldPos;
  // vec4 shadowProj;
}IN;

out vec4 fragColour;

void main(void){
   vec4 diffuse = texture(diffuseTex, IN.texCoord);
   vec3 viewDir = normalize(cameraPos - IN.worldPos);
   mat3 TBN = mat3(normalize(IN.tangent), normalize(IN.binormal), normalize(IN.normal));
   vec3 bumpNormal = texture(bumpTex , IN.texCoord).rgb;
   bumpNormal = normalize(TBN *normalize(bumpNormal*2.0-1.0));

   vec3 reflectDir = reflect(-viewDir, normalize(bumpNormal));
   vec4 reflectTex = texture(cubeTex,reflectDir);
   
   float shadow = 1.0;   //new!

   //  vec3 shadowNDC = IN.shadowProj.xyz / IN.shadowProj.w;
    // if(abs(shadowNDC.x) < 1.0f && abs(shadowNDC.y) < 1.0f && abs(shadowNDC.z) < 1.0f){
           //  vec3 biasCoord = shadowNDC * 0.5f + 0.5f;
          //   float shadowZ = texture(shadowTex , biasCoord.xy).x;
          //   if(shadowZ < biasCoord.z) 
         //    { shadow = 0.1f;}
   //  }

   //float halfAgl = abs(dot( normalize(viewDir), normalize(IN.normal))); 

   fragColour = reflectTex +(diffuse * 0.25f);
   //fragColour *= shadow;
}