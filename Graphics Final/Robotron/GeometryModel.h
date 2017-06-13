#pragma once

#include "Utils.h"
#include "Camera.h"

class GeometryModel
{
public:
	GeometryModel(GLuint program, CCamera* camera);
	void render();

	void setPosition(glm::vec3 _Position);

private:
	CCamera* Camera;
	GLuint program;
	GLuint vao, vbo, ebo;
	vec3 position;
};