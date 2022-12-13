#include "scene.h"
#include <numbers>

Object::Object()
{
	name = "Unnamed object";
	transform = createIdentityMatrix<float>(4);
	mesh = nullptr;
	parent = nullptr;

	//std::cout << "Object created" << std::endl;
}

//Object::Object(const Object& other)
//{
//	std::cout << "Copied" << std::endl;
//
//	name = other.name;
//	transform = other.transform;
//	mesh = other.mesh;
//	parent = other.parent;
//}

Object::~Object()
{
	//delete mesh;
	//std::cout << "Object destroyed" << std::endl;
}

void Object::transformByMatrix(Matrix<float>& transformation)
{
	transform = transformation.matmul(transform);
}

void Object::translate(float translateX, float translateY, float translateZ)
{
	Matrix<float> translationgMatrix = { {1, 0, 0, translateX},
										 {0, 1, 0, translateY},
										 {0, 0, 1, translateZ},
										 {0, 0, 0, 1} };
	transformByMatrix(translationgMatrix);
}

void Object::rotate(float rotateX, float rotateY, float rotateZ)
{

	rotateX *= std::numbers::pi / 180;
	rotateY *= std::numbers::pi / 180;
	rotateZ *= std::numbers::pi / 180;

	Matrix<float> rotX = {  {1, 0,	0, 0},
							{0, cos(rotateX), sin(rotateX), 0},
							{0, -sin(rotateX), cos(rotateX), 0},
							{0, 0, 0, 1} };

	Matrix<float> rotY = { {cos(rotateY), 0, -sin(rotateY), 0},
						   {0, 1, 0, 0},
						   {sin(rotateY), 0, cos(rotateY), 0},
						   {0, 0, 0, 1} };

	Matrix<float> rotZ = { {cos(rotateZ), -sin(rotateZ), 0, 0},
						   {sin(rotateZ), cos(rotateZ), 0, 0},
						   {0, 0, 1, 0},
						   {0, 0, 0, 1} };

	Matrix<float> rotationMatrix = rotY.matmul(rotX);
	rotationMatrix = rotZ.matmul(rotationMatrix);

	Vector<float> currentPos = getPosition();
	translate(-currentPos(0), -currentPos(1), -currentPos(2));


	transformByMatrix(rotationMatrix);

	translate(currentPos(0), currentPos(1), currentPos(2));
}

void Object::scale(float scaleX, float scaleY, float scaleZ)
{
	Matrix<float> scalingMatrix = { {scaleX, 0, 0, 0},
									{0, scaleY, 0, 0},
									{0, 0, scaleZ, 0},
									{0, 0, 0,      1} };
	transformByMatrix(scalingMatrix);

}

void Object::scale(float scalingFactor)
{
	Matrix<float> scalingMatrix = createIdentityMatrix<float>(4) * scalingFactor;
	scalingMatrix(3, 3) = 1;
	transformByMatrix(scalingMatrix);
}

Vector<float> Object::getPosition()
{
	Vector<float> out = { transform(0,3), transform(1,3), transform(2,3) };
	return out;
}

void Object::addChild(Object* newChild)
{
	std::cout << children.size() << std::endl;
	children.push_back(newChild);
	newChild->setParent(this);
}

void Object::removeChild(Object* child)
{
	for (std::vector<Object*>::iterator it = children.begin(); it != children.end(); it++)
	{
		if (*it = child)
		{
			children.erase(it);
		}
	}
}

std::vector<Object*> Object::getChildren()
{
	return children;
}

bool Object::hasParent()
{
	if (parent != nullptr)
		return true;
	return false;
}

void Object::setParent(Object* parent)
{
	this->parent = parent;
}


Object* Object::getParent()
{
	return parent;
}



Camera::Camera(int resolutionWidth, int resolutionHeight, float fov, float nearClipping, float farClipping)
	: Object(), resolutionWidth(resolutionWidth), resolutionHeight(resolutionHeight), fov(fov), nearClipping(nearClipping), farClipping(farClipping)
{
	name = "Unnamed camera";
}


void Scene::printChildrenNames(Object* object, int numTabs)
{
	std::vector<Object*> children = object->getChildren();
	for (int idx = 0; idx < children.size(); idx++)
	{
		Object* child = children[idx];

		if (idx == 0)
			std::cout << std::string(numTabs - 1, '\t') + "\u2A3D\t";
		else
			std::cout << std::string(numTabs, '\t');

		std::cout << child->name << std::endl;
		printChildrenNames(child, numTabs + 1);
	}
}

void Scene::deleteObject(Object* object)
{
	if (object->mesh != nullptr)
		delete object->mesh;

	if (object->hasParent())
	{
		Object* parent = object->getParent();
		parent->removeChild(object);
	}

	for (Object* child : object->getChildren())
	{
		// Apply parent transform such that child object stays in place
		child->transformByMatrix(object->transform);
		child->setParent(object->getParent());

	}
}

Scene::Scene()
{
	activeCamera = nullptr;
	camCounter = 0;
	objCounter = 0; 
}

Object* Scene::createObjectFromFile(const std::string& path)
{
	Object* newObject = createEmptyObject();
	Mesh* mesh = new Mesh(path);
	newObject->mesh = mesh;
	newObject->name = "Object " + std::to_string(objCounter);
	return newObject;
}

Object* Scene::createEmptyObject()
{
	objCounter++;
	Object* newObject = new Object();
	sceneObjects.push_back(newObject);
	newObject->name = "Empty object " + std::to_string(objCounter);
	return newObject;
}

void Scene::removeObject(Object* object)
{
	deleteObject(object);

	for (std::vector<Object*>::iterator it = sceneObjects.begin(); it != sceneObjects.end(); it++)
	{
		if (*it == object)
		{
			sceneObjects.erase(it);
			return;
		}
	}
}


Camera* Scene::createCamera(int resolutionWidth, int resolutionHeight, float fov, float nearClipping, float farClipping)
{
	camCounter++;
	Camera* newCamera = new Camera(resolutionWidth, resolutionHeight, fov, nearClipping, farClipping);
	sceneCameras.push_back(newCamera);
	newCamera->name = "Camera " + std::to_string(camCounter);
	if (activeCamera == nullptr)
		setActiveCamera(newCamera);
	return newCamera;
}

void Scene::removeCamera(Camera* camera)
{
	deleteObject(camera);
	for (std::vector<Camera*>::iterator it = sceneCameras.begin(); it != sceneCameras.end(); it++)
	{
		if (*it == camera)
		{
			sceneCameras.erase(it);
			return;
		}
	}
}


void Scene::setActiveCamera(Camera* camera)
{
	activeCamera = camera;
}

Camera* Scene::getActiveCamera()
{
	return activeCamera;
}

void Scene::summary()
{
	for (Object* object : sceneObjects)
	{
		if (!object->hasParent())
		{
			std::cout << object->name << std::endl;
			printChildrenNames(object, 1);
		}
	}
	
}

