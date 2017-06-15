#version 330

// vertex attributes 
layout(location = 0) in vec3 i_position;
layout(location = 1) in vec3 i_normal;
layout(location = 2) in vec2 i_texcoord1;

uniform mat4 viewProj; // viewProj-projection matrix
uniform mat4 model; // model matrix
uniform mat3 u_normal_mat; // normal matrix

uniform vec3 lightPos; 
uniform vec3 cameraPos;

// inputs for fragment shader
out vec3 v_normal;
out vec2 v_texcoord1;
out vec3 v_directionToLight;
out vec3 v_directionToCamera;

void main(void){
   vec4 worldPos = model * vec4(i_position, 1.0);
   v_normal = u_normal_mat * i_normal;
   v_texcoord1 = i_texcoord1;

   vec3 vectorToLight = lightPos - worldPos.xyz;
   v_directionToLight = normalize( vectorToLight);
   v_directionToCamera = normalize( cameraPos - worldPos.xyz );

   gl_Position = viewProj * worldPos; 
}