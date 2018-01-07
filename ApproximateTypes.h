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

//Example approximate type function overload.
template< typename T, int accNumurator >
float sinf(ApproxType<T, accNumurator> _x)
{
	static constexpr int accuracy = 8;
	typedef std::conditional < accuracy >= accNumurator, FuncWrapper<ApproxSin_Order7>, FuncWrapper<std::sinf> >::type A;

	return A::Call(_x.Get());
}