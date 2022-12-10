#pragma once
#include <vector>
#include "linalg.h"

class RenderBuffers
{
public:
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
	Rasterizer(int imageWidth, int imageHeight, float fov, float nearClipping, float farClipping);
	void setCameraSettings(int imageWidth, int imageHeight, float fov, float nearClipping, float farClipping);
	Matrix<float> makeProjectionMatrix(float aspect, float fov, float zNear, float zFar);

};