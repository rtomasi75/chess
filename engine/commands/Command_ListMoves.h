#pragma once
#ifndef COMMAND_LISTMOVES_H
#define COMMAND_LISTMOVES_H

#include "../Command.h"

class Command_ListMoves : public Command
{
public:
	Command_ListMoves(Engine* pEngine);
	virtual bool Try(const std::string& commandString) override;
};


#endif