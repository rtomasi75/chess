#include "Command_DebugSearch.h"
#include "../StringHelper.h"
#include "../Engine.h"
#include <sstream>

Command_DebugSearch::Command_DebugSearch(Engine* pEngine) :
	Command(pEngine)
{

}

bool Command_DebugSearch::Try(const std::string& commandString)
{
	std::string input = commandString;
	if (StringHelper::ToLower(StringHelper::Trim(input)) == "debug-search")
	{
		std::stringstream sstream;
		sstream << "Runtime configuration:" << std::endl;
		sstream << "  Thread count = " << GetEngine().ThreadCount() << std::endl;
		sstream << "  Thread memory = " << GetEngine().ThreadCount() * sizeof(SE_THREAD) << std::endl;
		sstream << "  FSM stack depth = " << SEARCH_MAX_DEPTH << std::endl;
		sstream << std::endl;
		sstream << "Search struct sizes:" << std::endl;
		PrintStructLayout<SE_POSITIONCOUNT>(sstream, "SE_POSITIONCOUNT", 2);
		PrintStructLayout<SE_LEAFCOUNT>(sstream, "SE_LEAFCOUNT", 2);
		PrintStructLayout<SE_DEPTH>(sstream, "SE_DEPTH", 2);
		PrintStructLayout<SE_CONTEXTPTR>(sstream, "SE_CONTEXTPTR", 2);
		PrintStructLayout<SE_EXECUTIONTOKEN>(sstream, "SE_EXECUTIONTOKEN", 2);
		PrintStructLayout<SE_FORKINDEX>(sstream, "SE_FORKINDEX", 2);
		PrintStructLayout<SE_RETENTIONSTATE>(sstream, "SE_RETENTIONSTATE", 2);
		PrintStructLayout<SE_ATOMICRETENTIONSTATE>(sstream, "SE_ATOMICRETENTIONSTATE", 2);
		PrintStructLayout<SE_SYSTEMTHREADHANDLE>(sstream, "SE_SYSTEMTHREADHANDLE", 2);
		PrintStructLayout<SE_TERMINATIONFLAG>(sstream, "SE_TERMINATIONFLAG", 2);
		PrintStructLayout<SE_THREAD>(sstream, "SE_THREAD", 2);
		PrintStructLayout<SE_CONTEXT_PERFT>(sstream, "SE_CONTEXT_PERFT", 2);
		PrintStructLayout<SE_PLAYEDMOVE>(sstream, "SE_PLAYEDMOVE", 2);
		PrintStructLayout<SE_NODESTATE>(sstream, "SE_NODESTATE", 2);
		PrintStructLayout<SE_NODEFLAGS>(sstream, "SE_NODEFLAGS", 2);
		PrintStructLayout<SE_FORKCOUNT>(sstream, "SE_FORKCOUNT", 2);
		PrintStructLayout<SE_NODE>(sstream, "SE_NODE", 2);
		PrintStructLayout<SE_HOSTCONTEXT>(sstream, "SE_HOSTCONTEXT", 2);
		PrintStructLayout<SE_SEARCHCONTEXTSTORAGE>(sstream, "SE_SEARCHCONTEXTSTORAGE", 2);
		PrintStructLayout<SE_PLAYEDMOVEINDEX>(sstream, "SE_PLAYEDMOVEINDEX", 2);
		PrintStructLayout<SE_GAME>(sstream, "SE_GAME", 2);
		PrintStructLayout<SE_FORKSUBMASK>(sstream, "SE_FORKSUBMASK", 2);
		PrintStructLayout<SE_FORKMASK>(sstream, "SE_FORKMASK", 2);
		PrintStructLayout<SE_FSM>(sstream, "SE_FSM", 2);
		PrintStructLayout<SE_FORK>(sstream, "SE_FORK", 2);
		PrintStructLayout<SE_DISPATCHMASK>(sstream, "SE_DISPATCHMASK", 2);
		PrintStructLayout<SE_DISPATCHER>(sstream, "SE_DISPATCHER", 2);
		PrintStructLayout<SE_ATOMICCONTROLFLAGS>(sstream, "SE_ATOMICCONTROLFLAGS", 2);
		PrintStructLayout<SE_CONTROLFLAGS>(sstream, "SE_CONTROLFLAGS", 2);
		PrintStructLayout<SE_FN_ONFSMCOMPLETE>(sstream, "SE_FN_ONFSMCOMPLETE", 2);
		PrintStructLayout<SE_FN_AGGREGATECONTEXT>(sstream, "SE_FN_AGGREGATECONTEXT", 2);
		PrintStructLayout<SE_CALLBACKS>(sstream, "SE_CALLBACKS", 2);
		GetEngine().OutputStream() << sstream.str();
		return true;
	}
	return false;
}
