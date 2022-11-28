#pragma once
#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include <random>
#include "utils.h"

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

	Matrix<T> operator - (Matrix<T>& otherMatrix)
	{
		return pointwise(otherMatrix, [](T a, T b) { return a - b; });
	}

	Matrix<T> operator * (Matrix<T>& otherMatrix)
	{
		return pointwise(otherMatrix, [](T a, T b) { return a * b; });
	}

	Matrix<T> operator / (Matrix<T>& otherMatrix)
	{
		return pointwise(otherMatrix, [](T a, T b) { return a / b; });
	}

	Matrix matmul(Matrix& otherMatrix)
	{
		assert(m_Width == otherMatrix.height(), 
			std::string("Width of matrix A '") + std::to_string(m_Width) + "' does not equal height of matrix B '" + std::to_string(otherMatrix.height()) + "'.");

		// Initialise new matrix data
		std::vector<T> newMatrix;
		newMatrix.reserve(m_Height * otherMatrix.width());
		for (int i = 0; i < newMatrix.capacity(); i++)
			newMatrix.push_back(0);

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
public:
	Vector(std::initializer_list<T> inputValues)
		: Matrix<T>(inputValues, inputValues.size(), 1)
	{
		std::cout << "Created Vector" << std::endl;
	}

	T operator () (int x)
	{
		return Matrix<T>::m_Values[x];
	}
};

template <typename T>
Matrix<T> createEmptyMatrix(int height, int width)
{
	std::vector<T> values;
	values.reserve(height * width);
	for (int i = 0; i < values.capacity(); i++)
		values.push_back(0);

	return Matrix<T>(values, height, width);
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