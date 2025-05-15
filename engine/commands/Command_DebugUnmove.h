#pragma once
#ifndef COMMAND_DEBUGUNMOVE_H
#define COMMAND_DEBUGUNMOVE_H

#include "../Command.h"

class Command_DebugUnmove : public Command
{
public:
	Command_DebugUnmove(Engine* pEngine);
	virtual bool Try(const std::string& commandString) override;
};


#endif