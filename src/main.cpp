#include <iostream>
#include "utils.h"
#include "linalg.h"
#include "geometry.h"
#include "scene.h"
#include "rasterization.h"

class TestClass
{
public:
	int id;
	int id2;
	TestClass(int x)
	{
		id = x;
		id2 = x + 1;
		std::cout << "Created" << std::endl;
	}

	TestClass(const TestClass& self)
	{
		std::cout << "Copied" << std::endl;
		id = self.id + 100;
	}

	~TestClass()
	{
		std::cout << "Destroyed" << std::endl;
	}
};


int main()
{
	//std::vector<TestClass> v;
	//v.reserve(5);

	//v.emplace_back(1);
	//v.emplace_back(2);
	//v.emplace_back(3);
	//v.emplace_back(4);
	//v.emplace_back(5);
	//
	//TestClass vref = v[v.size() - 1];

	//std::cout << vref.id << " " << vref.id2 << std::endl;

	////for (TestClass& t : v)
	////{
	////	std::cout << t.id << std::endl;
	////}



	//std::cin.get();
	Scene scene;

	Object* object1 = scene.createEmptyObject();
	object1->name = "First object";

	Object* newObject = scene.createObjectFromFile("testing/model.obj");

	std::cout << object1->getChildren().size() << std::endl;
	object1->addChild(newObject);
	newObject->name = "model";



	Object* object2 = scene.createEmptyObject();
	object1->addChild(object2);


	std::cout << object1->getChildren().size() << std::endl;

	Object* object3 = scene.createEmptyObject();
	//newObject.addChild(object3);


	Camera* newCamera = scene.createCamera(80, 40, 90.0f, 0.01f, 100.0f);
	newCamera->translate(0, 0, -3.0f);
	//newObject.scale(0.2f);
	//newObject.translate(0, -5, 0);

	//std::cout << &newObject << std::endl;
	//newObject.mesh->print();

	scene.summary();

	std::cin.get();

	Rasterizer rast;
	rast.render(scene);
	rast.toDisplay();

	while (true)
	{
		//std::cin.get();
		rast.clearBuffers();
		//newObject.rotate(0, 1, 0);
		//newObject.translate(0.1f, 0, 0);
		rast.render(scene);
		rast.toDisplay();
		//scene.sceneObjects[0].transform.print();
	}

	std::cin.get();
	return 1;
}