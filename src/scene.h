#pragma once

#include <vector>
#include <string>
#include "rasterization.h"
#include "geometry.h"

class Camera : public Object
{
public:
	int resolutionWidth;
	int resolutionHeight;
	float fov;
	float nearClipping;
	float farClipping;

	Camera(int resolutionWidth, int resolutionHeight, float fov, float nearClipping, float farClipping);

};

class Scene
{
private:
	Camera* activeCamera;

public:
	std::vector<Object> sceneObjects;
	std::vector<Camera> sceneCameras;

	Scene();
	Object& createObjectFromFile(const std::string& path);
	Camera& createCamera(int resolutionWidth, int resolutionHeight, float fov, float nearClipping, float farClipping);
	void setActiveCamera(Camera& camera);
	void render(Rasterizer& rasterizer);
};


