#pragma once
#include <vector>
#include <string>
#include "linalg.h"
#include <optional>

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

//class Material
//{
//public:
//	Material();
//};
//


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
	void renderMesh(); // Add reference to render target (ie buffers)
	void print();
};


class Object
{
private:
	std::vector<Object*> children;

public:
	Matrix<float> transform;
	Mesh* mesh; // Mesh should be heap allocated

	Object();
	void render();
	void transformByMatrix(Matrix<float>& transformation);
	void translate(float translateX, float translateY, float translateZ);
	void rotate(float rotateX, float rotateY, float rotateZ);
	void scale(float scaleX, float scaleY, float scaleZ);
	void scale(float scalingFactor);

	void addChild(Object& newChild);
	std::vector<Object*>& getChildren();
};

Object createObjectFromFile(std::string path);

class Pointlight : Object
{

};
