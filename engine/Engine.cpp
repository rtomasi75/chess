#include "Engine.h"
#include "commands/Command_Quit.h"
#include "commands/Command_Version.h"
#include "commands/Command_DebugBoard.h"
#include "commands/Command_DebugMoves.h"
#include "commands/Command_DebugUnmove.h"
#include "commands/Command_ListMoves.h"
#include "commands/Command_DebugMove.h"
#include "commands/Command_DebugJumps.h"
#include "commands/Command_DebugSlideMask.h"
#include "commands/Command_DebugMoveGen.h"
#include "commands/Command_DebugSlideTargets.h"
#include "commands/Command_DebugIntrinsics.h"
#include "commands/Command_DebugAttacks.h"
#include "commands/Command_Perft.h"
#include "commands/Command_Divide.h"
#include "commands/Command_SetFen.h"
#include <sstream>
#include <cstring>

Engine::Engine(std::istream& inputStream, std::ostream& outputStream, EngineStartupMode startupMode) :
	_isRunning(false),
	_inputStream(inputStream),
	_outputStream(outputStream),
	_isStopped(true),
	_pMainThread(nullptr),
	_UCI(false)
{
	CM_DetectIntrinsics();
	MOVEGEN_Initialize(&_moveGen);
	GAME_Initialize(&_game, &_moveGen);
	DISPATCHER_Initialize(&_dispatcher, &_moveGen);
	_basicCommands.emplace_back(std::make_unique<Command_Quit>(this));
	_basicCommands.emplace_back(std::make_unique<Command_Version>(this));
	_basicCommands.emplace_back(std::make_unique<Command_DebugBoard>(this));
	_basicCommands.emplace_back(std::make_unique<Command_DebugMoves>(this));
	_basicCommands.emplace_back(std::make_unique<Command_ListMoves>(this));
	_basicCommands.emplace_back(std::make_unique<Command_DebugMove>(this));
	_basicCommands.emplace_back(std::make_unique<Command_DebugJumps>(this));
	_basicCommands.emplace_back(std::make_unique<Command_DebugSlideMask>(this));
	_basicCommands.emplace_back(std::make_unique<Command_DebugMoveGen>(this));
	_basicCommands.emplace_back(std::make_unique<Command_DebugSlideTargets>(this));
	_basicCommands.emplace_back(std::make_unique<Command_DebugIntrinsics>(this));
	_basicCommands.emplace_back(std::make_unique<Command_DebugAttacks>(this));
	_basicCommands.emplace_back(std::make_unique<Command_Perft>(this));
	_basicCommands.emplace_back(std::make_unique<Command_Divide>(this));
	_basicCommands.emplace_back(std::make_unique<Command_SetFen>(this));
	_basicCommands.emplace_back(std::make_unique<Command_DebugUnmove>(this));
	switch (startupMode)
	{
	default: // just break out into interactive mode
		break;
	case EngineStartupMode::AutoPerft6:
		AutoPerft(6);
		return;
	}

}

void Engine::AutoPerft(const int& depth)
{
	Command_Perft command(this);
	std::stringstream sstream;
	sstream << "perft " << depth;
	command.Try(sstream.str());
	Stop();
}

Engine::~Engine()
{
	Stop();
	WaitForStop();
	DISPATCHER_Deinitialize(&_dispatcher);
	MOVEGEN_Deinitialize(&_moveGen);
}

MG_MOVELIST Engine::LegalMoves() const
{
	std::scoped_lock<std::mutex> lock(_gameMutex);
	return _game.LegalMoves;
}

const MG_MOVEGEN& Engine::MoveGen() const
{
	return _moveGen;
}

SE_GAME Engine::Game() const
{
	std::scoped_lock<std::mutex> lock(_gameMutex);
	return _game;
}

void Engine::SetPosition(const MG_POSITION& newPosition)
{
	std::scoped_lock<std::mutex> lock(_gameMutex);
	_game.CurrentPosition = newPosition;
	MOVEGEN_GenerateMoves(&_moveGen, &_game.CurrentPosition, &_game.LegalMoves);

}

SE_DISPATCHER& Engine::Dispatcher()
{
	return _dispatcher;
}

MG_POSITION Engine::Position() const
{
	std::scoped_lock<std::mutex> lock(_gameMutex);
	return _game.CurrentPosition;
}

bool Engine::TryParse(const std::string& commandString)
{
	for (std::unique_ptr<Command>& pCommand : _basicCommands)
	{
		if (pCommand->Try(commandString))
		{
			return true;
		}
	}
	return false;
}

void Engine::Start()
{
	std::scoped_lock<std::mutex> lock(_stopMutex);
	if (!_isStopped)
		return;
	_isStopped = false;
	_isRunning = true;
	_pMainThread = std::make_unique<std::thread>([this]() {MainThread(); });
}

void Engine::Stop()
{
	_isRunning = false;
}

bool Engine::IsUCI()
{
	return _UCI;
}

void Engine::MainThread()
{
	CM_SetCurrentThreadName("Engine main loop");
	std::string inputLine;
	while (_isRunning)
	{
		std::getline(_inputStream, inputLine);
		if (!TryParse(inputLine))
		{
			ErrorMessage("Could not parse command.");
		}
	}
	std::scoped_lock<std::mutex> lock(_stopMutex);
	_isStopped = true;
	_signalStop.notify_all();
}

void Engine::WaitForStop()
{
	std::unique_lock<std::mutex> lock(_stopMutex);
	while (!_isStopped)
	{
		_signalStop.wait(lock);
		if (_isStopped)
		{
			_pMainThread->join();
			_pMainThread = nullptr;
		}
	}
}

std::istream& Engine::InputStream()
{
	return _inputStream;
}

std::ostream& Engine::OutputStream()
{
	return _outputStream;
}

void Engine::ErrorMessage(const std::string& message)
{
	if (_UCI)
		_outputStream << "info string ERROR: " << message << std::endl;
	else
		_outputStream << "ERROR: " << message << std::endl;
}

std::string Engine::Version() const
{
	return "DixieKnight ver. 1.0 - by Roland Tomasi";
}

void Engine::MakeMove(const MG_MOVE& move)
{
	std::scoped_lock<std::mutex> lock(_gameMutex);
	GAME_MakeMove(&_game, move, &_moveGen);
}

void Engine::UnmakeMove()
{
	std::scoped_lock<std::mutex> lock(_gameMutex);
	GAME_UnmakeMove(&_game, &_moveGen);
}

MG_PLAYER Engine::MovingPlayer() const
{
	std::scoped_lock<std::mutex> lock(_gameMutex);
	return _game.CurrentPosition.Header.MovingPlayer;
}

MG_PLAYER Engine::PassivePlayer() const
{
	std::scoped_lock<std::mutex> lock(_gameMutex);
	return _game.CurrentPosition.Header.PassivePlayer;
}

void Engine::SignalExecutionToken(SE_EXECUTIONTOKEN token, SE_SEARCHCONTEXTSTORAGE* pSearchContext, SE_THREAD* pTerminatingThread)
{
	ExecutionToken* pToken = static_cast<ExecutionToken*>(token);
	pToken->SetSearchContext(pSearchContext);
	pToken->Signal();
}

SE_LEAFCOUNT Engine::Perft(const SE_DEPTH distanceToHorizon, SE_POSITIONCOUNT& nodeCount)
{
	MG_POSITION localPosition;
	memcpy(&localPosition, &_game.CurrentPosition, sizeof(MG_POSITION));
	SE_CONTEXT_PERFT searchContext;
	searchContext.LeafCount = 0;
	SE_HOSTCONTEXT hostContext;
	std::shared_ptr<ExecutionToken> pToken = std::make_shared<ExecutionToken>(&searchContext);
	SE_CALLBACKS callbacks;
	CALLBACKS_Initialize(&callbacks, Engine::SignalExecutionToken);
	callbacks.OnAggregateSearchContext = SEARCH_AggregatePerftContext;
	HOSTCONTEXT_Initialize(&hostContext, &callbacks, &searchContext, pToken.get());
	SEARCH_PerftRoot(&_dispatcher, &callbacks, &localPosition, distanceToHorizon, &hostContext);
	pToken->Wait();
	nodeCount = _dispatcher.pThreadPool[0].NodeCount;
	return reinterpret_cast<const SE_CONTEXT_PERFT*>(pToken->SearchContext())->LeafCount;
}




