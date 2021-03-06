#pragma once

#include <cmath>
#include <iostream>
#include <SFML/System/Vector2.hpp>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

template <typename T>
class Vec2 {
public:

	T x, y;

	Vec2() :x(0), y(0) {}
	Vec2(T x, T y) : x(x), y(y) {}
	Vec2(const Vec2& v) : x(v.x), y(v.y) {}

	Vec2& operator=(const Vec2& v)
	{
		x = v.x;
		y = v.y;
		return *this;
	}

	Vec2 operator+(const Vec2& v)
	{
		return Vec2(x + v.x, y + v.y);
	}
	Vec2 operator-(const Vec2& v)
	{
		return Vec2(x - v.x, y - v.y);
	}
	Vec2 operator*(const Vec2& v)
	{
		return Vec2(x * v.x, y * v.y);
	}
	Vec2 operator/(const Vec2& v)
	{
		return Vec2(x / v.x, y / v.y);
	}

	Vec2& operator+=(const Vec2& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}
	Vec2& operator-=(const Vec2& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}
	Vec2& operator*=(const Vec2& v)
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}
	Vec2& operator/=(const Vec2& v)
	{
		x /= v.x;
		y /= v.y;
		return *this;
	}

	Vec2 operator+(T s)
	{
		return Vec2(x + s, y + s);
	}
	Vec2 operator-(T s)
	{
		return Vec2(x - s, y - s);
	}
	Vec2 operator*(T s)
	{
		return Vec2(x * s, y * s);
	}
	Vec2 operator/(T s)
	{
		return Vec2(x / s, y / s);
	}

	Vec2& operator+=(T s)
	{
		x += s;
		y += s;
		return *this;
	}
	Vec2& operator-=(T s)
	{
		x -= s;
		y -= s;
		return *this;
	}
	Vec2& operator*=(T s)
	{
		x *= s;
		y *= s;
		return *this;
	}
	Vec2& operator/=(T s)
	{
		x /= s;
		y /= s;
		return *this;
	}

	Vec2& operator++()
	{
		x += 1.0;
		y += 1.0;
		return *this;
	}
	Vec2& operator--()
	{
		x -= 1.0;
		y -= 1.0;
		return *this;
	}
	Vec2 operator++(int)
	{
		Vec2 tmp(x, y);
		x += 1.0;
		y += 1.0;
		return tmp;
	}
	Vec2 operator--(int)
	{
		Vec2 tmp(x, y);
		x -= 1.0;
		y -= 1.0;
		return tmp;
	}

	friend bool operator==(const Vec2& v1, const Vec2& v2)
	{
		return v1.x == v2.x && v1.y == v2.y;
	}
	friend bool operator!=(const Vec2& v1, const Vec2& v2)
	{
		return !(v1.x == v2.x && v1.y == v2.y);
	}

	void Set(T x, T y)
	{
		this->x = x;
		this->y = y;
	}

	void Rotate(T deg)
	{
		T theta = deg / 180.0 * M_PI;
		T c = cos(theta);
		T s = sin(theta);
		T tx = x * c - y * s;
		T ty = x * s + y * c;
		x = tx;
		y = ty;
	}

	Vec2& Normalize()
	{
		if (Magnitude() == 0) return *this;
		*this *= (1.0 / Magnitude());
		return *this;
	}

	Vec2 Ortho() const
	{
		return Vec2(y, -x);
	}

	double Distance(const Vec2& v) const
	{
		Vec2 d(v.x - x, v.y - y);
		return d.Magnitude();
	}

	double SqrMagnitude() const
	{
		return x * x + y * y;
	}
	double Magnitude() const
	{
		return std::sqrt(SqrMagnitude());
	}

	Vec2 Abs() const
	{
		return Vec2(x < 0 ? x * -1 : x, y < 0 ? y * -1 : y);
	}

	double Degree(bool flipYAxis = false) const
	{
		T tmp = this->y;
		if (flipYAxis) tmp *= -1;
		double degree;
		if (tmp != 0) degree = atan(this->x / tmp) * 180 / M_PI;
		else
		{
			degree = 90;
			if (this->x < 0) degree *= -1;
		}
		if (tmp < 0 && flipYAxis) degree -= 180;
		return degree;
	}

	int Quadrant() const
	{
		if (x > 0)
		{
			if (y > 0)
			{
				return 1;
			}
			else
			{
				return 4;
			}
		}
		else
		{
			if (y > 0)
			{
				return 2;
			}
			else
			{
				return 3;
			}
		}
	}
	static Vec2 ReQuadrant(int quadrant)
	{
		switch (quadrant)
		{
			case 1:
				return Vec2(1, 1);
				break;
			case 2:
				return Vec2(-1, 1);
				break;
			case 3:
				return Vec2(-1, -1);
				break;
			case 4:
				return Vec2(1, -1);
				break;
			default:
				return Vec2::Zero();
				break;
		}
	}
	static Vec2 ReQuadrant(double deg, double zero = 0, bool anticlockwise = true)
	{
		double nDeg;

		if (anticlockwise)
		{
			nDeg = deg - zero;
		}
		else
		{
			nDeg = zero - deg;
		}

		while (nDeg < 0) nDeg += 360;

		nDeg = fmod(nDeg, 360);

		if (nDeg >= 0 && nDeg < 90) return Vec2<T>(1, 1);
		else if (nDeg >= 90 && nDeg < 180) return Vec2<T>(-1, 1);
		else if (nDeg >= 180 && nDeg < 270) return Vec2<T>(-1, -1);
		else if (nDeg >= 270 && nDeg < 360) return Vec2<T>(1, -1);
	}

	static Vec2 SplitComponents(T m, T deg, int quadrant = 1)
	{
		T theta = deg / 180.0 * M_PI;

		T c = m * cos(theta);
		T s = m * sin(theta);
		return Vec2(c, s) * ReQuadrant(quadrant);
	}

	static T Dot(const Vec2& v1, const Vec2& v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}
	static T Cross(const Vec2& v1, const Vec2& v2)
	{
		return (v1.x * v2.y) - (v1.y * v2.x);
	}

	// 0 --> collinear
	// >0 -> clockwise 
	// <0 -> counterclockwise 
	double OrientationEx(const Vec2& v2, const Vec2& v3) const
	{
		return (static_cast<long double>(v2.y) - y) * (static_cast<long double>(v3.x) - v2.x) - (static_cast<long double>(v2.x) - x) * (static_cast<long double>(v3.y) - v2.y);
	}
	// 0 --> collinear
	// 1 --> clockwise 
	// -1 -> counterclockwise 
	int Orientation(const Vec2& v2, const Vec2& v3) const
	{
		double val = OrientationEx(v2, v3);

		if (val == 0.0) return 0;
		return (val > 0.0) ? 1 : -1;
	}

	static Vec2 One()
	{
		return Vec2(1, 1);
	}
	static Vec2 Zero()
	{
		return Vec2(0, 0);
	}
	static Vec2 Up()
	{
		return Vec2(0, 1);
	}
	static Vec2 Down()
	{
		return Vec2(0, -1);
	}
	static Vec2 Left()
	{
		return Vec2(-1, 0);
	}
	static Vec2 Right()
	{
		return Vec2(1, 0);
	}

	template <typename T>
	Vec2<T> Cast() const
	{
		return Vec2<T>(x, y);
	}

	sf::Vector2f sfVector2f() const
	{
		return sf::Vector2f(x, y);
	}
	sf::Vector2i sfVector2i() const
	{
		return sf::Vector2i(x, y);
	}
	sf::Vector2u sfVector2u() const
	{
		return sf::Vector2f(x, y);
	}

	template <typename T, typename TArg>
	static Vec2<T> ToVector2D(const sf::Vector2<TArg> sfv)
	{
		return Vec2<T>(sfv.x, sfv.y);
	}

	friend std::ostream& operator<<(std::ostream& o, const Vec2& v)
	{
		o << v.x << ", " << v.y;
		return o;
	}
};

typedef Vec2<double> Vector2D;
typedef Vec2<float> Vector2Df;
typedef Vec2<int> Vector2Di;
typedef Vec2<unsigned int> Vector2Du;