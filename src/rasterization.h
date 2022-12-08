#pragma once
#include <vector>
#include "linalg.h"
//#include "scene.h"

class RenderBuffers
{
public:
	int width, height;
	std::vector<float> frameBuffer;
	std::vector<float> zBuffer;
	RenderBuffers(int width, int height);
};

class Rasterizer
{
public:
	int imageWidth, imageHeight;
	RenderBuffers* buffers;
	Matrix<float> projectionMatrix;

	Rasterizer();
	//Rasterizer(Camera& camera);
	//void linkCamera(Camera& camera);
	Matrix<float> makeProjectionMatrix(float aspect, float fov, float zNear, float zFar);

};