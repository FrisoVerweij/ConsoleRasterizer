#include <iostream>
#include <fstream>
#include <type_traits>
#include "linalg.h"
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
	// Allocate data on stack as their sizes can become large
	//vertexPositions = new std::vector<Vector<float>>;
	//vertexNormals = new std::vector<Vector<float>>;
	//textureCoordinates = new std::vector<Vector<float>>;
	//triangles = new std::vector<Triangle>;

	// Read file
	std::string extension = path.substr(path.find_last_of("."));
	if (extension == ".obj")
		readOBJ(path);
	else
		throw std::invalid_argument(std::string("File of type '") + extension + "' is not supported.");
}

Mesh::~Mesh()
{}

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

	std::cout << "\n\Triangles:" << std::endl;
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



Object::Object()
{
	transform = createIdentityMatrix<float>(4);
	mesh = nullptr;
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
	// Not yet implemented
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
}

std::vector<Object*>& Object::getChildren()
{
	return children;
}

Object createObjectFromFile(std::string path) 
{
	Object newObject;
	Mesh* mesh = new Mesh(path);
	newObject.mesh = mesh;
	return newObject;
}