#pragma once
#ifndef COMMAND_DEBUGRETENTION_H
#define COMMAND_DEBUGRETENTION_H

#include "../Command.h"

class Command_DebugRetention : public Command
{
public:
	Command_DebugRetention(Engine* pEngine);
	virtual bool Try(const std::string& commandString) override;
};

#endif