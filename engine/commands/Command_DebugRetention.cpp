#include "Command_DebugRetention.h"
#include "../StringHelper.h"
#include "../Engine.h"

Command_DebugRetention::Command_DebugRetention(Engine* pEngine) :
	Command(pEngine)
{

}

bool Command_DebugRetention::Try(const std::string& commandString)
{
	std::string input = commandString;
	if (StringHelper::ToLower(StringHelper::Trim(input)) == "debug-retention")
	{
		std::stringstream sstream;
		for (SE_THREADINDEX threadIndex = 0; threadIndex < GetEngine().ThreadCount(); threadIndex++)
		{
			sstream << "Thread " << threadIndex << ":" << std::endl;
			CM_COUNTER yields;
			CM_COUNTER sleeps;
			CM_COUNTER transitions;
			CM_COUNTER wakeUps;
			GetEngine().GetRetentionInfo(threadIndex, sleeps, yields, wakeUps, transitions);
			sstream << "  Yields: " << yields << std::endl;
			sstream << "  Sleeps: " << sleeps << std::endl;
			sstream << "  Wake-ups: " << wakeUps << std::endl;
			sstream << "  Transitions: " << transitions << std::endl;
		}
		GetEngine().OutputStream() << sstream.str();
		return true;
	}
	return false;
}
