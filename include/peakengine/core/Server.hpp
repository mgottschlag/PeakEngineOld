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

#ifndef _PEAKENGINE_CORE_SERVER_HPP_
#define _PEAKENGINE_CORE_SERVER_HPP_

#include "../entity/EntityManager.hpp"
#include "../support/Buffer.hpp"

namespace peak
{
	class Connection;
	class Thread;

	class ClientInfo
	{
		public:
			ClientInfo(Connection *connection) : connection(connection),
				lastreceived(0)
			{
				id = ++lastid;
			}

			Connection *connection;
			unsigned int id;
			static unsigned int lastid;
			unsigned int lastreceived;
	};

	/**
	 * Base class for game servers.
	 */
	class Server : public EntityManager
	{
		public:
			/**
			 * Constructor.
			 */
			Server(Engine *engine);
			/**
			 * Destructor.
			 */
			virtual ~Server();

			/**
			 * Creates the server. Calls load() after the network socket has
			 * been set up.
			 */
			bool init(BufferPointer serverdata, unsigned int port = 27272);
			/**
			 * Closes the server. Can be implemented by the user.
			 */
			virtual bool shutdown();
			/**
			 * Creates the server. Has to be implemented by the user.
			 */
			virtual bool load(BufferPointer serverdata) = 0;

			/**
			 * Adds an entity to the server. This must only be called once for
			 * every entity.
			 */
			virtual void addEntity(Entity *entity);
			/**
			 * Removes an entity from the server.
			 */
			virtual void removeEntity(Entity *entity);

			/**
			 * This is called when a new client connects to the server.
			 * @return Initial server data for the client. If 0 is returned,
			 * the connection is closed at once.
			 */
			virtual BufferPointer onNewConnection(Connection *connection) = 0;
			/**
			 * This is called after a client has finished connecting.
			 */
			virtual void onConnectionAccepted(Connection *connection) = 0;

			/**
			 * Adds a client to the server. Only called from within
			 * Client::initLocally().
			 */
			void addClient(Connection *connection);

			/**
			 * Sends a message to a certain entity. This usually is not called
			 * directly but rather through ServerEntity::sendMessage().
			 */
			void sendEntityMessage(Entity *entity, BufferPointer data,
				bool reliable = false);

			/**
			 * Returns the game time of the server. The game time is incremented
			 * every 20 ms.
			 */
			virtual unsigned int getTime();

			/**
			 * Main server game loop.
			 */
			void runThread();
		private:
			Thread *thread;
			volatile bool stopping;
			Connection * volatile localconnection;
			std::vector<ClientInfo> clients;
			unsigned int time;
	};
}

#endif
