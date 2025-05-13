#pragma once
#ifndef COMMAND_DIVIDE_H
#define COMMAND_DIVIDE_H

#include "../Command.h"

class Command_Divide : public Command
{
private:
	void SortMoves(MG_MOVELIST& moveList);
public:
	Command_Divide(Engine* pEngine);
	virtual bool Try(const std::string& commandString) override;
};


#endif