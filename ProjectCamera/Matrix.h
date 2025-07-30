#pragma once
#include<cmath>
#include<algorithm>
#include"Vector.h"
class Matrix
{
public:
	Matrix(double rad);
	~Matrix();
	void set(double rad);
	Matrix transpose();
	void transposeThis();
	Matrix operator*(Matrix rhs);
	template<typename C>
	inline Vector<C> operator*(Vector<C> rhs)
	{
		Vector<C> V(2);
		V.set(
			components[0] * rhs.x + components[1] * rhs.y,
			components[2] * rhs.x + components[3] * rhs.y
		);
		return V;
	}
	double components[4];
	double angle;
};
