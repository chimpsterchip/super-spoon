#include "HeightGenerator.h"

#include <math.h>
#include <random>
#include <functional>

HeightGenerator::HeightGenerator()
{
	//create a random seed for height map on first creation of object
	srand(time(NULL));
	seed = rand() % 1000000000;
}

float HeightGenerator::GenerateHeight(int x, int z)
{
	float total = 0;
	float d = (float)pow(2, octaves - 1);
	for (int i = 0; i < octaves; i++) {
		float freq = (float)(pow(2, i) / d);
		float amp = (float)pow(roughness, i) * amplitude;
		total += GetLerpNoise(x * freq, z * freq) * amp;
	}
	return total;
}

//generate random float between -1 and 1 as noise
float HeightGenerator::GetNoise(int x, int z)
{
	//seed needs to be the same whenever that vertex is called
	srand(x + z + seed);
	float random = ((float)rand()) / (float)RAND_MAX;
	//float difference = 1 - (-1);
	//difference always 2
	float r = random * 2;
	
	return -1 + r;
}

//gets random noise that takes into account neighbouring vectors
float HeightGenerator::GetSmoothNoise(int x, int z)
{
	float corners = (GetNoise(x - 1, z - 1) + GetNoise(x + 1, z - 1) + GetNoise(x - 1, z + 1) + GetNoise(x + 1, z + 1)) / 16.0f;
	float sides = (GetNoise(x - 1, z) + GetNoise(x + 1, z) + GetNoise(x, z - 1) + GetNoise(x, z + 1)) / 8.0f;
	float center = GetNoise(x, z) / 4.0f;
	return corners + sides + center;
}

float HeightGenerator::Lerp(float a, float b, float blend)
{
	//blend * pi
	double theta = blend * 3.14;
	float f = (float)(1.0f - cosf(theta)) * 0.5f;
	return a * (1.0f - f) + b * f;
}

float HeightGenerator::GetLerpNoise(float x, float z)
{
	int intX = (int)x;
	int intZ = (int)z;
	float fracX = x - intX;
	float fracZ = z - intZ;

	float v1 = GetSmoothNoise(intX, intZ);
	float v2 = GetSmoothNoise(intX + 1, intZ);
	float v3 = GetSmoothNoise(intX, intZ + 1);
	float v4 = GetSmoothNoise(intX + 1, intZ + 1);
	float i1 = Lerp(v1, v2, fracX);
	float i2 = Lerp(v3, v4, fracX);
	return Lerp(i1, i2, fracZ);
}
