/*
Copyright (c) 2009, Mathias Gottschlag

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include "peakengine/core/Engine.hpp"
#include "peakengine/core/Game.hpp"
#include "peakengine/core/Client.hpp"

#include <enet/enet.h>

namespace peak
{
	Engine::Engine() : game(0)
	{
		// Initialize networking
		enet_initialize();
		// FIXME: This limits us to one existing instance of Engine.
	}
	Engine::~Engine()
	{
		// Shutdown networking
		enet_deinitialize();
	}

	bool Engine::loadGame(std::string filename)
	{
		// TODO
		return false;
	}
	bool Engine::loadGame(Game *game)
	{
		this->game = game;
		game->setEngine(this);
		return true;
	}
	Game *Engine::getGame()
	{
		return game;
	}

	void Engine::setGameDirectory(std::string directory)
	{
		this->directory = directory;
	}
	std::string Engine::getGameDirectory()
	{
		return directory;
	}

	bool Engine::run()
	{
		// Check game
		if (!game)
			return false;
		if (directory == "")
			return false;
		// Initialize engine
		stopping = false;
		if (!game->init())
			return false;
		// TODO
		// Create test game
		BufferPointer buffer = new Buffer();
		buffer->writeString("testmap");
		Server *server = game->createServer(buffer);
		Client *client = game->createClient(server);
		// TODO
		// Client main loop
		client->runThread();
		// Clean up again
		game->shutdown();
		// TODO
		return false;
	}
	bool Engine::runFromEditor()
	{
		// Check game
		if (!game)
			return false;
		if (directory == "")
			return false;
		// Initialize engine
		// TODO
		// Start game
		// TODO
		return false;
	}
	bool Engine::runServer()
	{
		// Check game
		if (!game)
			return false;
		if (directory == "")
			return false;
		// Initialize engine
		// TODO
		// Start game
		// TODO
		return false;
	}
	void Engine::stop()
	{
		stopping = true;
	}

	void Engine::render()
	{
	}
}
