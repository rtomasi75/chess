#include "main.h"
#include "libCommon.h"
#include "libBitboard.h"
#include "libMovegen.h"
#include "libEngine.h"
#include <iostream>
#include <unordered_map>
#include <string>

static const std::unordered_map<std::string, EngineStartupMode> g_argMap =
{
	{"--autoperft6", EngineStartupMode::AutoPerft6}
};

int main(int argc, char* argv[])
{
	EngineStartupMode startupMode = EngineStartupMode::Default;
	if (argc > 1) // automated mode - don't start the engine, just handle the run
	{
		const auto it = g_argMap.find(argv[1]);
		if (it != g_argMap.end())
			startupMode = it->second;
	}
	Engine engine(std::cin, std::cout, startupMode);
	if (startupMode == EngineStartupMode::Default)
	{
		engine.Start();
		engine.WaitForStop();
	}
	return 0;
}
