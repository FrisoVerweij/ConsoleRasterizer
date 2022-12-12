#include <iostream>
#include "utils.h"
#include "linalg.h"
#include "geometry.h"
#include "scene.h"
#include "rasterization.h"

int main()
{
	Scene scene;
	Object& newObject = scene.createObjectFromFile("testing/model.obj");
	Camera& newCamera = scene.createCamera(80, 40, 90.0f, 0.01f, 100.0f);
	newCamera.translate(0, 0, -3.0f);
	//newObject.scale(0.2f);
	//newObject.translate(0, -5, 0);

	//std::cout << &newObject << std::endl;
	//newObject.mesh->print();

	//std::cin.get();

	Rasterizer rast;
	rast.render(scene);
	rast.toDisplay();

	while (true)
	{
		//std::cin.get();
		rast.clearBuffers();
		//newObject.translate(0.3f, 0, 0);
		newObject.rotate(0, 1, 0);
		rast.render(scene);
		rast.toDisplay();
		scene.sceneObjects[0].transform.print();
	}

	std::cin.get();
	return 1;
}