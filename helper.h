#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <array>
#include <numeric>

//For initialisation lists to construct some things.
#include <utility>

#define JOINFUNCNAME(a,b) #a#b

#pragma warning(disable:4996)
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

template<typename T, typename U>
void WriteCSV(std::vector<T> _inputVectorX, std::vector<U> _inputVectorY, std::string _name)
{
	std::ofstream csvFile;
	csvFile.open(_name.c_str(), std::ofstream::out | std::ofstream::trunc);
	for (unsigned int i = 0; i < _inputVectorX.size(); i++)
		csvFile << _inputVectorX[i] << ", " << _inputVectorY[i] << ",\n";

	csvFile << "\n";
	std::cout << "Wrote " << _name.c_str() << " \n";
}

void WriteLog(std::string _msg, std::string _name)
{
	std::ofstream csvFile;
	csvFile.open(_name.c_str(), std::ofstream::out | std::ofstream::app);

	csvFile << _msg.c_str() << "\n";

	csvFile << "\n";
	std::cout << "Wrote " << _name.c_str() << " \n";
}

unsigned long NumberOfFloatStepsBetween(float _min, float _max)
{
	unsigned long counter = 0;

	float f = _min;
	while (f < _max)
	{
		f = std::nextafterf(f, _max + std::numeric_limits< float >::epsilon());
		counter++;
	}

	return counter;
}

unsigned long NumberOfDoubleStepsBetween(double _min, double _max)
{
	unsigned long counter = 0;

	double f = _min;
	while (f < _max)
	{
		f = std::nextafter(f, _max + std::numeric_limits< double >::epsilon());
		counter++;
	}
	return counter;
}

template<typename T, unsigned int elemCount>
struct vecN
{
public:
	vecN() {}

	template<class... Args>
	vecN(Args... args)
	{
		Set(args...);
	}

	template<class... Args>
	void Set(Args... args)
	{
		const int n = sizeof...(Args);
		static_assert(n == elemCount, "Invalid number of arguments for vector type");

		data = { { args... } };
	}

	void Multiply(T _mulVal)
	{
		std::for_each(data.begin(), data.end(), [&_mulVal](T& elem) { elem*= _mulVal; });
	}

	void Multiply(vecN<T,elemCount> _mulVal)
	{
		int counter = 0;
		std::for_each(data.begin(), data.end(), [&counter, &_mulVal](T& elem) { elem *= _mulVal.data[counter++]; });
	}

	void Add(T _addVal)
	{
		std::for_each(data.begin(), data.end(), [&_addVal](T& elem) { elem += _addVal; });
	}

	void Add(vecN<T, elemCount> _addVal)
	{
		int counter = 0;
		std::for_each(data.begin(), data.end(), [&counter, &_addVal](T& elem) { elem += _addVal.data[counter++]; });
	}

	float Length()
	{
		std::array<T, elemCount> dataSqr = data;
		std::for_each(dataSqr.begin(), dataSqr.end(), [](T& elem) { elem = elem*elem; });
		T sum = std::accumulate(dataSqr.begin(), dataSqr.end(), (T)0);
		return sqrt(sum);
	}

	void Normalise()
	{
		T len = 1.0/Length();
		std::for_each(data.begin(), data.end(), [&len](T& elem) { elem *= len; });
	}

	vecN<T, elemCount - 1> PopElem()
	{
		vecN<T, elemCount - 1> output;
		std::copy_n(data.begin(), elemCount-1, output.data.begin());
		return output;
	}

	vecN<T, elemCount + 1> PushElem(T _value = 0.0)
	{
		vecN<T, elemCount + 1> output;
		std::copy_n(data.begin(), elemCount, output.data.begin());
		output.data[elemCount] = _value;
		return output;
	}

	template<int _size>
	constexpr vecN<T, _size> GetResizedVector()
	{
		vecN<T, _size> output;
		std::copy_n(data.begin(), std::min((unsigned int)(_size), elemCount), output.data.begin());
		return output;
	}

	constexpr T& GetElement(int _index)
	{
		return data[_index];
	}

	template<int _index>
	constexpr T& GetElement()
	{
		return data[_index];
	}

	template<class... Indexs>
	constexpr vecN<T, sizeof...(Indexs)> GetOrderedArrayOfIndices(Indexs... indxs)
	{
		vecN<T, sizeof...(Indexs)> output;
		output.data = { { data[indxs]... } };
		return output;
	}

	constexpr T& x()
	{
		static_assert(1 <= elemCount, "Invalid number of arguments for vector type");
		return data[0];
	}

	constexpr T& y()
	{
		static_assert(2 <= elemCount, "Invalid number of arguments for vector type");
		return data[1];
	}

	constexpr T& z()
	{
		static_assert(3 <= elemCount, "Invalid number of arguments for vector type");
		return data[2];
	}

	constexpr T& w()
	{
		static_assert(4 <= elemCount, "Invalid number of arguments for vector type");
		return data[3];
	}

	constexpr std::array<T, 3> xxx()
	{
		static_assert(1 <= elemCount, "Invalid number of arguments for vector type");
		std::array<T, 3> xyz = { data[0], data[0], data[0] };
		return xyz;
	}

	constexpr std::array<T, 3> yyy()
	{
		static_assert(2 <= elemCount, "Invalid number of arguments for vector type");
		std::array<T, 3> xyz = { data[1], data[1], data[1] };
		return xyz;
	}

	constexpr std::array<T, 3> zzz()
	{
		static_assert(3 <= elemCount, "Invalid number of arguments for vector type");
		std::array<T, 3> xyz = { data[2], data[2], data[2] };
		return xyz;
	}

	constexpr std::array<T, 3> xyz()
	{
		static_assert(3 <= elemCount, "Invalid number of arguments for vector type");
		std::array<T, 3> xyz = { data[0], data[1], data[2] };
		return xyz;
	}
	constexpr std::array<T, 3> xzy()
	{
		static_assert(3 <= elemCount, "Invalid number of arguments for vector type");
		std::array<T, 3> xyz = { data[0], data[2], data[1] };
		return xyz;
	}

	constexpr std::array<T, 3> yzx()
	{
		static_assert(3 <= elemCount, "Invalid number of arguments for vector type");
		std::array<T, 3> xyz = { data[1], data[2], data[0] };
		return xyz;
	}

	constexpr std::array<T, 3> yxz()
	{
		static_assert(3 <= elemCount, "Invalid number of arguments for vector type");
		std::array<T, 3> xyz = { data[1], data[0], data[2] };
		return xyz;
	}

	constexpr std::array<T, 3> zyx()
	{
		static_assert(3 <= elemCount, "Invalid number of arguments for vector type");
		std::array<T, 3> xyz = { data[2], data[1], data[0] };
		return xyz;
	}

	constexpr std::array<T, 3> zxy()
	{
		static_assert(3 <= elemCount, "Invalid number of arguments for vector type");
		std::array<T, 3> xyz = { data[2], data[0], data[1] };
		return xyz;
	}

	constexpr std::array<T, 2> xx()
	{
		static_assert(1 <= elemCount, "Invalid number of arguments for vector type");
		std::array<T, 2> xyz = { data[0], data[0] };
		return xyz;
	}

	constexpr std::array<T, 2> xy()
	{
		static_assert(2 <= elemCount, "Invalid number of arguments for vector type");

		std::array<T, 2> xyz = { data[0], data[1] };
		return xyz;
	}

	constexpr std::array<T, 2> xz()
	{
		static_assert(3 <= elemCount, "Invalid number of arguments for vector type");
		std::array<T, 2> xyz = { data[0], data[2] };
		return xyz;
	}

	constexpr std::array<T, 2> yx()
	{
		static_assert(2 <= elemCount, "Invalid number of arguments for vector type");
		std::array<T, 2> xyz = { data[1], data[0] };
		return xyz;
	}

	constexpr std::array<T, 2> yy()
	{
		static_assert(2 <= elemCount, "Invalid number of arguments for vector type");
		std::array<T, 2> xyz = { data[1], data[1] };
		return xyz;
	}

	constexpr std::array<T, 2> yz()
	{
		static_assert(3 <= elemCount, "Invalid number of arguments for vector type");
		std::array<T, 2> xyz = { data[1], data[2] };
		return xyz;
	}

	constexpr std::array<T, 2> zx()
	{
		static_assert(3 <= elemCount, "Invalid number of arguments for vector type");
		std::array<T, 2> xyz = { data[2], data[0] };
		return xyz;
	}
	constexpr std::array<T, 2> zy()
	{
		static_assert(3 <= elemCount, "Invalid number of arguments for vector type");
		std::array<T, 2> xyz = { data[2], data[1] };
		return xyz;
	}
	constexpr std::array<T, 2> zz()
	{
		static_assert(3 <= elemCount, "Invalid number of arguments for vector type");
		std::array<T, 2> xyz = { data[2], data[1] };
		return xyz;
	}

	std::array<T, elemCount> data;
};
typedef vecN<float, 4> tfloat4;
typedef vecN<float, 3> tfloat3;
typedef vecN<float, 2> tfloat2;
typedef vecN<float, 1> tfloat1;

template<typename T>
struct vec2
{
	vec2()	{}

	void Scale(T _scale)
	{
		x *= _scale;
		y *= _scale;
	}

	vec2(T _x, T _y) : x(_x), y(_y) {}
	T x;
	T y;
};

template<typename T>
struct vec3
{
	vec3()	{}

	void Scale(T _scale)
	{
		x *= _scale;
		y *= _scale;
		z *= _scale;
	}

	vec3(T _x, T _y) : x(_x), y(_y), z(_z) {}
	T x;
	T y;
	T z;
};

template<typename T>
struct vec4
{
	vec4()	{}

	void Scale(T _scale)
	{
		x *= _scale;
		y *= _scale;
		z *= _scale;
		w *= _scale;
	}

	vec4(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}
	T x;
	T y;
	T z;
	T w;
};

struct int2
{
	int2(int _x, int _y) : x(_x), y(_y) {}
	int x, y;
};

struct ubyte4
{
	ubyte4() :x(0), y(0), z(0), w(0) {}
	ubyte4(unsigned char _x, unsigned char _y, unsigned char _z, unsigned char _w) :x(_x), y(_y), z(_z), w(_w) {}
	unsigned char x, y, z, w;
};

struct int4
{
	int4() :x(0), y(0), z(0), w(0) {}
	int4(int _x, int _y, int _z, int _w) :x(_x), y(_y), z(_z), w(_w) {}
	int x, y, z, w;
};

struct float4
{
	float4() :x(0.f), y(0.f), z(0.f), w(0.f) {}
	float4(float _x, float _y, float _z, float _w) :x(_x), y(_y), z(_z), w(_w) {}
	float x, y, z, w;
};

//width height
template < int WIDTH, int HEIGHT, typename T=ubyte4>
class CanvasStatic
{
public:
	CanvasStatic()  {}

	int GetWidth()
	{
		return WIDTH;
	}

	int GetHeight()
	{
		return HEIGHT;
	}

	void ClearImage(T& _col)
	{
		imageData.fill(_col);
	}

	void SetValue(int2& _pos, T& _col)
	{
		if (_pos.x > 0 && _pos.x < WIDTH &&
			_pos.y > 0 && _pos.y < HEIGHT
			)
		{
			int index = (_pos.y * WIDTH) + _pos.x;
			imageData[index] = _col;
		}
	}

	bool LoadImage(std::string _filename)
	{
		int x, y, n;
		unsigned char *data = stbi_load(_filename.c_str(), &x, &y, &n, 4);

		if (x != WIDTH || y != HEIGHT || n!=sizeof(T))
		{
			return false;
		}
		else
		{
			memcpy(&imageData[0], data, n*x*y);
		}

		stbi_image_free(data);

		return true;
	}

	void WriteImageBMP(std::string _str)
	{
		int bpp = 4;
		unsigned char*  data = (unsigned char*)imageData.data();
		stbi_write_bmp(_str.c_str(), WIDTH, HEIGHT, bpp, imageData.data());
	}

	void WriteImage()
	{
		int bpp = 4;
		unsigned char*  data = (unsigned char*)imageData.data();
		stbi_write_tga("testOutput.tga", WIDTH, HEIGHT, bpp, imageData.data());
	}
	void WriteImageTGA()
	{
		WriteImage();
	}

	void WriteImageBMP()
	{
		int bpp = 4;
		unsigned char*  data = (unsigned char*)imageData.data();
		stbi_write_bmp("testOutput.bmp", WIDTH, HEIGHT, bpp, imageData.data());
	}

	std::array<T, WIDTH*HEIGHT> imageData;
};

//width height
template <typename T = ubyte4>
class CanvasDynamic
{
public:
	CanvasDynamic(unsigned int _width, unsigned int _height)
		: m_width(_width), m_height(_height)
	{
		imageData = new T[_width * _height];
	}

	int GetWidth()
	{
		return m_width;
	}

	int GetHeight()
	{
		return m_height;
	}

	void ClearImage(T& _col)
	{
		for (int i = 0; i < m_width*m_height; i++)
		{
			imageData[i] = _col;
		}
	}

	T GetValue(int2& _pos)
	{
		if (_pos.x >= 0 && _pos.x < m_width &&
			_pos.y >= 0 && _pos.y < m_height
			)
		{
			int index = (_pos.y * m_width) + _pos.x;
			return imageData[index];
		}
	}

	void SetValue(int2& _pos, T& _col)
	{
		if (_pos.x >= 0 && _pos.x < m_width &&
			_pos.y >= 0 && _pos.y < m_height
			)
		{
			int index = (_pos.y * m_width) + _pos.x;
			imageData[index] = _col;
		}
	}

	void SetValue(tfloat2& _pos, T& _col)
	{
		if (_pos.x() >= 0.0f && _pos.x() < 1.0f &&
			_pos.y() >= 0.0f && _pos.y() < 1.0f
			)
		{
			int posx = std::round(_pos.x() * (float)m_width);
			int posy = std::round(_pos.y() * (float)m_height);

			int index = (posy * m_width) + posx;
			imageData[index] = _col;
		}
	}

	void AddValue(tfloat2& _pos, T& _col)
	{
		if (_pos.x() >= 0.0f && _pos.x() < 1.0f &&
			_pos.y() >= 0.0f && _pos.y() < 1.0f
			)
		{
			int posx = _pos.x() * (float)m_width;
			int posy = _pos.y() * (float)m_height;

			int index = (posy * m_width) + posx;

			imageData[index].x += _col.x;
			imageData[index].y += _col.y;
			imageData[index].z += _col.z;
			imageData[index].w += _col.w;
		}
	}

	void WriteImage()
	{
		int bpp = 4;
		unsigned char*  data = (unsigned char*)imageData;
		stbi_write_tga("Output.tga", m_width, m_height, bpp, imageData);
	}
	void WriteImageTGA()
	{
		WriteImage();
	}

	void WriteImageBMP()
	{
		int bpp = 4;
		unsigned char*  data = (unsigned char*)imageData;
		stbi_write_bmp("Output.bmp", m_width, m_height, bpp, imageData);
	}

	void WriteImageBMP(std::string _name)
	{
		_name.append(".bmp");
		int bpp = 4;
		unsigned char*  data = (unsigned char*)imageData;
		stbi_write_bmp(_name.c_str(), m_width, m_height, bpp, imageData);
	}

	bool LoadImage(std::string _filename)
	{
		int x, y, n;
		int sz = sizeof(T);
		unsigned char *data = stbi_load(_filename.c_str(), &x, &y, &n, sizeof(T));

		if (x != m_width || y != m_height)// || n != sizeof(T))
		{
			return false;
		}
		else
		{
			memcpy(&imageData[0], data, sz*x*y);
		}

		stbi_image_free(data);

		return true;
	}

	T* GetData()
	{
		return imageData;
	}

	T* imageData;
	unsigned int m_width;
	unsigned int m_height;
};