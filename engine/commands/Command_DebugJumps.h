#pragma once
#ifndef COMMAND_DEBUGJUMPS_H
#define COMMAND_DEBUGJUMPS_H

#include "../Command.h"

class Command_DebugJumps : public Command
{
private:
	static std::string JumpTableIndexToString(const int& index);
public:
	Command_DebugJumps(Engine* pEngine);
	virtual bool Try(const std::string& commandString) override;
};


#endif