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
public:
	const MG_PLAYER& MovingPlayer() const;
	const MG_PLAYER& PassivePlayer() const;
	std::string Version() const;
	bool IsUCI();
	std::istream& InputStream();
	std::ostream& OutputStream();
	MG_POSITION& Position();
	const MG_POSITION& Position() const;
	const MG_MOVEGEN& MoveGen() const;
	const MG_MOVELIST& LegalMoves() const;
	void Start();
	void Stop();
	void WaitForStop();
	void ErrorMessage(const std::string&);
	void MakeMove(const MG_MOVE& move);
	Engine(std::istream& inputStream, std::ostream& outputStream);
	~Engine();
};

#endif