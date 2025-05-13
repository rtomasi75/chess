#pragma once
#ifndef COMMAND_PERFT_H
#define COMMAND_PERFT_H

#include "../Command.h"

class Command_Perft : public Command
{
public:
	Command_Perft(Engine* pEngine);
	virtual bool Try(const std::string& commandString) override;
};


#endif