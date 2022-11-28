#pragma once
#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include <random>
#include <functional>
#include "utils.h"

template<typename T>
class Vector;

template <typename T>
class Matrix
{
protected:
	std::vector<T> m_Values;
	int m_Width;
	int m_Height;

private:
	Matrix<T> pointwise(Matrix<T>& otherMatrix, T(*func)(T, T))
	{
		assert(m_Width == otherMatrix.width() && m_Height == otherMatrix.height(), 
			std::string("Shape of matrix A (") + std::to_string(m_Height) + ", " + std::to_string(m_Width) 
			+ ") does not equal shape of matrix B (" + std::to_string(otherMatrix.height()) + ", " + std::to_string(otherMatrix.width()) + ").");

		std::vector<T> newMatrix;
		newMatrix.reserve(m_Width * m_Height);

		for (int i = 0; i < m_Height; i++)
		{
			for (int j = 0; j < m_Width; j++)
			{
				newMatrix.push_back(func((*this)(i, j), otherMatrix(i, j)));
			}
		}
		return Matrix<T>(newMatrix, m_Height, m_Width);
	}

	Matrix<T> pointwiseScalar(std::function<T(T)> func)
	{
		std::vector<T> newMatrix;
		newMatrix.reserve(m_Height * m_Width);
		for (T value : m_Values)
			newMatrix.push_back(func(value));

		return Matrix<T>(newMatrix, m_Height, m_Width);
	}

	Matrix<T> getMinor(int y, int x)
	{
		assert(m_Width > 1 && m_Height > 1, std::string("Cannot calculate minor of matrix with size (") + std::to_string(m_Height) + ", " + std::to_string(m_Width) + ").");

		int newWidth = m_Width - 1;
		int newHeight = m_Height - 1;

		std::vector<T> newMinor;
		newMinor.reserve(newHeight * newWidth);

		for (int i = 0; i < m_Height; i++)
		{
			for (int j = 0; j < m_Width; j++)
			{
				if (i != y && j != x)
					newMinor.push_back((*this)(i, j));
			}
		}
		return Matrix<T>(newMinor, newHeight, newWidth);
	}

	Matrix<T> getCofactorMatrix()
	{
		assert(m_Height == m_Width, "Cannot calculate cofactor matrix of non-square matrix.");

		std::vector<T> newCofactorMatrix;
		newCofactorMatrix.reserve(m_Height * m_Height);

		for (int i = 0; i < m_Height; i++)
		{
			for (int j = 0; j < m_Width; j++)
				newCofactorMatrix.push_back(pow(-1, i+j) * getMinor(i, j).determinant());
		}

		return Matrix<T>(newCofactorMatrix, m_Height, m_Width);
	}

	Matrix<T> getAdjunct()
	{
		return getCofactorMatrix().transpose();
	}

public:
	Matrix(std::initializer_list<std::initializer_list<T>> inputValues)
		: m_Height((int)inputValues.size()), m_Width((int)(*inputValues.begin()).size())
	{
		m_Values.reserve(m_Width * m_Height);
		for (std::initializer_list<T> row : inputValues)
		{
			for (T value : row)
			{
				m_Values.push_back(value);
			}
		}
		std::cout << "Created Matrix of size (" << m_Height << ", " << m_Width << ")." << std::endl;
	}

	Matrix(std::vector<T> inputValues, int height, int width)
		: m_Values(inputValues), m_Width(width), m_Height(height)
	{}

	T& operator () (int y, int x)
	{
		return m_Values[x + y * m_Width];
	}

	Matrix<T> operator + (Matrix<T>& otherMatrix)
	{
		return pointwise(otherMatrix, [](T a, T b) { return a + b; });
	}

	Matrix<T> operator + (T value)
	{
		return pointwiseScalar([&](T a) { return a + value; });
	}

	Matrix<T> operator - (Matrix<T>& otherMatrix)
	{
		return pointwise(otherMatrix, [](T a, T b) { return a - b; });
	}

	Matrix<T> operator - (T value)
	{
		return pointwiseScalar([&](T a) { return a - value; });
	}

	Matrix<T> operator * (Matrix<T>& otherMatrix)
	{
		return pointwise(otherMatrix, [](T a, T b) { return a * b; });
	}

	Matrix<T> operator * (T value)
	{
		return pointwiseScalar([&](T a) { return a * value; });
	}

	Matrix<T> operator / (Matrix<T>& otherMatrix)
	{
		return pointwise(otherMatrix, [](T a, T b) { return a / b; });
	}

	Matrix<T> operator / (T value)
	{
		return pointwiseScalar([&](T a) { return a / value; });
	}

	Matrix<T> matmul(Matrix<T>& otherMatrix)
	{
		assert(m_Width == otherMatrix.height(), 
			std::string("Width of matrix A '") + std::to_string(m_Width) + "' does not equal height of matrix B '" + std::to_string(otherMatrix.height()) + "'.");

		// Initialise new matrix data
		std::vector<T> newMatrix(m_Height * otherMatrix.width(), 0);

		for (int i = 0; i < m_Height; i++)
		{
			for (int k = 0; k < m_Width; k++)
			{
				for (int j = 0; j < otherMatrix.width(); j++)
				{
					newMatrix[j + i * otherMatrix.width()] += (*this)(i, k) * otherMatrix(k, j);
				}
			}
		}

		return Matrix<T>(newMatrix, m_Height, otherMatrix.width());
	}
	
	Vector<T> matmul(Vector<T>& otherVector)
	{
		assert(m_Width == otherVector.height(),
			std::string("Width of matrix A '") + std::to_string(m_Width) + "' does not equal height of vector B '" + std::to_string(otherVector.height()) + "'.");

		std::vector<T> newVector(m_Height, 0);

		for (int i = 0; i < m_Height; i++)
		{
			for (int k = 0; k < m_Width; k++)
			{
				newVector[i] += (*this)(i, k) * otherVector(k);
			}
		}
		return Vector<T>(newVector);
	}

	T determinant()
	{
		assert(m_Height == m_Width, "Cannot calculate determinant of non-square matrix.");

		// Base case
		if (m_Height == 1)
		{
			return m_Values[0];
		}

		T output = 0;
		for (int column = 0; column < m_Width; column++)
		{
			Matrix<T> currentMinor = getMinor(0, column);
			output += pow(-1, column) * m_Values[column] * currentMinor.determinant();
		}
		return output;
	}

	Matrix<T> inverse()
	{
		T det = determinant();
		if (det == 0)
		{
			std::cout << "Cannot inverse non-singular matrix. Returning self." << std::endl;
			return *this;
		}

		Matrix<T> adj = getAdjunct();
		return adj / det;
	}

	Matrix transpose()
	{
		Matrix<T> transposed = createEmptyMatrix<T>(m_Width, m_Height);
		for (int i = 0; i < m_Height; i++)
		{
			for (int j = 0; j < m_Width; j++)
				transposed(j, i) = (*this)(i, j);
		}

		return transposed;
	}

	void print()
	{
		// To account for proper spacing, find the largest value in number of digits.
		int largestNumDigits = 0;
		for (T value : m_Values)
		{
			int length = (int)std::to_string(value).length();
			largestNumDigits = length > largestNumDigits ? length : largestNumDigits;
		}

		// Print each value in a matrix format with adequate spacing
		for (int y = 0; y < m_Height; y++)
		{
			for (int x = 0; x < m_Width; x++)
			{
				std::string s = std::to_string(m_Values[x + y * m_Width]);
				int spacing = x < m_Width - 1 ? largestNumDigits - (int)s.length() + 1 : 1;
				s.append(spacing, ' ');
				s = s[0] != '-' ? std::string(" ") + s : s + " ";
				std::cout << s;
			}
			std::cout << "\n";
		}
	}

	const int width() const
	{
		return m_Width;
	}
	
	const int height() const
	{
		return m_Height;
	}

	const std::tuple<int, int> size()
	{
		return { m_Height, m_Width };
	}
};


template <typename T>
class Vector : public Matrix<T>
{
private:
	Vector<T> pointwise(Vector<T>& otherVector, T(*func)(T, T))
	{
		assert(Matrix<T>::m_Height == otherVector.height(),
			std::string("Height of vector A '") + std::to_string(Matrix<T>::m_Height) + "' does not equal height of vector B (" + std::to_string(otherVector.height()) + "'.");

		std::vector<T> newVector;
		newVector.reserve(Matrix<T>::m_Height);

		for (int i = 0; i < Matrix<T>::m_Height; i++)
			newVector.push_back(func((*this)(i), otherVector(i)));

		std::cout << Matrix<T>::m_Height << std::endl;

		return Vector<T>(newVector);
	}

	Vector<T> pointwiseScalar(std::function<T(T)> func)
	{
		std::vector<T> newVector;
		newVector.reserve(Matrix<T>::m_Height);

		for (T value : Matrix<T>::m_Values)
			newVector.push_back(func(value));

		return Vector<T>(newVector);
	}

public:
	Vector(std::initializer_list<T> inputValues)
		: Matrix<T>(inputValues, inputValues.size(), 1)
	{}

	Vector(std::vector<T> inputValues)
		: Matrix<T>(inputValues, inputValues.size(), 1)
	{}

	T& operator () (int x)
	{
		return Matrix<T>::m_Values[x];
	}

	Vector<T> operator + (Vector<T>& otherVector)
	{
		return pointwise(otherVector, [](T a, T b) { return a + b; });
	}

	Vector<T> operator + (T value)
	{
		return pointwiseScalar([&](T a) { return a + value; });
	}

	Vector<T> operator - (Vector<T>& otherVector)
	{
		return pointwise(otherVector, [](T a, T b) { return a - b; });
	}

	Vector<T> operator - (T value)
	{
		return pointwiseScalar([&](T a) { return a - value; });
	}

	Vector<T> operator * (Vector<T>& otherVector)
	{
		return pointwise(otherVector, [](T a, T b) { return a * b; });
	}

	Vector<T> operator * (T value)
	{
		return pointwiseScalar([&](T a) { return a * value; });
	}

	Vector<T> operator / (Vector<T>& otherVector)
	{
		return pointwise(otherVector, [](T a, T b) { return a / b; });
	}

	Vector<T> operator / (T value)
	{
		return pointwiseScalar([&](T a) { return a / value; });
	}
};

template <typename T>
Matrix<T> createEmptyMatrix(int height, int width)
{
	std::vector<T> values(height * width, 0);
	return Matrix<T>(values, height, width);
}

template <typename T>
Vector<T> createEmptyVector(int height)
{
	std::vector<T> values(height, 0);
	return Vector<T>(values);
}

template <typename T>
Matrix<T> createIdentityMatrix(int size)
{
	Matrix<T> newMatrix = createEmptyMatrix<T>(size, size);
	for (int i = 0; i < size; i++)
		newMatrix(i, i) = 1;

	return newMatrix;
}

template <typename T>
Matrix<T> createRandomMatrix(int height, int width)
{
	std::random_device rd;
	std::normal_distribution<T> randomGenerator(0, 1);
	Matrix<T> newMatrix = createEmptyMatrix<T>(height, width);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			newMatrix(i, j) = randomGenerator(rd);
		}
	}
	return newMatrix;
}

template <typename T>
Vector<T> createRandomVector(int height)
{
	std::random_device rd;
	std::normal_distribution<T> randomGenerator(0, 1);
	Vector<T> newVector = createEmptyVector<T>(height);
	for (int i = 0; i < height; i++)
	{
		newVector(i) = randomGenerator(rd);
	}
	return newVector;
}