#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <vector>

template <typename T>
class Matrix
{
public:

	std::vector<std::vector<T>> matrix;

	uint32_t width, height;

	Matrix() = default;

	Matrix(std::vector<std::vector<T>> matrix) : matrix(matrix)
	{
		width = matrix.at(0).size();
		height = matrix.size();
	}

	Matrix(uint32_t width, uint32_t height, T val) : width(width), height(height)
	{
		matrix.resize(height);
		for (auto& h : matrix) h.resize(width);

		Fill(val);
	}

	void SetSize(uint32_t w, uint32_t h, T val)
	{
		matrix.resize(h);
		for (auto& h : matrix) h.resize(w);

		for (auto tmp_h = 0u; tmp_h < h; ++tmp_h)
		{
			for (auto tmp_w = 0u; tmp_w < w; ++tmp_w)
			{
				if (tmp_h < height || tmp_w < width) continue;
				At(tmp_w, tmp_h) = val;
			}
		}

		width = w;
		height = h;
	}

	std::vector<T>& Row(uint32_t r)
	{
		return matrix.at(r);
	}
	// to not affect the performance, returning pointers is necessary
	std::vector<T*> Column(uint32_t c)
	{
		std::vector<T*> col;
		for (auto h = 0u; h < height; ++h) col.push_back(&matrix.at(h).at(c));
		return col;
	}

	T& At(uint32_t x, uint32_t y)
	{
		return matrix.at(y).at(x);
	}
	T At(uint32_t x, uint32_t y) const
	{
		if (x > width || y > height) return 0;
		return matrix.at(y).at(x);
	}

	void Fill(T val)
	{
		for (auto& h : matrix)
		{
			for (auto& w : h)
			{
				w = val;
			}
		}
	}

	void Line(int x1, int y1, int x2, int y2, T val)
	{
		float m = fabsf(y1 - y2) / fabsf(x1 - x2);
		
		if (m > 1 || x1 == x2)
		{
			for (int y = 0; y <= abs(y1 - y2); ++y)
			{
				int posx;
				if (!std::isnan(m)) posx = static_cast<int>(y / m + (x1 < x2 ? x1 : x2));
				else posx = (x1 < x2 ? x1 : x2);
				int posy = static_cast<int>(y + (y1 < y2 ? y1 : y2));

				At(posx, posy) = val;
			}
		}
		else
		{
			for (int x = 0; x <= abs(x1 - x2); ++x)
			{
				int posx = static_cast<int>(x + (x1 < x2 ? x1 : x2));
				int posy = static_cast<int>(x * m + (y1 < y2 ? y1 : y2));
				
				At(posx, posy) = val;
			}
		}
	}

	void Rectangle(uint32_t x, uint32_t y, uint32_t w, uint32_t h, T val)
	{
		for (auto ax = x; ax < x + w; ++ax)
		{
			for (auto ay = y; ay < y + h; ++ay)
			{
				if (ax > width || ay > height) continue;
				At(ax, ay) = val;
			}
		}
	}

	Matrix operator+(const Matrix& m)
	{
		int w = std::max(this->width, m.width), h = std::max(this->height, m.height);
		Matrix<T> matrix(w, h, 0);

		for (int x = 0; x < w; ++x)
		{
			for (int y = 0; y < h; ++y)
			{
				matrix.At(x, y) = this->At(x, y) + m.At(x, y);
			}
		}

		return matrix;
	}
};

typedef Matrix<int> Matrixi;
typedef Matrix<float> Matrixf;