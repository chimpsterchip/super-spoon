#pragma once
#ifndef HeightGenerator_H
#define HeightGenerator_H

#include <vector>
#include "Utils.h"

class HeightGenerator {
public:
	HeightGenerator();

private:
	float GenerateHeight(int x, int z);
	float GetNoise(int x, int z);
	float GetSmoothNoise(int x, int z);
	float Lerp(float a, float b, float blend);
	float GetLerpNoise(float x, float z);

private:
	std::vector<VertexFormat> verticesVector;

	int length = 500;
	int width = 500;
	int seed; //random seed generated at start to 
	int octaves = 3;
	float roughness = 0.3f;
	float amplitude = 70.0f;// how high or low terrain can be

};

#endif // !HeightGenerator_H
