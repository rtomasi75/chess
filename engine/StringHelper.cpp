#include "StringHelper.h"
#include <algorithm>
#include <cctype>

std::string& StringHelper::ToLower(std::string& input)
{
	std::transform(input.begin(), input.end(), input.begin(), [](unsigned char c)
		{
			return std::tolower(c);
		});
	return input;
}

std::string& StringHelper::ToUpper(std::string& input)
{
	std::transform(input.begin(), input.end(), input.begin(), [](unsigned char c)
		{
			return std::toupper(c);
		});
	return input;
}

std::string& StringHelper::TrimLeft(std::string& input)
{
	input.erase(input.begin(), std::find_if(input.begin(), input.end(), [](unsigned char ch)
		{
			return !std::isspace(ch);
		}));
	return input;
}

std::string& StringHelper::TrimRight(std::string& input)
{
	input.erase(std::find_if(input.rbegin(), input.rend(), [](unsigned char ch)
		{
			return !std::isspace(ch);
		}).base(), input.end());
	return input;
}

std::string& StringHelper::Trim(std::string& input)
{
	return TrimRight(TrimLeft(input));
}

bool StringHelper::NextToken(const std::string& commandString, std::string& token, std::string& remainder)
{
	int foundPos = -1;
	for (size_t pos = 0; pos < commandString.size(); pos++)
	{
		if (commandString[pos] == ' ')
		{
			foundPos = (int)pos;
			break;
		}
	}
	if (foundPos < 0)
		return false;
	token = commandString.substr(0, foundPos);
	remainder = commandString.substr(foundPos + 1, commandString.size() - foundPos - 1);
	return true;
}
