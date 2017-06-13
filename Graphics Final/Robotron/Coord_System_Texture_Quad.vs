#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 normvec;
 
out vec3 outColor;
out vec2 TexCoord;
out vec3 NormVec;
out vec3 fragPosition;
out vec4 worldPos;

//uniform mat4 transform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(void)
{
	worldPos = model * vec4(position, 1.0f);
	gl_Position = projection * view * worldPos;
	//gl_Position = projection * view * model * vec4(position, 1.0f);
	outColor = color;
	fragPosition = vec3(model * vec4(position, 1.0f));
	NormVec = mat3(transpose(inverse(model))) * normvec;
	TexCoord = vec2(texCoord.x, texCoord.y);
}