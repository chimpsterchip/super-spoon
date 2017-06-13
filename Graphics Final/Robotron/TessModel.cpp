#include "TessModel.h"

TessModel::TessModel(GLuint _program, CCamera* _camera)
{

	m_Camera = _camera;
	program = _program;
	position = vec3(0.0f, 5.0f, 0.0f);
	GLfloat points[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f
	};
	glPatchParameteri(GL_PATCH_VERTICES, 4); //Comment out for tri patch

	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	glBindVertexArray(0);
}

TessModel::~TessModel()
{

}

void TessModel::render()
{
	glUseProgram(this->program);

	glm::mat4 model;
	model = glm::translate(model, position);
	glm::mat4 mvp = m_Camera->GetProjection() * m_Camera->GetView() * model;

	GLint mvLoc = glGetUniformLocation(program, "mvp");
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvp));

	vec3 distVec = m_Camera->GetPosition() - position;
	float dist = glm::length(distVec);

	GLint distLoc = glGetUniformLocation(program, "dist");
	glUniform1f(distLoc, dist);

	glBindVertexArray(vao);
	glDrawArrays(GL_PATCHES, 0, 4);
	glBindVertexArray(0);
}

void TessModel::setPosition(vec3 _NewPosition)
{
	position = _NewPosition;
}