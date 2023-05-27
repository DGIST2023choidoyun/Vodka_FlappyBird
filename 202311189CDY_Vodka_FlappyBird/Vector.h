
#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <cmath>

#define VECTOR2_ZERO_D VECTOR2<double>(0, 0)
#define VECTOR2_ZERO_F VECTOR2<float>(0, 0)
#define VECTOR2_ZERO_I VECTOR2<int>(0, 0)
#define VECTOR2_X_D VECTOR2<double>(1, 0)
#define VECTOR2_X_F VECTOR2<float>(1, 0)
#define VECTOR2_X_I VECTOR2<int>(1, 0)
#define VECTOR2_Y_D VECTOR2<double>(0, 1)
#define VECTOR2_Y_F VECTOR2<float>(0, 1)
#define VECTOR2_Y_I VECTOR2<int>(0, 1)

template <typename T>
struct DOUBLE_PAIR {
	T value[2] = { 0, 0 };
};

template <typename T>
struct VECTOR2 {
	T x, y;

	VECTOR2()
	{
		x = y = 0;
	}
	VECTOR2(T x, T y)
	{
		this->x = x;
		this->y = y;
	}

	VECTOR2<T> Add(VECTOR2<T> i)
	{
		x += i.x;
		y += i.y;

		return *this;
	}
	VECTOR2<T> operator+=(VECTOR2<T> i)
	{
		x += i.x;
		y += i.y;

		return *this;
	}
	VECTOR2<T> Add(T x, T y)
	{
		this->x += x;
		this->y += y;

		return *this;
	}
	VECTOR2<T> Subtract(VECTOR2<T> i)
	{
		x -= i.x;
		y -= i.y;

		return *this;
	}
	VECTOR2<T> operator-=(VECTOR2<T> i)
	{
		x -= i.x;
		y -= i.y;

		return *this;
	}
	VECTOR2<T> Subtract(T x, T y)
	{
		this->x -= x;
		this->y -= y;

		return *this;
	}
	VECTOR2<T> operator+(VECTOR2<T> i) const
	{
		return VECTOR2<T>{ x + i.x, y + i.y };
	}
	VECTOR2<T> Addition(T x, T y) const
	{
		return VECTOR2<T>{ this->x + x, this->y + y };
	}
	VECTOR2<T> operator-(VECTOR2<T> i) const
	{
		return VECTOR2<T>{ x - i.x, y - i.y };
	}
	VECTOR2<T> Subtraction(T x, T y) const
	{
		return VECTOR2<T>{ this->x - x, this->y - y };
	}
	T Dot(VECTOR2<T> i) const
	{
		return x * i.x + y * i.y;
	}
	T operator*(VECTOR2<T> i) const
	{
		return x * i.x + y * i.y;
	}
	T Dot(T x, T y) const
	{
		return this->x * x + this->y * y;
	}
	VECTOR2<T> ConstProduct(double d)
	{
		x *= d;
		y *= d;
		return *this;
	}
	VECTOR2<T> operator*(double d) const
	{
		return VECTOR2<T>{ x * d, y * d };
	}
	VECTOR2<T> operator*=(double d)
	{
		x *= d;
		y *= d;
		return *this;
	}
	VECTOR2<T> operator/(double d) const
	{
		return VECTOR2<T>{ x / d, y / d };
	}
	VECTOR2<T> operator/=(double d)
	{
		x /= d;
		y /= d;
		return *this;
	}
	double Magnitude() const
	{
		return sqrt(x * x + y * y);
	}
	VECTOR2<T> Opposite() const
	{
		return VECTOR2<T>{-x, -y};
	}
	VECTOR2<T> Normal() const
	{
		return VECTOR2<T>{-y, x};
	}
	VECTOR2<T> Direction() const
	{
		double r = Magnitude();
		return VECTOR2<T>{x / r, y / r};
	}
	VECTOR2<T> Projection(VECTOR2<T> target) const
	{
		double length = target.Magnitude();
		return target.Direction() * Dot(target) / length;
	}
	void Rotate(double rad)
	{
		double c = cos(rad), s = sin(rad);
		double xP = c * x + s * y;
		y = -s * x + c * y;
		x = xP;
	}
	VECTOR2<T> Rotation(double rad) const
	{
		double c = cos(rad), s = sin(rad);
		return VECTOR2<T>{c * x + s * y, -s * x + c * y};
	}
	bool operator<(VECTOR2<T> v)
	{
		if (x < v.x && y < v.y)
			return true;
		return false;
	}
	bool operator>(VECTOR2<T> v)
	{
		if (x < v.x && y < v.y)
			return false;
		return true;
	}
};

#endif