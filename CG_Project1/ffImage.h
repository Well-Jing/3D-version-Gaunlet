#pragma once
//#include <glad/glad.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
//#include"glm.hpp"
#include"gtc/matrix_transform.hpp"
#include"gtc/type_ptr.hpp"
#include<vector>
#include<map>

typedef unsigned char	byte;

struct ffRGBA
{
	byte m_r;
	byte m_g;
	byte m_b;
	byte m_a;

	ffRGBA(byte _r = 255,
		byte _g = 255,
		byte _b = 255,
		byte _a = 255)
	{
		m_r = _r;
		m_g = _g;
		m_b = _b;
		m_a = _a;
	}
};

template<typename T>
struct tVec3
{
	T	m_x;
	T	m_y;
	T	m_z;
	tVec3(T _x = 0, T _y = 0, T _z = 0)
	{
		m_x = _x;
		m_y = _y;
		m_z = _z;
	}
};
template<typename T>
struct tVec2
{
	T	m_x;
	T	m_y;

	tVec2(T _x = 0, T _y = 0)
	{
		m_x = _x;
		m_y = _y;
	}
};


class ffImage
{
private:
	int					m_width;
	int					m_height;
	int					m_picType;
	ffRGBA*				m_data;
public:
	int			getWidth()const { return m_width; }
	int			getHeight()const { return m_height; }
	int			getPicType()const { return m_picType; }
	ffRGBA*		getData()const { return m_data; }

	ffRGBA getColor(int x, int y)const
	{
		if (x < 0 || x > m_width - 1 || y <0 || y > m_height - 1)
		{
			return ffRGBA(0, 0, 0, 0);
		}
		return m_data[y * m_width + x];
	}
	ffImage(int _width = 0, int _height = 0, int _picType = 0, ffRGBA* _data = NULL)
	{
		m_width = _width;
		m_height = _height;
		m_picType = _picType;

		int _sumSize = m_width * m_height;
		if (_data && _sumSize)
		{
			m_data = new ffRGBA[_sumSize];
			memcpy(m_data, _data, sizeof(ffRGBA) * _sumSize);
		}
		else
		{
			m_data = NULL;
		}
	}
	~ffImage()
	{
		if (m_data)
		{
			delete[]m_data;
		}
	}

public:
	static ffImage* readFromFile(const char* _fileName);
	static ffImage* readFromMemory(unsigned char* _data, unsigned int _width, unsigned int _height);
};

