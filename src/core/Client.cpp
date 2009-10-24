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
#include "peakengine/network/NetworkClient.hpp"
#include "peakengine/core/Engine.hpp"
#include "peakengine/core/Game.hpp"
#include "peakengine/entity/EntityFactory.hpp"
#include "peakengine/entity/ClientEntity.hpp"
#include "peakengine/support/OS.hpp"

#include <iostream>

namespace peak
{
	Client::Client(Engine *engine) : EntityManager(engine), connection(0), client(0)
	{
	}
	Client::~Client()
	{
	}

	bool Client::init(std::string address, unsigned int port, unsigned int ms)
	{
		client = new NetworkClient();
		if(!client->init())
			return false;
		if(!client->connect(address, port, ms))
			return false;
		return true;
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
		time = 0;
		lastupdate = 0;
		load(initialdata);
		// TODO
		stopping = false;
		return true;
	}
	bool Client::shutdown()
	{
		if(!client->shutdown())
			return false;
		return true;
	}

	void Client::sendEntityMessage(Entity *entity, BufferPointer data,
		bool reliable)
	{
		if (reliable)
		{
			// Send message directly
			BufferPointer msg = new Buffer();
			msg->write8(EPT_EntityMessage);
			msg->write16(entity->getID() - 1);
			*msg.get() += *data.get();
			connection->send(msg, true);
		}
		else
		{
			// Put the message into the queue
			entitymessages.push(EntityMessage(entity, data));
		}
	}

	unsigned int Client::getTime()
	{
		return time;
	}

	void Client::runThread()
	{
		uint64_t lastframe = OS::getSystemTime();
		while (!stopping)
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
					case EPT_Update:
					{
						bool updatevalid = true;
						unsigned int updatetime = data->read32();
						unsigned int updateclienttime = data->read32();
						// Adjust time if the latency has decreased
						// TODO: This might affect client prediction
						if (updatetime > time)
							time = updatetime;
						// Apply the updates
						while (data->getPosition() + 16 <= data->getSize() * 8)
						{
							unsigned int id = data->read16() + 1;
							ClientEntity *entity = (ClientEntity*)getEntity(id);
							// Ignore invalid updates
							if (!entity)
							{
								updatevalid = false;
								break;
							}
							// Apply backup to remove any user changes
							if (entity->hasBackup())
								entity->applyBackup();
							// Apply update
							entity->applyUpdate(data.get(), updatetime);
							entity->onUpdate(updateclienttime);
							std::cout << "Update applied for " << id << "." << std::endl;
						}
						// Only send the server that we have received this
						// update if it was valid
						if (updatevalid)
							lastupdate = updatetime;
						break;
					}
					case EPT_EntityMessage:
					{
						unsigned int id = data->read16() + 1;
						Entity *entity = getEntity(id);
						entity->receiveMessage(data.get());
						break;
					}
					default:
						// TODO: Warn
						break;
				}
			}
			// Update entities
			time++;
			update();
			// Send messages
			BufferPointer update = new Buffer();
			update->write8(EPT_Update);
			update->write32(lastupdate);
			update->write32(time);
			while (entitymessages.size() > 0)
			{
				EntityMessage message = entitymessages.front();
				entitymessages.pop();
				update->write16(message.entity->getID() - 1);
				update->write16(message.data->getSize());
				*update.get() += *message.data.get();
			}
			connection->send(update);
			// 20 ms per frame
			lastframe = lastframe + 20000;
			uint64_t currenttime = OS::getSystemTime();
			if (currenttime < lastframe)
			{
				float percentage = (float)(lastframe - currenttime) / 200;
				//std::cout << "Server: " << lastframe - currenttime << std::endl;
				std::cout << std::fixed;
				std::streamsize prec = std::cout.precision(1);
				std::cout << "Client: " << percentage << "%" << std::endl;
				std::cout.precision(prec);
				OS::sleep(lastframe - currenttime);
			}
		}
	}
}
