#pragma once
#ifndef COMMAND_DEBUGBOARD_H
#define COMMAND_DEBUGBOARD_H

#include "../Command.h"

class Command_DebugBoard : public Command
{
public:
	Command_DebugBoard(Engine* pEngine);
	virtual bool Try(const std::string& commandString) override;
};


#endif