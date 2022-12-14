#pragma once
#include <vector>
#include <string>
#include "linalg.h"

class Vertex
{
private:
	Vector<float>* positionPtr;
	Vector<float>* texCoordsPtr;
	Vector<float>* normalPtr;

public:
	Vertex(Vector<float>* positionPtr, Vector<float>* textureCoordinatePtr, Vector<float>* normalPtr);

	Vector<float>& position();
	Vector<float>& texCoords();
	Vector<float>& normal();
};

class Triangle
{
public:
	std::vector<Vertex> vertices;
	Triangle();
};



class Mesh
{
private:
	std::vector<Vector<float>> vertexPositions;
	std::vector<Vector<float>> vertexNormals;
	std::vector<Vector<float>> textureCoordinates;

	void readOBJ(const std::string& path);

public:
	std::vector<Triangle> triangles;
	//Material material;

	Mesh(std::string path);
	~Mesh();
	void print();
};
