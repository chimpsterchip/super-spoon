#include "ToonShadedModel.h"
#include "Utils.h"

bool ToonShadedModel::InitDone = false;

ToonShadedModel::ToonShadedModel(GLuint program, CCamera * camera, CLight * light, vec3 pos)
{
	this->program = program;
	this->camera = camera;
	this->light = light;
	this->pos = pos;
}

ToonShadedModel::ToonShadedModel()
{
}

void ToonShadedModel::init(ModelType type, float modelScale)
{
	//Set member variables
	modelScale = modelScale;
	modelType = type;

	if (!InitDone)
	{
		glEnable(GL_DEPTH_TEST);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // GL_FILL, GL_LINE
	}
	//Remember to keep the size the same across classes
	GLfloat vertices[1000];
	GLuint indices[690];
	//Set vertices in CUtils
	CUtils::SetVertices(type, modelScale);

	//Get the vertices from CUtils and put into a constant array
	GLfloat* pTemp = CUtils::GetVertices();
	for (int i = 0; i < CUtils::GetVerticesSize(); ++i)
	{
		vertices[i] = *pTemp;
		pTemp++;
	}

	//Get the indices from CUtils and put into a constant array
	GLuint* pTemp2 = CUtils::GetIndices();
	for (int j = 0; j < CUtils::GetIndicesSize(); ++j)
	{
		indices[j] = *pTemp2;
		pTemp2++;
	}

	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	if (InitDone == false)
		InitDone = true;

}

void ToonShadedModel::render()
{
	//glClearColor(0.0f, 0.6f, 1.0f, 1.0f);

	//Shader to use
	glUseProgram(program);

	glBindVertexArray(vao);

	vec3 LightPos = GameScene::GetInstance().GetLight()->GetPosition();
	vec3 LightColor = GameScene::GetInstance().GetLight()->GetColor();

	//lighting uniform Variables
	glUniform3f(glGetUniformLocation(program, "lightDir"), LightPos.x, LightPos.y, LightPos.z);

	//Translating the model
	GLint modelLoc = glGetUniformLocation(program, "model");

	glm::mat4 model;


	//Finish up render function
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, numVertices, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	/*glm::mat4 view;
	glm::mat4 projection;

	view = camera->GetView();

	projection = glm::perspective(45.0f, (GLfloat)800 / (GLfloat)800, 0.1f, 100.0f);

	GLint viewLoc = glGetUniformLocation(program, "view");
	GLint projLoc = glGetUniformLocation(program, "projection");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));*/

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
}
