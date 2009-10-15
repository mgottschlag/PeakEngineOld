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

#include "peakengine/core/Client.hpp"
#include "peakengine/network/LocalConnection.hpp"
#include "peakengine/core/Server.hpp"
#include "peakengine/network/NetworkData.hpp"
#include "peakengine/core/Engine.hpp"
#include "peakengine/core/Game.hpp"
#include "peakengine/entity/EntityFactory.hpp"
#include "peakengine/entity/ClientEntity.hpp"

namespace peak
{
	Client::Client(Engine *engine) : EntityManager(engine), connection(0)
	{
	}
	Client::~Client()
	{
	}

	bool Client::init(std::string address)
	{
		return false;
	}
	bool Client::initLocally(Server *server)
	{
		// Create local connection
		LocalConnection *a = new LocalConnection();
		LocalConnection *b = new LocalConnection();
		LocalConnection::create(*a, *b);
		connection = a;
		// Connect to the server
		server->addClient(b);
		// Wait for initial data
		BufferPointer initialdata;
		while (!initialdata)
		{
			if (connection->hasData())
			{
				initialdata = connection->receive();
				PacketType type = (PacketType)initialdata->read8();
				if (type != EPT_InitialData)
				{
					initialdata = 0;
				}
			}
		}
		// Initialize the game
		load(initialdata);
		// TODO
		return true;
	}
	bool Client::shutdown()
	{
	}

	unsigned int Client::getTime()
	{
	}

	void Client::update()
	{
		// Receive data
		while (connection->hasData())
		{
			BufferPointer data = connection->receive();
			PacketType type = (PacketType)data->read8();
			switch (type)
			{
				case EPT_EntityCreated:
				{
					unsigned int id = data->read16();
					std::string type = data->readString();
					// Create entity
					Game *game = getEngine()->getGame();
					EntityFactory *factory = game->getEntityFactory(type);
					if (!factory)
					{
						// TODO: Warn
						continue;
					}
					ClientEntity *entity = factory->createClientEntity(this);
					entity->setID(id);
					entity->setState(data.get());
					addEntity(entity);
					break;
				}
				case EPT_EntityDeleted:
				{
					unsigned int id = data->read16();
					Entity *entity = getEntity(id);
					removeEntity(entity);
					delete entity;
					break;
				}
				default:
					// TODO: Warn
					break;
			}
		}
		// Update entities
		EntityManager::update();
		// Send messages
		// TODO
	}
}