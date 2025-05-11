#pragma once
#ifndef COMMAND_DEBUGSLIDETARGETS_H
#define COMMAND_DEBUGSLIDETARGETS_H

#include "../Command.h"

class Command_DebugSlideTargets : public Command
{
private:
	static std::string SlideMaskTableIndexToString(const int& index);
public:
	Command_DebugSlideTargets(Engine* pEngine);
	virtual bool Try(const std::string& commandString) override;
};


#endif