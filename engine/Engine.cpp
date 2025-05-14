#include "Engine.h"
#include "commands/Command_Quit.h"
#include "commands/Command_Version.h"
#include "commands/Command_DebugBoard.h"
#include "commands/Command_DebugMoves.h"
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

Engine::Engine(std::istream& inputStream, std::ostream& outputStream) :
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
}

Engine::~Engine()
{
	Stop();
	WaitForStop();
	MOVEGEN_Deinitialize(&_moveGen);
}

const MG_MOVELIST& Engine::LegalMoves() const
{
	return _game.LegalMoves;
}

const MG_MOVEGEN& Engine::MoveGen() const
{
	return _moveGen;
}

const MG_POSITION& Engine::Position() const
{
	return _game.CurrentPosition;
}

void Engine::SetPosition(const MG_POSITION& newPosition)
{
	_game.CurrentPosition = newPosition;
	MOVEGEN_GenerateMoves(&_moveGen, &_game.CurrentPosition, &_game.LegalMoves);

}

MG_POSITION& Engine::Position()
{
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
	std::lock_guard<std::mutex> lock(_stopMutex);
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
	std::string inputLine;
	while (_isRunning)
	{
		std::getline(_inputStream, inputLine);
		if (!TryParse(inputLine))
		{
			ErrorMessage("Could not parse command.");
		}
	}
	std::lock_guard<std::mutex> lock(_stopMutex);
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
	return "Pygmalion ver. 1.0 - by Roland Tomasi";
}

void Engine::MakeMove(const MG_MOVE& move)
{
	GAME_MakeMove(&_game, move, &_moveGen);
}

const MG_PLAYER& Engine::MovingPlayer() const
{
	return Position().MovingPlayer;
}

const MG_PLAYER& Engine::PassivePlayer() const
{
	return Position().PassivePlayer;
}


