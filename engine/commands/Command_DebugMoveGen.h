#pragma once
#ifndef COMMAND_DEBUGMOVEGEN_H
#define COMMAND_DEBUGMOVEGEN_H

#include "../Command.h"

class Command_DebugMoveGen : public Command
{
public:
	Command_DebugMoveGen(Engine* pEngine);
	virtual bool Try(const std::string& commandString) override;
};


#endif