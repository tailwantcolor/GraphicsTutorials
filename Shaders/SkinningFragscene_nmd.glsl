#version 330 core
uniform sampler2D diffuseTex;
uniform sampler2D shadowTex；

in Vertex{
   vec2 texCoord;
   vec4 shadowProj；
}IN;

out vec4 fragColour;
void main(void){
    
     vec3 shadowNDC = IN.shadowProj.xyz / IN.shadowProj.w;

    float shadow = 1.0;
     if(abs(shadowNDC.x) < 1.0f && abs(shadowNDC.y) < 1.0f && abs(shadowNDC.z) < 1.0f){
             vec3 biasCoord = shadowNDC * 0.5f + 0.5f;
             float shadowZ = texture(shadowTex , biasCoord.xy).x;
             if(shadowZ <biasCoord.z) //to do modify the value
             { shadow = 0.1f;}
      }
      fragColour = texture(diffuseTex, IN.texCoord);
      fragColour.rgb *= shadow;
}