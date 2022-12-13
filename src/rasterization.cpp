#include "rasterization.h"
#include <iostream>
#include <thread>
#include <Windows.h>

RenderBuffers::RenderBuffers(int width, int height)
	{
		frameBuffer = std::vector<float>(height * width * 3, 0.0f);
		zBuffer = std::vector<float>(height * width, 100000.0f);
	}

void Rasterizer::clearDisplay()
{
	HANDLE hStdout;
	COORD destCoord;
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	//position cursor at start of window
	destCoord.X = 0;
	destCoord.Y = 0;
	SetConsoleCursorPosition(hStdout, destCoord);
	//std::cout << "\x1B[2J\x1B[H";
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
		projected(0) /= projected(3);
		projected(1) /= projected(3);
		projected(2) /= projected(3);
		// Now in image space

		// Placeholder \/
		int xPixel = (projected(0) + 1) / 2 * imageWidth;
		int yPixel = (projected(1) + 1) / 2 * imageHeight * 0.65f;
		buffers->frameBuffer[(yPixel * imageWidth + xPixel) * 3] = 1.0f;
		buffers->frameBuffer[(yPixel * imageWidth + xPixel) * 3 + 1] = 1.0f;
		buffers->frameBuffer[(yPixel * imageWidth + xPixel) * 3 + 2] = 1.0f;
	}
}

void Rasterizer::rasterizeMesh(Matrix<float>& toCamera, Matrix<float> toWorld, Mesh* mesh)
{
	//std::cout << "mesh" << std::endl;
	for (Triangle& triangle : mesh->triangles)
		rasterizeTriangle(toCamera, toWorld, triangle);
}

void Rasterizer::renderObject(Matrix<float>& toCamera, Matrix<float> toWorld, Object& object)
{
	//std::cout << "Rendering object" << std::endl;
	toWorld = toWorld.matmul(object.transform);

	if (object.mesh != nullptr)
	{
		rasterizeMesh(toCamera, toWorld, object.mesh);
	}

	for (Object* child : object.getChildren())
	{
		renderObject(toCamera, toWorld, *child);
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

Rasterizer::~Rasterizer()
{
	if (buffers != nullptr)
		delete buffers;
}

void Rasterizer::setCameraSettings(int imageWidth, int imageHeight, float fov, float nearClipping, float farClipping)
{
	this->imageWidth = imageWidth;
	this->imageHeight = imageHeight;
	if (buffers != nullptr)
		delete buffers;

	buffers = new RenderBuffers(imageWidth, imageHeight);
	projectionMatrix = makeProjectionMatrix((float)imageHeight / (float)imageWidth, fov, nearClipping, farClipping);
}

Matrix<float> Rasterizer::makeProjectionMatrix(float aspect, float fov, float zNear, float zFar)
{
	projectionMatrix = createEmptyMatrix<float>(4, 4);
	projectionMatrix(0, 0) = aspect / tan(fov / 2);
	projectionMatrix(1, 1) = 1 / tan(fov / 2);
	projectionMatrix(2, 2) = zFar / (zFar - zNear);
	projectionMatrix(2, 3) = (-zFar * zNear) / (zFar - zNear);
	projectionMatrix(3, 2) = 1.0f;
	return projectionMatrix;
}

void Rasterizer::toDisplay()
{
	std::vector<float> buffer = grayscaleBuffer();

	std::string output = "";
	
	for (int y = 0; y < imageHeight; y++)
	{
		for (int x = 0; x < imageWidth; x++)
		{
			char pixel = getCharFromIntensity(buffer[y * imageWidth + x]);
			output += pixel;
		}
		if (y != imageHeight - 1)
			output += '\n';
	}
	clearDisplay();
	std::cout << output;
	using namespace std::literals;
	std::this_thread::sleep_for(100ms);
}

void Rasterizer::clearBuffers()
{
	for (int idx = 0; idx < buffers->frameBuffer.size(); idx++)
		buffers->frameBuffer[idx] = 0;

	for (int idx = 0; idx < buffers->zBuffer.size(); idx++)
		buffers->zBuffer[idx] = 100000.0f;
}


void Rasterizer::render(Scene& scene)
{
	Camera* activeCamera = scene.getActiveCamera();
	if (activeCamera == nullptr)
	{
		std::cout << "Scene: No active camera!" << std::endl;
		return;
	}

	Matrix<float> toWorld = createIdentityMatrix<float>(4);
	Matrix<float> toCamera = activeCamera->transform.inverse();
	setCameraSettings(activeCamera->resolutionWidth, activeCamera->resolutionHeight, activeCamera->fov,
		activeCamera->nearClipping, activeCamera->farClipping);

	// Start rendering the objects in the scene without parent, or in other words, root objects
	for (Object& obj : scene.sceneObjects)
	{
		if (!obj.hasParent())
			renderObject(toCamera, toWorld, obj);
	}
}
