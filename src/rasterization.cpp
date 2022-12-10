#include "rasterization.h"

RenderBuffers::RenderBuffers(int width, int height)
	{
		frameBuffer = std::vector<float>(height * width * 3, 0.0f);
		zBuffer = std::vector<float>(height * width, 100000.0f);
	}

Rasterizer::Rasterizer()
{
	buffers = nullptr;
	imageWidth = 0;
	imageHeight = 0;
}

Rasterizer::Rasterizer(int imageWidth, int imageHeight, float fov, float nearClipping, float farClipping)
{
	setCameraSettings(imageWidth, imageHeight, fov, nearClipping, farClipping);
}

void Rasterizer::setCameraSettings(int imageWidth, int imageHeight, float fov, float nearClipping, float farClipping)
{
	this->imageWidth = imageWidth;
	this->imageHeight = imageHeight;
	buffers = new RenderBuffers(imageWidth, imageHeight);
	projectionMatrix = makeProjectionMatrix((float)imageHeight / (float)imageWidth, fov, nearClipping, farClipping);
}

Matrix<float> Rasterizer::makeProjectionMatrix(float aspect, float fov, float zNear, float zFar)
{
	projectionMatrix = createEmptyMatrix<float>(4, 4);
	projectionMatrix(0, 0) = aspect * (1 / tan(fov / 2));
	projectionMatrix(1, 1) = 1 / tan(fov / 2);
	projectionMatrix(2, 2) = zFar / (zFar - zNear);
	projectionMatrix(2, 3) = (-zFar * zNear) / (zFar - zNear);
	projectionMatrix(3, 2) = 1.0f;
	return projectionMatrix;
}