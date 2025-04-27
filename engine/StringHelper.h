#pragma once
#ifndef STRINGHELPER_H
#define STRINGHELPER_H

#include <string>
#include <sstream>
#include <iomanip>

class StringHelper
{
public:
	static std::string& ToLower(std::string& input);
	static std::string& TrimLeft(std::string& input);
	static std::string& TrimRight(std::string& input);
	static std::string& Trim(std::string& input);
	template< typename T >
	static std::string ToHexString(T i)
	{
		std::stringstream stream;
		stream << "0x"
			<< std::setfill('0') << std::setw(sizeof(T) * 2)
			<< std::hex << i;
		return stream.str();
	}
	static bool NextToken(const std::string& commandString, std::string& token, std::string& remainder);
};

#endif