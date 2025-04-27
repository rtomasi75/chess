#pragma once
#ifndef COMMAND_QUIT_H
#define COMMAND_QUIT_H

#include "../Command.h"

class Command_Quit : public Command
{
public:
	Command_Quit(Engine* pEngine);
	virtual bool Try(const std::string& commandString) override;
};

#endif