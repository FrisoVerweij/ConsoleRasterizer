#pragma once

#include <vector>
#include <string>
#include "geometry.h"


class Object
{
private:
	std::vector<Object*> children;
	bool parent;

public:
	Matrix<float> transform;
	Mesh* mesh;

	Object();
	~Object();
	void transformByMatrix(Matrix<float>& transformation);
	void translate(float translateX, float translateY, float translateZ);
	void rotate(float rotateX, float rotateY, float rotateZ);
	void scale(float scaleX, float scaleY, float scaleZ);
	void scale(float scalingFactor);

	void addChild(Object& newChild);
	std::vector<Object*> getChildren();
	bool hasParent();
};

class Pointlight : Object
{

};

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
	Camera* getActiveCamera();
	void summary();
};


