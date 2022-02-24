#version 330 core
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 shadowmatrix_posx;
uniform mat4 shadowmatrix_negx;
uniform mat4 shadowmatrix_posy;
uniform mat4 shadowmatrix_negy;
uniform mat4 shadowmatrix_posz;
uniform mat4 shadowmatrix_negz;

uniform vec3 lightPos;

in vec3 position;
in vec3 colour;
in vec3 normal;
in vec4 tangent;
in vec2 texCoord;

out Vertex{
       vec3 colour;
       vec2 texCoord;
       vec3 normal;
       vec3 tangent;
       vec3 binormal;
	   vec3 viewDir;// sampler vector
       vec3 worldPos;
       vec4 shadowProj;// a viewMatrix only
}OUT;

void main(void){
         OUT.colour=colour;
		 OUT.texCoord=texCoord;
		 mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
		 vec3 wNormal = normalize(normalMatrix * normalize(normal));
        vec3 wTangent = normalize(normalMatrix * normalize(tangent.xyz));

       OUT.normal = wNormal;
       OUT.tangent = wTangent;
       OUT.binormal = cross(wNormal, wTangent) * tangent.w;

       vec4 worldPos =  (modelMatrix * vec4(position, 1));
       OUT.worldPos = worldPos.xyz;
       gl_Position = (projMatrix * viewMatrix) * worldPos;
       vec3 viewDir = normalize(lightPos - worldPos.xyz);
	   OUT.viewDir = viewDir;
       vec4 pushVal = vec4(OUT.normal,0) * dot(viewDir , OUT.normal);
	   float a = 1.0f;
       if(abs(viewDir.x )> abs(viewDir.y) && abs(viewDir.x)>abs(viewDir.z))
	   { 
		 if(viewDir.x>0)
	     {OUT.shadowProj = shadowmatrix_posx * (worldPos + pushVal);
		  a = 1.0f;}
		 else 
		 {OUT.shadowProj = shadowmatrix_negx * (worldPos + pushVal);
		  a = 1.0f;}

	    }
	   if(abs(viewDir.y) > abs(viewDir.x) && abs(viewDir.y)>abs(viewDir.z))
	    {
		 if(viewDir.y>0)
	     {OUT.shadowProj = shadowmatrix_posy * (worldPos + pushVal);
		  a = 1.0f;}
		  else
		  {OUT.shadowProj = shadowmatrix_negy * (worldPos + pushVal);
		   a=1.0f;}
		}
		if(abs(viewDir.z)>abs(viewDir.x) && abs(viewDir.z)>abs(viewDir.y))
		{
		 if(viewDir.z>0)
		 {OUT.shadowProj = shadowmatrix_posz * (worldPos + pushVal);
		  a = 1.0f;}
		  else
		  {OUT.shadowProj = shadowmatrix_negz * (worldPos + pushVal);
		   a=1.0f;}
		}
		if(a==0.0f) { OUT.shadowProj = shadowmatrix_posx * (worldPos + pushVal);}


	       

}