#pragma once
#include <vector>
#include "linalg.h"
#include "geometry.h"

class RenderBuffers
{
public:
	std::vector<float> frameBuffer;
	std::vector<float> zBuffer;
	RenderBuffers(int width, int height);
};

class Rasterizer
{
private:
	void clearDisplay();
	std::vector<float> grayscaleBuffer();
	float grayscale(float red, float green, float blue);
	char getCharFromIntensity(float intensity);
	void rasterizeTriangle(Matrix<float>& toCamera, Matrix<float> toWorld, Triangle& triangle);

public:
	int imageWidth, imageHeight;
	RenderBuffers* buffers;
	Matrix<float> projectionMatrix;

	Rasterizer();
	Rasterizer(int imageWidth, int imageHeight, float fov, float nearClipping, float farClipping);
	void setCameraSettings(int imageWidth, int imageHeight, float fov, float nearClipping, float farClipping);
	Matrix<float> makeProjectionMatrix(float aspect, float fov, float zNear, float zFar);
	void toDisplay();
	void clearBuffers();

	void rasterizeMesh(Matrix<float>& toCamera, Matrix<float> toWorld, Mesh& mesh);
};