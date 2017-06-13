#pragma once

#include "Utils.h"
#include "Camera.h"

class TessModel
{
public:
	TessModel(GLuint program, CCamera* camera);
	~TessModel();

	void render();
	void setPosition(vec3 _NewPosition);
private:
	CCamera* m_Camera;
	GLuint program;
	GLuint vao, vbo, ebo;
	vec3 position;
	
};