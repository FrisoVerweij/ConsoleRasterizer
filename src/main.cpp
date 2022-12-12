#include <iostream>
#include "utils.h"
#include "linalg.h"
#include "geometry.h"
#include "scene.h"

int main()
{
	Scene scene;
	Object& newObject = scene.createObjectFromFile("testing/model.obj");
	Camera& newCamera = scene.createCamera(300, 150, 90.0f, 0.01f, 100.0f);
	newCamera.translate(0, 0, -3.0f);
	//newObject.translate(0, -5, 0);

	scene.summary();

	//std::cout << &newObject << std::endl;
	//newObject.mesh->print();

	//std::cin.get();

	Rasterizer rast;
	scene.render(rast);
	rast.toDisplay();

	while (true)
	{
		//std::cin.get();
		rast.clearBuffers();
		//newObject.translate(0.3f, 0, 0);
		newObject.rotate(0, 1, 0);
		scene.render(rast);
		rast.toDisplay();
		//scene.sceneObjects[0].transform.print();
	}

	std::cin.get();
	return 1;
}