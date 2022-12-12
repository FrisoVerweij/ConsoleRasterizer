#include "scene.h"
#include <numbers>

Object::Object()
{
	transform = createIdentityMatrix<float>(4);
	mesh = nullptr;
	parent = false;
}

Object::~Object()
{
	delete mesh;
}

void Object::render(Matrix<float>& toCamera, Matrix<float> toWorld, Rasterizer& rasterizer)
{
	//std::cout << "Rendering object" << std::endl;
	toWorld = toWorld.matmul(transform);

	if (mesh != nullptr)
	{
		rasterizer.rasterizeMesh(toCamera, toWorld, *mesh);
	}

	for (Object* child : children)
	{
		child->render(toCamera, toWorld, rasterizer);
	}
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
	transformByMatrix(rotationMatrix);
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
	transformByMatrix(scalingMatrix);
}


void Object::addChild(Object& newChild)
{
	children.push_back(&newChild);
	parent = true;
}

std::vector<Object*> Object::getChildren()
{
	return children;
}

bool Object::hasParent()
{
	return parent;
}



Camera::Camera(int resolutionWidth, int resolutionHeight, float fov, float nearClipping, float farClipping)
	: Object(), resolutionWidth(resolutionWidth), resolutionHeight(resolutionHeight), fov(fov), nearClipping(nearClipping), farClipping(farClipping)
{}

Scene::Scene()
{
	activeCamera = nullptr;
}

Object& Scene::createObjectFromFile(const std::string& path)
{
	sceneObjects.emplace_back();
	Object& newObject = sceneObjects[sceneObjects.size() - 1];
	Mesh* mesh = new Mesh(path);
	newObject.mesh = mesh;

	std::cout << &newObject << std::endl;
	return newObject;
}

Camera& Scene::createCamera(int resolutionWidth, int resolutionHeight, float fov, float nearClipping, float farClipping)
{
	sceneCameras.emplace_back(resolutionWidth, resolutionHeight, fov, nearClipping, farClipping);
	Camera& ref = sceneCameras[sceneCameras.size() - 1];
	if (activeCamera == nullptr)
		setActiveCamera(ref);
	return ref;
}

void Scene::setActiveCamera(Camera& camera)
{
	activeCamera = &camera;
}

void Scene::render(Rasterizer& rasterizer)
{
	//std::cout << "Rendering scene" << std::endl;
	if (activeCamera == nullptr)
	{
		std::cout << "Scene: No active camera!" << std::endl;
		return;
	}

	Matrix<float> toWorld = createIdentityMatrix<float>(4);
	Matrix<float> toCamera = activeCamera->transform.inverse();
	rasterizer.setCameraSettings(activeCamera->resolutionWidth, activeCamera->resolutionHeight, activeCamera->fov,
		activeCamera->nearClipping, activeCamera->farClipping);

	// Start rendering the objects in the scene without parent, or in other words, root objects
	for (Object& obj : sceneObjects)
	{
		//std::cout << obj.hasParent() << std::endl;

		if (!obj.hasParent())
			obj.render(toCamera, toWorld, rasterizer);
	}
}

void Scene::summary()
{
	std::cout << "Scene summary\nNum objects: " << sceneObjects.size() << "\nNum cameras: " << sceneCameras.size() << std::endl;
	
}

