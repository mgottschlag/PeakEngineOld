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

	class Server : public EntityManager
	{
		public:
			Server(Engine *engine);
			virtual ~Server();

			bool init(BufferPointer serverdata, unsigned int port = 27272);
			virtual bool shutdown();
			virtual bool load(BufferPointer serverdata) = 0;

			virtual BufferPointer onNewConnection(Connection *connection) = 0;
			virtual void onConnectionAccepted(Connection *connection) = 0;

			void addClient(Connection *connection);

			virtual unsigned int getTime();

			void runThread();
		private:
			Thread *thread;
			volatile bool stopping;
			Connection * volatile localconnection;
			std::vector<Connection*> clients;
	};
}

#endif
