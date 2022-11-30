#include <iostream>
#include <fstream>
#include <type_traits>
#include "linalg.h"
#include "utils.h"
#include "geometry.h"

template <typename T>
std::vector<T> Model::splitObjEntry(std::string entry, std::string delimiter, int numElements)
{
	std::vector<T> values;
	values.reserve(numElements);
	for (int i = 0; i < numElements; i++)
	{
		size_t idx = entry.find_first_of(delimiter);

		T element;
		if (std::is_same<T, float>::value)
			element = std::stof(entry.substr(0, idx));
		else if (std::is_same<T, double>::value)
			element = std::stod(entry.substr(0, idx));
		else if (std::is_same<T, int>::value)
			element = std::stoi(entry.substr(0, idx));
		else if (std::is_same<T, long>::value)
			element = std::stol(entry.substr(0, idx));

		values.push_back(element);
		if (idx != std::string::npos)
			entry.erase(0, idx + delimiter.length());
	}
	return values;
}


void Model::readOBJ(std::string path)
{
	std::ifstream file(path);

	int numVerts = 0;
	int numTris = 0;

	std::string line;

	// Preprocess
	while (std::getline(file, line))
	{
		std::string tag = line.substr(0, line.find_first_of(' '));
		if (tag == "v")
			numVerts++;
		else if (tag == "f") // Cannot handle n-gons
			numTris++;
	}

	vertices->reserve(numVerts);
	vertexNormals->reserve(numVerts);
	textureCoordinates->reserve(numVerts);
	triangles->reserve(numTris);

	file.clear();
	file.seekg(0);

	// Fill in data
	while (std::getline(file, line))
	{
		int splitIdx = (int)line.find_first_of(' ');
		std::string tag = line.substr(0, splitIdx);
		if (tag == "v")
			vertices->emplace_back(splitObjEntry<float>(line, " ", 3));
		else if (tag == "vn")
			vertexNormals->emplace_back(splitObjEntry<float>(line, " ", 3));
		else if (tag == "vt")
			textureCoordinates->emplace_back(splitObjEntry<float>(line, " ", 2));
		else if (tag == "f") // Cannot handle n-gons
			triangles->emplace_back(splitObjEntry<int>(line, "/", 3));
	}
	file.close();
}


Model::Model(std::string path)
{
	// Allocate data on stack as their sizes can become large
	vertices = new std::vector<Vector<float>>;
	std::vector<Vector<float>>* vertexNormals = new std::vector<Vector<float>>;
	std::vector<Vector<float>>* textureCoordinates = new std::vector<Vector<float>>;
	std::vector<Vector<int>>* triangles = new std::vector<Vector<int>>;

	// Read file
	std::string extension = path.substr(path.find_last_of("."));
	if (extension == ".obj")
		readOBJ(path);
	else
		throw std::invalid_argument(std::string("File of type '") + extension + "' is not supported.");

}

Model::~Model()
{
	delete vertices;
	delete vertexNormals;
	delete textureCoordinates;
	delete triangles;
}