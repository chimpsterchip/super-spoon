#version 330 core

verying vec3 normal;

void main()
{
	normal = gl_NormalMatrix * gl_Normal;
	gl_Position = ftransform();
}