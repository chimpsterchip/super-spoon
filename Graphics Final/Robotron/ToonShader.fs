#version 330

uniform sampler2D u_colorTexture; // diffuse texture
uniform vec3 u_baseColor; // shading color
uniform float u_numShades; // number of shades

// inputs from vertex shader
in vec3 v_normal;
in vec2 v_texcoord1;
in vec3 v_directionToLight;
in vec3 v_directionToCamera;

layout(location = 0) out vec4 o_FragColor;

// calculate diffuse component of lighting
float diffuseSimple(vec3 L, vec3 N){
   return clamp(dot(L,N),0.0,1.0);
}

// calculate specular component of lighting
float specularSimple(vec3 L,vec3 N,vec3 H){
   if(dot(N,L)>0){
      return pow(clamp(dot(H,N),0.0,1.0),64.0);
   }
   return 0.0;
}

void main(void){
   // sample color from diffuse texture
   vec3 colfromtex = texture( u_colorTexture, v_texcoord1 ).rgb;

   // calculate total intensity of lighting
   vec3 halfVector = normalize( v_directionToLight + v_directionToCamera );
   float iambi = 0.1;
   float idiff = diffuseSimple(v_directionToLight, v_normal);
   float ispec = specularSimple(v_directionToLight,v_normal, halfVector);
   float intensity = iambi + idiff + ispec;

   // quantize intensity for cel shading
   float shadeIntensity = ceil(intensity * u_numShades)/ u_numShades;

   // use base color
   o_FragColor.xyz = u_baseColor*shadeIntensity ; 
   // or use color from texture
   o_FragColor.xyz = colfromtex*shadeIntensity ;
   // or use mixed colors
   o_FragColor.xyz = u_baseColor * colfromtex*shadeIntensity ; 

   o_FragColor.w = 1.0;
}