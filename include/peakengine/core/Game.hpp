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

#ifndef _PEAKENGINE_CORE_GAME_HPP_
#define _PEAKENGINE_CORE_GAME_HPP_

#include "../support/Buffer.hpp"

#include <map>

namespace peak
{
	class EntityFactory;
	class Server;
	class Client;
	class Engine;
	class ServerEntity;
	class ClientEntity;

	/**
	 * Main interface providing the basic game actions. This class has to be
	 * implemented, the engine only provides an abstract interface.
	 */
	class Game
	{
		public:
			Game();
			virtual ~Game();

			virtual bool init() = 0;
			virtual bool shutdown() = 0;

			void setEngine(Engine *engine);
			Engine *getEngine();

			virtual Server *createServer(BufferPointer serverdata) = 0;
			virtual Client *createClient(std::string address) = 0;
			virtual Client *createClient(Server *server) = 0;

			void addEntityFactory(EntityFactory *factory, std::string name);
			EntityFactory *getEntityFactory(std::string name);
		private:
			Engine *engine;
			std::map<std::string, EntityFactory*> factories;
	};
}

#endif
