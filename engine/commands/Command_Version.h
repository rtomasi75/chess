#pragma once
#ifndef COMMAND_VERSION_H
#define COMMAND_VERSION_H

#include "../Command.h"

class Command_Version : public Command
{
public:
	Command_Version(Engine* pEngine);
	virtual bool Try(const std::string& commandString) override;
};

#endif