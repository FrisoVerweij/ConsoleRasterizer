#pragma once
#include <string>
#include "linalg.h"


class Model
{
private:

	template <typename T>
	std::vector<T> splitObjEntry(std::string entry, std::string delimiter, int numElements);
	void readOBJ(std::string path);

public:
	std::vector<Vector<float>>* vertices;
	std::vector<Vector<float>>* vertexNormals;
	std::vector<Vector<float>>* textureCoordinates;
	std::vector<Vector<int>>* triangles;

	Model(std::string path);
	~Model();
};

