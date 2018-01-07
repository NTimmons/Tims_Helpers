#pragma once

#include <map>
#include <string>
#include <array>
#include <type_traits>

//hard-coded test
struct FunctionInformation_IntInt_Int
{
	FunctionInformation_IntInt_Int() : inputA(0), inputB(0), returnVal(0){}
	int inputA;
	int inputB;
	int returnVal;
};

class ZeroSizeClass
{
public:
	ZeroSizeClass() {}
	template<typename T> ZeroSizeClass(T _0) {}
};

template < typename T >
class InputVariableTemplateA
{
public:
	T m_inputVariableA;
};

template < typename T >
class InputVariableTemplateB
{
public:
	T m_inputVariableB;
};

template < typename T >
class InputVariableTemplateC
{
public:
	T m_inputVariableC;
};

template < typename T >
class InputVariableTemplateD
{
public:
	T m_inputVariableD;
};

template < typename T >
class InputVariableTemplateE
{
public:
	T m_inputVariableE;
};

template	<	typename R, 
				typename A = ZeroSizeClass, typename B = ZeroSizeClass, 
				typename C = ZeroSizeClass, typename D = ZeroSizeClass, 
				typename E = ZeroSizeClass  >
class FunctionInformation
	:	std::conditional< std::is_same<A, ZeroSizeClass>::value, InputVariableTemplateA<ZeroSizeClass>, InputVariableTemplateA<A> >::type,
		std::conditional< std::is_same<B, ZeroSizeClass>::value, InputVariableTemplateB<ZeroSizeClass>, InputVariableTemplateB<B> >::type,
		std::conditional< std::is_same<C, ZeroSizeClass>::value, InputVariableTemplateC<ZeroSizeClass>, InputVariableTemplateC<C> >::type,
		std::conditional< std::is_same<D, ZeroSizeClass>::value, InputVariableTemplateD<ZeroSizeClass>, InputVariableTemplateD<D> >::type,
		std::conditional< std::is_same<E, ZeroSizeClass>::value, InputVariableTemplateE<ZeroSizeClass>, InputVariableTemplateE<E> >::type
{
public:
	FunctionInformation() {}
	void Set(	A _a = 0,
				B _b = 0,	
				C _c = 0,
				D _d = 0,	
				E _e = 0
	)
	{
		if (!std::is_same<A, ZeroSizeClass>::value)
			m_inputVariableA = _a;
		if (!std::is_same<B, ZeroSizeClass>::value)
			m_inputVariableB = _b;
		if (!std::is_same<C, ZeroSizeClass>::value)
			m_inputVariableC = _c;
		if (!std::is_same<D, ZeroSizeClass>::value)
			m_inputVariableD = _d;
		if (!std::is_same<E, ZeroSizeClass>::value)
			m_inputVariableE = _e;
	}

	R _returnValue;
};

template<typename... Types>
constexpr auto GetTypesOffsets() 
{
	auto typeSizes	= std::array<std::size_t, sizeof...(Types)>{sizeof(Types)...};
	auto offsets	= std::array<std::size_t, sizeof...(Types)>();

	int offset = 0;
	offsets[0] = 0;
	for (int index = 1; index < sizeof...(Types); index++)
	{
		offsets[index] = offset + typeSizes[index - 1];
	}

	return offsets;
}

template<int N, typename T, T ... Ts>
struct GetN 
{
	static T values[sizeof...(Ts)] = { Ts... };
	static const T value = values[N];
};

template<int N, typename T, T ... Ts>
T GetN<N, T, Ts...>::values[sizeof...(Ts)];

template<typename T>
bool CheckValid(const T& inputValue, const T& referenceValue, const double& percentage)
{
	const double min = (double)(referenceValue)-(double)(referenceValue*(1.0 - percentage));
	const double max = (double)(referenceValue)+(double)(referenceValue*(1.0 - percentage));
	if (inputValue < min || inputValue > max)
	{
		return false;
	}
	return true;
}

template<typename T, typename ret, typename argV>
const argV MemberVariablePointer(const char* voidFi, const int& _index, const T& _offsets)
{
	const char* dataPointer = voidFi + _offsets[_index];// +sizeof(ret);
	const argV* d = (const argV*)dataPointer;
	return *d;
}

//Generated Memoise Function
#define CACHE_SIZE 10
template<typename Fn, Fn func, int percentageErrorAllowed, typename... Args>
typename std::result_of<Fn(Args...)>::type MemoiseWrapperFunction(Args&&... args)
{
	////Approximate Information
	static std::array< FunctionInformation<std::result_of<Fn(Args...)>::type, Args...>, CACHE_SIZE > globalFunctionResultCache;
	static		int				currentIndex		= 0;
	constexpr	double			percentageError		= double(percentageErrorAllowed) / (100.f);
	constexpr	unsigned int	inputVariableCount	= sizeof...(Args);
	constexpr	auto			argOffsets			= GetTypesOffsets<Args...>();

	//Check if there is a stored close enough value
	for (int ii = 0; ii < CACHE_SIZE; ii++)
	{
		const char*	voidFi			= (const char*)&globalFunctionResultCache[ii];
		bool		inRange			= true;
		int			variableIndex	= 0;

		const bool results[] = 
								{ (CheckValid( args, MemberVariablePointer	<	decltype(argOffsets), 
																				std::result_of<Fn(Args...)>::type,
																				std::remove_reference<decltype(args)>::type 
																			>	(voidFi, variableIndex++, argOffsets) , percentageError))... };
		for (const bool res : results)
		{
			if (!res)
			{
				inRange = false; break;
			}
		}

		if (inRange)
		{
			//We have a match in the cache.
			return globalFunctionResultCache[ii]._returnValue;
		}
	}
	
	//No match found.
	//Do work and store cache result
	const int writeindex = currentIndex;
	globalFunctionResultCache[writeindex]._returnValue = func(std::forward<Args>(args)...);
	globalFunctionResultCache[writeindex].Set(std::forward<Args>(args)...);

	//Setup next cache index
	currentIndex++;
	currentIndex = currentIndex % CACHE_SIZE;

	//Return results
	return globalFunctionResultCache[writeindex]._returnValue;
}
#define MemoWrapper(FUNC, ERRORPERC) MemoiseWrapperFunction<decltype(&FUNC), &FUNC, ERRORPERC>



//Hard-coded Approximate Memoise
int ApproximateMemoise_Addition(int _A, int _B)
{
	//Approximate Information
	static std::array< FunctionInformation_IntInt_Int, 5 > globalFunctionResultCache;
	int allowedOffset_A = 2;
	int allowedOffset_B = 2;
	static int currentIndex = 0;

	//Check if there is a stored close enough value
	for (auto fi : globalFunctionResultCache)
	{
		bool valid_a = abs(_A - fi.inputA) < allowedOffset_A;
		bool valid_b = abs(_B - fi.inputB) < allowedOffset_B;
		if (valid_a && valid_b)
		{
			//We have a match in the cache.
			return fi.returnVal;
		}
	}

	//No match found.
	//Do work and store cache result

	int result = _A + _B;

	//Fill cache entry.
	FunctionInformation_IntInt_Int funcInfo;
	funcInfo.inputA = _A;
	funcInfo.inputB = _B;
	funcInfo.returnVal = result;

	globalFunctionResultCache[currentIndex] = std::move(funcInfo);
	currentIndex++;

	return result;

}