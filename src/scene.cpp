#include "scene.h"

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

void Scene::render(Rasterizer& rasterizer) // Change return type
{
	if (activeCamera == nullptr)
	{
		std::cout << "Scene: No active camera!" << std::endl;
		return;
	}

	Matrix<float> toWorld = createIdentityMatrix<float>(4);
	Matrix<float> toCamera = activeCamera->transform.inverse();
	//rasterizer.linkCamera(*activeCamera);

	// Start rendering the objects in the scene without parent, or in other words, root objects
	for (Object obj : sceneObjects)
	{
		if (!obj.hasParent())
			obj.render(toCamera, toWorld, rasterizer);
	}
}

