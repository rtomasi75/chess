#pragma once
#ifndef ENGINE_H
#define ENGINE_H

#include "libMovegen.h"
#include "libSearch.h"
#include <istream>
#include <ostream>
#include <string>
#include <vector>
#include <memory>
#include "Command.h"
#include <thread>
#include <atomic>
#include <condition_variable>
#include <mutex>

enum class EngineStartupMode
{
	Default,
	AutoPerft6
};

class Engine
{
private:
	std::atomic_bool _isRunning;
	std::istream& _inputStream;
	std::ostream& _outputStream;
	bool _isStopped;
	MG_MOVEGEN _moveGen;
	SE_GAME _game;
	std::vector<std::unique_ptr<Command>> _basicCommands;
	std::condition_variable _signalStop;
	std::mutex _stopMutex;
	std::unique_ptr<std::thread> _pMainThread;
	bool _UCI;
	bool TryParse(const std::string& commandString);
	void MainThread();
	void AutoPerft(const int& depth);
public:
	const MG_PLAYER& MovingPlayer() const;
	const MG_PLAYER& PassivePlayer() const;
	std::string Version() const;
	bool IsUCI();
	std::istream& InputStream();
	std::ostream& OutputStream();
	MG_POSITION& Position();
	const SE_GAME& Game() const;
	const MG_POSITION& Position() const;
	const MG_MOVEGEN& MoveGen() const;
	const MG_MOVELIST& LegalMoves() const;
	void Start();
	void Stop();
	void WaitForStop();
	void ErrorMessage(const std::string&);
	void MakeMove(const MG_MOVE& move);
	void UnmakeMove();
	void SetPosition(const MG_POSITION& newPosition);
	Engine(std::istream& inputStream, std::ostream& outputStream, EngineStartupMode startupMode = EngineStartupMode::Default);
	~Engine();
};

#endif