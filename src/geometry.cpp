#include <iostream>
#include <fstream>
#include "utils.h"
#include "geometry.h"


Vertex::Vertex(Vector<float>* positionPtr, Vector<float>* textureCoordinatePtr, Vector<float>* normalPtr)
	: positionPtr(positionPtr), texCoordsPtr(textureCoordinatePtr), normalPtr(normalPtr)
{}

Vector<float>& Vertex::position()
{
	return *positionPtr;
}

Vector<float>& Vertex::texCoords()
{
	return *texCoordsPtr;
}

Vector<float>& Vertex::normal()
{
	return *normalPtr;
}

Triangle::Triangle()
{
	vertices.reserve(3);
}


Mesh::Mesh(std::string path)
{
	// Read file
	std::string extension = path.substr(path.find_last_of("."));
	if (extension == ".obj")
		readOBJ(path);
	else
		throw std::invalid_argument(std::string("File of type '") + extension + "' is not supported.");
}

Mesh::~Mesh()
{}

//void Mesh::renderMesh(Matrix<float>& toCamera, Matrix<float> toWorld, Rasterizer& rasterizer)
//{
//	// Actual rendering
//}

void Mesh::print()
{
	std::cout << "Vertex positions:" << std::endl;
	for (Vector<float> v : vertexPositions)
		v.transpose().print();

	std::cout << "\nTexture coordinates:" << std::endl;
	for (Vector<float> v : textureCoordinates)
		v.transpose().print();

	std::cout << "\nVertex normals:" << std::endl;
	for (Vector<float> v : vertexNormals)
		v.transpose().print();

	std::cout << "\nTriangles:" << std::endl;
	for (int i = 0; i < triangles.size(); i++)
	{
		Triangle t = triangles[i];
		std::cout << i << std::endl;
		for (Vertex v : t.vertices)
			v.position().transpose().print();
	}
}

void Mesh::readOBJ(const std::string& path)
{
	int numVerts = 0;
	int numTexCoords = 0;
	int numNorms = 0;
	int numTris = 0;

	std::ifstream file(path);
	std::string line;

	// Preprocess
	while (std::getline(file, line))
	{
		std::string tag = line.substr(0, line.find_first_of(' '));
		if (tag == "v")
			numVerts++;
		else if (tag == "vt")
			numTexCoords++;
		else if (tag == "vn")
			numNorms++;
		else if (tag == "f") // Cannot handle n-gons
			numTris++;
	}

	vertexPositions.reserve(numVerts);
	vertexNormals.reserve(numNorms);
	textureCoordinates.reserve(numTexCoords);
	triangles.reserve(numTris);

	file.clear();
	file.seekg(0);

	// Fill in data
	while (std::getline(file, line))
	{
		int splitIdx = (int)line.find_first_of(' ');
		std::string tag = line.substr(0, splitIdx);
		line.erase(0, splitIdx + 1);
		std::vector<std::string> elements = splitString(line, " ");

		if (tag == "v")
			vertexPositions.emplace_back(vectorStof(elements));
		else if (tag == "vn")
			vertexNormals.emplace_back(vectorStof(elements));
		else if (tag == "vt")
			textureCoordinates.emplace_back(vectorStof(elements));
		else if (tag == "f") // Cannot handle n-gons
		{
			Triangle triangle;

			for (std::string element : elements)
			{
				Vector<float>* posPtr, *texPtr, *normPtr;

				std::vector<std::string> indices = splitString(element, "/");

				posPtr = &(vertexPositions[std::stoi(indices[0]) - 1]);
				texPtr = indices.size() > 1 && indices[1] != "" ? &(textureCoordinates[std::stoi(indices[1]) - 1]) : nullptr;
				normPtr = indices.size() > 2 ? &(vertexNormals[std::stoi(indices[2]) - 1]) : nullptr;

				triangle.vertices.emplace_back(posPtr, texPtr, normPtr);
			}
			triangles.push_back(triangle);
		}
	}

	// TODO: Read material file

	file.close();
}