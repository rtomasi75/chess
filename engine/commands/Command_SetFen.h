#pragma once
#ifndef COMMAND_SETFEN_H
#define COMMAND_SETFEN_H

#include "../Command.h"

class Command_SetFen : public Command
{
public:
	Command_SetFen(Engine* pEngine);
	virtual bool Try(const std::string& commandString) override;
};


#endif