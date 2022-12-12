#include "rasterization.h"

RenderBuffers::RenderBuffers(int width, int height)
	{
		frameBuffer = std::vector<float>(height * width * 3, 0.0f);
		zBuffer = std::vector<float>(height * width, 100000.0f);
	}

void Rasterizer::clearDisplay()
{
	std::cout << "\x1B[2J\x1B[H";
}

std::vector<float> Rasterizer::grayscaleBuffer()
{
	std::vector<float> output;
	output.reserve(imageHeight * imageWidth);
	for (int idx = 0; idx < output.capacity(); idx++)
	{
		float red   = buffers->frameBuffer[idx * 3];
		float green = buffers->frameBuffer[idx * 3 + 1];
		float blue  = buffers->frameBuffer[idx * 3 + 2];
		output.push_back(grayscale(red, green, blue));
	}
	return output;
}

float Rasterizer::grayscale(float red, float green, float blue)
{
	return 0.299f * red + 0.587f * green + 0.114f * blue;
}

char Rasterizer::getCharFromIntensity(float intensity)
{
	if (intensity > 0)
		return 'O';
	else
		return ' ';
}

void Rasterizer::rasterizeTriangle(Matrix<float>& toCamera, Matrix<float> toWorld, Triangle& triangle)
{
	//std::cout << "t" << std::endl;
	for (Vertex vertex : triangle.vertices)
	{
		Vector<float> temp = { vertex.position()(0) , vertex.position()(1) , vertex.position()(2), 1.0f };
		temp = toWorld.matmul(temp);
		temp = toCamera.matmul(temp);
		Vector<float> projected = projectionMatrix.matmul(temp);
		projected(0) /= projected(2);
		projected(1) /= projected(2);
		int xPixel = (projected(0) + 1) / 2 * imageWidth;
		int yPixel = (projected(1) + 1) / 2 * imageHeight * 0.65f;
		buffers->frameBuffer[(yPixel * imageWidth + xPixel) * 3] = 1.0f;
		buffers->frameBuffer[(yPixel * imageWidth + xPixel) * 3 + 1] = 1.0f;
		buffers->frameBuffer[(yPixel * imageWidth + xPixel) * 3 + 2] = 1.0f;
	}
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

void Rasterizer::toDisplay()
{
	clearDisplay();
	std::vector<float> buffer = grayscaleBuffer();
	
	for (int y = 0; y < imageHeight; y++)
	{
		for (int x = 0; x < imageWidth; x++)
		{
			char pixel = getCharFromIntensity(buffer[y * imageWidth + x]);
			putchar(pixel);
		}
		putchar('\n');
	}
}

void Rasterizer::clearBuffers()
{
	for (int idx = 0; idx < buffers->frameBuffer.size(); idx++)
		buffers->frameBuffer[idx] = 0;

	for (int idx = 0; idx < buffers->zBuffer.size(); idx++)
		buffers->zBuffer[idx] = 100000.0f;
}


void Rasterizer::rasterizeMesh(Matrix<float>& toCamera, Matrix<float> toWorld, Mesh& mesh)
{
	std::cout << "mesh" << std::endl;
	for (Triangle triangle : mesh.triangles)
		rasterizeTriangle(toCamera, toWorld, triangle);
}