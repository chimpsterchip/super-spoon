#pragma once
#ifndef ToonShadedModel_H
#define ToonShadedModel_H

#include "GameScene.h"

using namespace glm;

class ToonShadedModel {

public:
	ToonShadedModel(GLuint program, CCamera* camera, CLight* light, vec3 pos);
	ToonShadedModel();
	void init(ModelType type, float modelScale);
	void render();

	CLight* light;
	CCamera* camera;

	GLuint vao, vbo, ebo, program;
	GLsizei numVertices;
	vec3 pos;

	ModelType modelType;
	float modelScale;

	static bool InitDone;

private:


};

#endif // !ToonShadedModel_H
