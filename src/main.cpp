#include <iostream>
#include "utils.h"
#include "linalg.h"
#include "geometry.h"
#include "scene.h"
#include "rasterization.h"


int main()
{
	Scene scene;

	Object* top = scene.createEmptyObject();
	top->name = "top";
	top->rotate(0, 0, 45);

	Object* object1 = scene.createEmptyObject();
	object1->name = "First object";
	object1->rotate(0, 45, 0);
	top->addChild(object1);

	Object* newObject = scene.createObjectFromFile("testing/model.obj");

	object1->addChild(newObject);
	newObject->name = "model";



	Object* object2 = scene.createEmptyObject();
	object1->addChild(object2);
	object2->name = "Empty object 2";


	Object* object3 = scene.createEmptyObject();
	newObject->addChild(object3);
	object3->name = "Empty object 3";


	Camera* newCamera = scene.createCamera(80, 40, 90.0f, 0.01f, 100.0f);
	newCamera->translate(0, 0, -3.0f);
	//newObject.scale(0.2f);
	//newObject.translate(0, -5, 0);

	//std::cout << &newObject << std::endl;
	//newObject.mesh->print();

	scene.summary();

	std::cin.get();

	//object1->removeChild(object2);
	//scene.summary();
	//std::cin.get();

	//scene.removeObject(newObject);

	//scene.summary();
	//std::cin.get();


	//object3->addChild(object2);

	//scene.summary();
	//std::cin.get();

	//newObject = scene.createObjectFromFile("testing/model.obj");
	//newObject->name = "boss";

	//newObject->addChild(object1);


	//scene.summary();
	//std::cin.get();


	Rasterizer rast;
	rast.render(scene);
	rast.toDisplay();


	std::cin.get();

	object1->removeChild(newObject);
	//scene.removeObject(object1);

	rast.clearBuffers();
	rast.render(scene);
	rast.toDisplay();
	std::cin.get();



	while (true)
	{
		//std::cin.get();
		rast.clearBuffers();
		//newObject->rotate(0, 1, 0);
		//newObject.translate(0.1f, 0, 0);
		rast.render(scene);
		rast.toDisplay();
		//scene.sceneObjects[0].transform.print();
	}

	std::cin.get();
	return 1;
}