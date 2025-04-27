#pragma once
#ifndef COMMAND_DEBUGSLIDEMASK_H
#define COMMAND_DEBUGSLIDEMASK_H

#include "../Command.h"

class Command_DebugSlideMask : public Command
{
private:
	static std::string SlideMaskTableIndexToString(const int& index);
public:
	Command_DebugSlideMask(Engine* pEngine);
	virtual bool Try(const std::string& commandString) override;
};


#endif