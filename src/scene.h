#pragma once

#include <vector>
#include <string>
#include "geometry.h"


class Object
{
private:
	std::vector<Object*> children;
	Object* parent;

public:
	std::string name;
	Matrix<float> transform;
	Mesh* mesh;

	Object();
	//Object(const Object& other);
	~Object();
	void transformByMatrix(Matrix<float>& transformation);
	void translate(float translateX, float translateY, float translateZ);
	void rotate(float rotateX, float rotateY, float rotateZ);
	void scale(float scaleX, float scaleY, float scaleZ);
	void scale(float scalingFactor);
	Vector<float> getPosition();

	void addChild(Object& newChild);
	void removeChild(Object& child);
	std::vector<Object*> getChildren();
	bool hasParent();
	void setParent(Object* parent);
	Object* getParent();
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
	int camCounter;
	int objCounter;

	void printChildrenNames(Object& object, int numTabs);
	void deleteObject(Object& object);

public:
	std::vector<Object> sceneObjects;
	std::vector<Camera> sceneCameras;

	Scene();
	Object& createObjectFromFile(const std::string& path);
	Object& createEmptyObject();
	void removeObject(Object& object);
	Camera& createCamera(int resolutionWidth, int resolutionHeight, float fov, float nearClipping, float farClipping);
	void removeCamera(Camera& camera);
	void setActiveCamera(Camera& camera);
	Camera* getActiveCamera();
	void summary();
};


