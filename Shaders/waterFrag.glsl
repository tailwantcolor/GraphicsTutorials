#version 330 core
uniform sampler2D diffuseTex;
uniform sampler2D bumpTex;
uniform sampler2D shadowTex; // shadow parlight
uniform sampler2D shadowTex_spot; // shadow spotlight

uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform vec4 lightColour;
uniform float lightRadius;
uniform float lightcosAngle;
uniform vec3  lightTarget;
uniform vec3 lightmidDir;
uniform vec4 ParlightColour;
uniform vec3 ParlightPos;
uniform vec3 ParlightTarget;
uniform vec3 ParlightDir;


in Vertex{
   vec4 colour;
   vec2 texCoord;
   vec3 normal;
   vec3 tangent;
   vec3 binormal;
   vec3 worldPos;
   vec4 shadowProj_par;//new 
   vec4 shadowProj_spot;//new
}IN;

out vec4 fragColour;

void main(void){
   // parlight
   vec3 incident = normalize(ParlightDir);
   vec3 viewDir = normalize(cameraPos - IN.worldPos);
   vec3 halfDir = normalize(incident + viewDir);

   //spotlight
   vec3 incident_spot = normalize(lightPos - IN.worldPos);
   vec3 viewDir_spot = normalize(cameraPos - IN.worldPos);
   vec3 halfDir_spot = normalize(incident_spot + viewDir_spot);

   mat3 TBN = mat3(normalize(IN.tangent) , normalize(IN.binormal),normalize(IN.normal));
   vec4 diffuse = texture(diffuseTex, IN.texCoord);
   vec3 bumpNormal = texture(bumpTex,IN.texCoord).rgb;
   bumpNormal = normalize(TBN*normalize(bumpNormal*2.0-1.0));

   //parlight
   float lambert_par = max(dot(incident , bumpNormal) , 0.0f);

   //spotlight
   float lambert_spot = max(dot(incident_spot , bumpNormal) , 0.0f);

   //just the spotlight has the attenuation
   float distance = length(lightPos - IN.worldPos);
   float attenuation = 1.0f - clamp(distance / lightRadius,0.0,1.0);

   //parlight
   float specFactor_par = clamp(dot(halfDir,bumpNormal),0.0,1.0);
   specFactor_par = pow(specFactor_par, 60.0);
   vec3 surface_par = (diffuse.rgb * lightColour.rgb);

   //spotlight
   float specFactor_spot = clamp(dot(halfDir_spot,bumpNormal),0.0,1.0);
   specFactor_spot = pow(specFactor_spot, 60.0);
   vec3 surface_spot = (diffuse.rgb * ParlightColour.rgb);

   //spotlight
   float is_in = 1.0f;
   if(dot(incident_spot , normalize(lightmidDir))<lightcosAngle)
   { is_in = 0.0f;
   }

   //parlight shadow
   float shadow_par = 1.0; 
   vec3 shadowNDC_par = IN.shadowProj_par.xyz / IN.shadowProj_par.w;
   if(abs(shadowNDC_par.x) < 1.0f && abs(shadowNDC_par.y) < 1.0f && abs(shadowNDC_par.z) < 1.0f){
             vec3 biasCoord_par = shadowNDC_par * 0.5f + 0.5f;
             float shadowZ_par = texture(shadowTex , biasCoord_par.xy).x;
             if(shadowZ_par <biasCoord_par.z-0.0001) //to do modify the value
             { shadow_par = 0.0f;}
    }

//spotlight shadow
   
   float shadow_spot = 1.0; 
   vec3 shadowNDC_spot = IN.shadowProj_spot.xyz / IN.shadowProj_spot.w;
   if(abs(shadowNDC_spot.x) < 1.0f && abs(shadowNDC_spot.y) < 1.0f && abs(shadowNDC_spot.z) < 1.0f){
             vec3 biasCoord_spot = shadowNDC_spot * 0.5f + 0.5f;
             float shadowZ_spot = texture(shadowTex_spot , biasCoord_spot.xy).x;
             if(shadowZ_spot <biasCoord_spot.z) //to do modify the value
             { shadow_spot = 0.0f;}
    }

    vec4 fragColour_spot = vec4(0,0,0,1);
    fragColour_spot.rgb = surface_spot * lambert_spot * attenuation;
    fragColour_spot.rgb +=(lightColour.rgb * specFactor_spot) * attenuation * 0.33;
    fragColour_spot.rgb *= is_in;
    fragColour_spot.rgb *= shadow_spot;

    vec4 fragColour_par = vec4(0,0,0,1);
    fragColour_par.rgb = surface_par * lambert_par;
    fragColour_par.rgb +=(ParlightColour.rgb * specFactor_par)  * 0.33;
    fragColour_par.rgb *= shadow_par;
    fragColour_par.rgb += surface_par * 0.1f;

    fragColour.rgb = fragColour_spot.rgb;
    fragColour.rgb += fragColour_par.rgb;

  // fragColour.rgb = surface_spot * lambert_spot * attenuation;
   //fragColour.rgb += (lightColour.rgb * specFactor_spot) * attenuation * 0.33; 
   //fragColour.rgb *= is_in;
  // fragColour.rgb += surface * 0.1f;
   
   fragColour.a = 0.5f;
  // fragColour = texture(diffuseTex, IN.texCoord);
}