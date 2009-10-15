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

#ifndef _PEAKENGINE_CORE_CLIENT_HPP_
#define _PEAKENGINE_CORE_CLIENT_HPP_

#include "../entity/EntityManager.hpp"
#include "../support/Buffer.hpp"

namespace peak
{
	class Server;
	class Connection;

	class Client : public EntityManager
	{
		public:
			Client(Engine *engine);
			virtual ~Client();

			bool init(std::string address);
			bool initLocally(Server *server);
			virtual bool shutdown();
			virtual bool load(BufferPointer serverdata) = 0;

			virtual unsigned int getTime();

			void runThread();
		private:
			Connection *connection;
			volatile bool stopping;
			unsigned int time;
	};
}

#endif
