#include "Command_DebugMoveGen.h"
#include "../StringHelper.h"
#include "../Engine.h"
#include <sstream>

Command_DebugMoveGen::Command_DebugMoveGen(Engine* pEngine) :
	Command(pEngine)
{

}

bool Command_DebugMoveGen::Try(const std::string& commandString)
{
	std::string input = commandString;
	if (StringHelper::ToLower(StringHelper::Trim(input)) == "debug-movegen")
	{
		std::stringstream sstream;
		sstream << "Move generation struct sizes:" << std::endl;
		sstream << "  sizeof(MG_CASTLEFLAGS) = " << sizeof(MG_CASTLEFLAGS) << std::endl;
		sstream << "  sizeof(MG_HASH) = " << sizeof(MG_HASH) << std::endl;
		sstream << "  sizeof(MG_JUMPTABLE) = " << sizeof(MG_JUMPTABLE) << std::endl;
		sstream << "  sizeof(MG_MOVE) = " << sizeof(MG_MOVE) << std::endl;
		sstream << "  sizeof(MG_MOVEDATA) = " << sizeof(MG_MOVEDATA) << std::endl;
		sstream << "  sizeof(MG_MOVEGEN) = " << sizeof(MG_MOVEGEN) << std::endl;
		sstream << "  sizeof(MG_MOVEINFO) = " << sizeof(MG_MOVEINFO) << std::endl;
		sstream << "  sizeof(MG_MOVELIST) = " << sizeof(MG_MOVELIST) << std::endl;
		sstream << "  sizeof(MG_PIECETYPE) = " << sizeof(MG_PIECETYPE) << std::endl;
		sstream << "  sizeof(MG_PLAYER) = " << sizeof(MG_PLAYER) << std::endl;
		sstream << "  sizeof(MG_POSITION) = " << sizeof(MG_POSITION) << std::endl;
		sstream << "  sizeof(MG_SLIDEENTRY) = " << sizeof(MG_SLIDEENTRY) << std::endl;
		sstream << "  sizeof(MG_SLIDELOOKUP) = " << sizeof(MG_SLIDELOOKUP) << std::endl;
		sstream << "  sizeof(MG_SLIDEMASKS) = " << sizeof(MG_SLIDEMASKS) << std::endl;
		sstream << "  sizeof(MG_JUMPTARGETSINDEX) = " << sizeof(MG_JUMPTARGETSINDEX) << std::endl;
		sstream << "  sizeof(MG_OPTIONINDEX) = " << sizeof(MG_OPTIONINDEX) << std::endl;
		sstream << "  sizeof(MG_MOVEINDEX) = " << sizeof(MG_MOVEINDEX) << std::endl;
		sstream << "  sizeof(MG_TABLEINDEX) = " << sizeof(MG_TABLEINDEX) << std::endl;
		sstream << "  sizeof(MG_SLIDEENTRYINDEX) = " << sizeof(MG_SLIDEENTRYINDEX) << std::endl;
		sstream << "  sizeof(MG_SLIDEMASKINDEX) = " << sizeof(MG_SLIDEMASKINDEX) << std::endl;
		sstream << "  sizeof(MG_SLIDETARGETINDEX) = " << sizeof(MG_SLIDETARGETINDEX) << std::endl;
#ifdef MOVEGEN_COMPACT_TARGETS
		sstream << "  sizeof(MG_COMPRESSEDTARGETS) = " << sizeof(MG_COMPRESSEDTARGETS) << std::endl;
#endif
		sstream << "Count moves (white): " << GetEngine().MoveGen().CountMoves[PLAYER_WHITE] << std::endl;
		sstream << "Count moves (black): " << GetEngine().MoveGen().CountMoves[PLAYER_BLACK] << std::endl;
		sstream << "Count slide-entries: " << GetEngine().MoveGen().CountSlideEntries << std::endl;
		sstream << "Memory moves: " << (GetEngine().MoveGen().CountMoves[PLAYER_WHITE] + GetEngine().MoveGen().CountMoves[PLAYER_BLACK]) * sizeof(MG_MOVE) * 2 << std::endl;
		sstream << "Memory slide-entries: " << GetEngine().MoveGen().CountSlideEntries * sizeof(MG_SLIDEENTRY) << std::endl;
		GetEngine().OutputStream() << sstream.str();
		return true;
	}
	return false;
}
