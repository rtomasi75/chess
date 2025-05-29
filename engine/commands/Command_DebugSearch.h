#pragma once
#ifndef COMMAND_DEBUGSEARCH_H
#define COMMAND_DEBUGSEARCH_H

#include "../Command.h"

class Command_DebugSearch : public Command
{
public:
	Command_DebugSearch(Engine* pEngine);
	virtual bool Try(const std::string& commandString) override;
};


#endif