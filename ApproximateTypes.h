#pragma once

template< typename T, int accNumurator >
class ApproxType
{
public:
	ApproxType(T _in)
	{
		m_data = _in;
	}

	T& Get()
	{
		return m_data;
	}

	T	m_data;
};

template< float func(float) >
class FuncWrapper
{
public:

	static float Call(const float& _in)
	{
		return func(_in);
	}
};