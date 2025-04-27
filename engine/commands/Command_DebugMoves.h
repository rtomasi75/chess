#pragma once
#ifndef COMMAND_DEBUGMOVES_H
#define COMMAND_DEBUGMOVES_H

#include "../Command.h"

class Command_DebugMoves : public Command
{
public:
	Command_DebugMoves(Engine* pEngine);
	virtual bool Try(const std::string& commandString) override;
};


#endif