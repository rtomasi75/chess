#pragma once
#ifndef COMMAND_DEBUGMOVE_H
#define COMMAND_DEBUGMOVE_H

#include "../Command.h"

class Command_DebugMove : public Command
{
public:
	Command_DebugMove(Engine* pEngine);
	virtual bool Try(const std::string& commandString) override;
};


#endif