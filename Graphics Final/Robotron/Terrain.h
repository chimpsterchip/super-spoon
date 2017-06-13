#pragma once
#include "Utils.h"
//#include "HeightGenerator.h"

class CLight;
class CCamera;
class HeightGenerator;

class CTerrain
{
public:
	CTerrain(std::wstring HeightmapFilename, std::string texFileName, std::string texFileName1, std::string texFileName2, GLuint program, CCamera* camera, CLight* light, HeightGenerator* hGenerator);
	~CTerrain();
	
	CLight* light;
	CCamera* camera;
	HeightGenerator* heightGenerator;

	std::string texFileName;
	std::string texFileName1;
	std::string texFileName2;

	GLuint mNumVertices;
	GLuint mNumFaces;

	GLuint numRows = 20, numCols = 20;
	GLfloat heightScale = 0.35f, heightOffset = 0.0f, cellSpacing = 12.0f;
	std::vector<float> heightmap;
	std::vector<VertexFormat>vertices;
	std::vector<GLuint>indices;
	std::wstring heightmapFilename;

	GLuint vao, vbo, ebo, program, texture, texture1, texture2;

	void loadHeightMap();
	void generateHeightMap();
	void smooth();
	float average(UINT i, UINT j);
	void createVerticesAndIndices();
	void buildBuffers();
	GLuint setTexture(std::string  texFileName);
	void render();
};