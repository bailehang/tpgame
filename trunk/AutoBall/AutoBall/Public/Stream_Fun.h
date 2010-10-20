#pragma once 

#include <sstream>
#include <string>
#include <iomanip>


template <class T>
inline std::string ttos(const T& t, int precision = 2)
{
	std::ostringstream buffer;

	buffer << std::fixed << std::setprecision(precision) << t;

	return buffer.str();
}

inline std::string btos(bool b)
{
	if (b) return "true";
	return "false";
}

template <typename T>
inline T GetValueFromStream(std::ifstream& stream)
{
	T val;

	stream >> val;
	if (!stream)
	{
		throw std::runtime_error("retrieve wrong type from stream");
	}

	return val;
}

template <typename T>
void WriteBitsToStream(std::ostream& stream, const T& val)
{
	int iNumBits = sizeof(T) * 8;

	while (--iNumBits >= 0)
	{
		if ((iNumBits+1) % 8 == 0) stream << " ";
		unsigned long mask = 1 << iNumBits;
		if (val & mask) stream << "1";
		else stream << "0";
	}
}
