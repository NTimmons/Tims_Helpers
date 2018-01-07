#pragma once

#include <type_traits>
#include <iostream>

class FloatError
{
public:

	FloatError(float _x)
	{
		static int floatIDCounter = 0;

		m_name = "FloatID_";
		m_name.append(std::to_string(floatIDCounter));

		m_value = _x;
		m_error = 0.0;
	}

	FloatError(float _x, long double _error)
	{
		static int floatIDCounter = 0;

		m_name = "FloatID_";
		m_name.append(std::to_string(floatIDCounter));

		m_value = _x;
		m_error = _error;
	}

	FloatError()
	{
		static int floatIDCounter = 0;

		m_name = "FloatID_";
		m_name.append(std::to_string(floatIDCounter));

		m_value = 0.f;
		m_error = 0.0;
	}

	~FloatError()
	{
		//std::cout << m_name.c_str() << " error was: " << m_error << "\n";
	}

	//Operators overload
	FloatError operator+(const FloatError& _in)
	{
		float val = m_value + _in.m_value;
		long double error = ((long double)m_value + (long double)_in.m_value);

		error = abs((long double)val - error);
		m_error += error + _in.m_error;
		return FloatError(val, m_error);
	}

	FloatError operator-(const FloatError& _in)
	{
		float val = m_value - _in.m_value;
		long double error = ((long double)m_value - (long double)_in.m_value);

		error = abs((long double)val - error);
		m_error += error + _in.m_error;
		return FloatError(val, m_error);
	}

	FloatError operator*(const FloatError& _in)
	{
		float val = m_value * _in.m_value;
		long double error = ((long double)m_value * (long double)_in.m_value);

		error = abs((long double)val - error);
		m_error += error + _in.m_error;
		return FloatError(val, m_error);
	}

	FloatError operator/(const FloatError& _in)
	{
		float val = m_value / _in.m_value;
		long double error = ((long double)m_value / (long double)_in.m_value);

		error = abs((long double)val - error);
		m_error += error + _in.m_error;
		return FloatError(val, m_error);
	}

	FloatError operator=(const FloatError& _in)
	{
		m_value = _in.m_value;
		m_error = _in.m_error;
		return FloatError(m_value, m_error);
	}

	std::string m_name;
	float m_value;
	long double m_error;
};